//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : pluginmanager.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include "app.h"
#include "environmentconfig.h"
#include "macromanager.h"
#include "build_settings_config.h"
#include "buildmanager.h"
#include "jobqueue.h"
#include "pluginmanager.h"
#include "pluginsdata.h"
#include "pluginconfig.h"
#include "optionsconfig.h"
#include "language.h"
#include "manager.h"
#include "wx/filename.h"
#include <wx/log.h>
#include <wx/dir.h>
#include "frame.h"
#include "generalinfo.h"
#include "editor_config.h"
#include "fileexplorertree.h"
#include "workspace_pane.h"
#include "fileview.h"
#include "wx/xrc/xmlres.h"
#include "ctags_manager.h"
#include "fileexplorer.h"
#include "plugin_version.h"
#include "workspacetab.h"
#include "errorstab.h"

PluginManager *PluginManager::Get()
{
	static PluginManager theManager;
	return &theManager;
}

void PluginManager::UnLoad()
{
	std::map<wxString, IPlugin*>::iterator plugIter = m_plugins.begin();
	for (; plugIter != m_plugins.end(); plugIter++) {
		IPlugin *plugin = plugIter->second;
		plugin->UnPlug();
		delete plugin;
	}

#if wxVERSION_NUMBER < 2900
	std::list<clDynamicLibrary*>::iterator iter = m_dl.begin();
	for ( ; iter != m_dl.end(); iter++ ) {
		( *iter )->Detach();
		delete ( *iter );
	}
#endif

	m_dl.clear();
	m_plugins.clear();
	PluginConfig::Instance()->Release();
}

PluginManager::~PluginManager()
{
}

void PluginManager::Load()
{
	wxString ext;
#if defined (__WXGTK__) || defined (__WXMAC__)
	ext = wxT("so");
#else
	ext = wxT("dll");
#endif

	wxString fileSpec( wxT( "*." ) + ext );
	PluginConfig::Instance()->Load(ManagerST::Get()->GetConfigDir() + wxT("/plugins.xml"), wxT("2.0.1"));

	PluginsData pluginsData;
	PluginConfig::Instance()->ReadObject(wxT("plugins_data"), &pluginsData);

	//get the map of all available plugins
	m_pluginsInfo = pluginsData.GetInfo();
	std::map<wxString, PluginInfo> actualPlugins;

	//set the managers
	//this code assures us that the shared objects will see the same instances as the application
	//does
	LanguageST::Get()->SetTagsManager( GetTagsManager() );
	TagsManagerST::Get()->SetLanguage( LanguageST::Get() );

	// Plugin loading is disabled?
	if (((App*) GetTheApp())->GetLoadPlugins() == false)
		return;

	wxString pluginsDir(ManagerST::Get()->GetPluginsDir());

	if ( wxDir::Exists(pluginsDir) ) {
		//get list of dlls
		wxArrayString files;
		wxDir::GetAllFiles( pluginsDir, &files, fileSpec, wxDIR_FILES );

		for ( size_t i=0; i<files.GetCount(); i++ ) {
			clDynamicLibrary *dl = new clDynamicLibrary();
			wxString fileName( files.Item( i ) );
			if ( !dl->Load( fileName ) ) {
				wxLogMessage( wxT( "Failed to load plugin's dll: " ) + fileName );
				if (!dl->GetError().IsEmpty())
					wxLogMessage(dl->GetError());
#if wxVERSION_NUMBER < 2900
				delete dl;
#endif
				continue;
			}

			bool success( false );
			GET_PLUGIN_INFO_FUNC pfnGetPluginInfo = ( GET_PLUGIN_INFO_FUNC )dl->GetSymbol( wxT( "GetPluginInfo" ), &success );
			if ( !success ) {
#if wxVERSION_NUMBER < 2900
				delete dl;
#endif
				continue;
			}

			// load the plugin version method
			// if the methods does not exist, handle it as if it has value of 100 (lowest version API)
			int interface_version(100);
			GET_PLUGIN_INTERFACE_VERSION_FUNC pfnInterfaceVersion = (GET_PLUGIN_INTERFACE_VERSION_FUNC) dl->GetSymbol(wxT("GetPluginInterfaceVersion"), &success);
			if ( success ) {
				interface_version = pfnInterfaceVersion();
			} else {
				wxLogMessage(wxT("Failed to find GetPluginInterfaceVersion() in dll: ") + fileName);
				if (!dl->GetError().IsEmpty())
					wxLogMessage(dl->GetError());
			}

			if ( interface_version != PLUGIN_INTERFACE_VERSION ) {
				wxLogMessage(wxString::Format(wxT("Version interface mismatch error for plugin '%s'. Plugin's interface version is '%d', EmbeddedLite interface version is '%d'"),
				                              fileName.c_str(),
				                              interface_version,
				                              PLUGIN_INTERFACE_VERSION));
#if wxVERSION_NUMBER < 2900
				delete dl;
#endif
				continue;
			}

			// Check if this dll can be loaded
			PluginInfo pluginInfo = pfnGetPluginInfo();
			std::map< wxString, PluginInfo>::const_iterator iter = m_pluginsInfo.find(pluginInfo.GetName());
			if (iter == m_pluginsInfo.end()) {
				//new plugin?, add it and use the default enabled/disabled for this plugin
				actualPlugins[pluginInfo.GetName()] = pluginInfo;
				if (pluginInfo.GetEnabled() == false) {
#if wxVERSION_NUMBER < 2900
					delete dl;
#endif
					continue;
				}
			} else {
				// we have a match!
				// it means that this plugin was already found in the plugins configuration file,
				// use the value set in it to determine whether this plugin should be enabled or disabled
				PluginInfo pi = iter->second;
				pluginInfo.SetEnabled(pi.GetEnabled());

				actualPlugins[pluginInfo.GetName()] = pluginInfo;
				if (pluginInfo.GetEnabled() == false) {
#if wxVERSION_NUMBER < 2900
					delete dl;
#endif
					continue;
				}
			}

			//try and load the plugin
			GET_PLUGIN_CREATE_FUNC pfn = ( GET_PLUGIN_CREATE_FUNC )dl->GetSymbol( wxT( "CreatePlugin" ), &success );
			if ( !success ) {
				wxLogMessage(wxT("Failed to find CreatePlugin() in dll: ") + fileName);
				if (!dl->GetError().IsEmpty())
					wxLogMessage(dl->GetError());

				//mark this plugin as not available
				pluginInfo.SetEnabled(false);
				actualPlugins[pluginInfo.GetName()] = pluginInfo;

#if wxVERSION_NUMBER < 2900
				delete dl;
#endif
				continue;
			}

			IPlugin *plugin = pfn( ( IManager* )this );
			wxLogMessage( wxT( "Loaded plugin: " ) + plugin->GetLongName() );
			m_plugins[plugin->GetShortName()] = plugin;

			//load the toolbar
			clToolBar *tb = plugin->CreateToolBar( AllowToolbar() ? (wxWindow*)Frame::Get()->GetMainPanel() : (wxWindow*)Frame::Get() );
			if ( tb ) {
				Frame::Get()->GetDockingManager().AddPane( tb, wxAuiPaneInfo().Name( plugin->GetShortName() ).LeftDockable( true ).RightDockable( true ).Caption( plugin->GetShortName() ).ToolbarPane().Top() );

				//Add menu entry at the 'View->Toolbars' menu for this toolbar
				int ii = Frame::Get()->GetMenuBar()->FindMenu( wxT( "View" ) );
				if ( ii != wxNOT_FOUND ) {
					wxMenu *viewMenu = Frame::Get()->GetMenuBar()->GetMenu( ii );
					wxMenu *submenu = NULL;
					wxMenuItem *item = viewMenu->FindItem( XRCID("toolbars_menu") );
					if (item) {
						submenu = item->GetSubMenu();
						//add the new toolbar entry at the end of this menu

						int id = wxNewId();
						wxString text(plugin->GetShortName());
						text << wxT(" ToolBar");
						wxMenuItem *newItem = new wxMenuItem(submenu, id, text, wxEmptyString, wxITEM_CHECK);
						submenu->Append(newItem);
						Frame::Get()->RegisterToolbar(id, plugin->GetShortName());
					}
				}
			}

			//let the plugin plug its menu in the 'Plugins' menu at the menu bar
			//the create menu will be placed as a sub menu of the 'Plugin' menu
			int idx = Frame::Get()->GetMenuBar()->FindMenu( wxT( "Plugins" ) );
			if ( idx != wxNOT_FOUND ) {
				wxMenu *pluginsMenu = Frame::Get()->GetMenuBar()->GetMenu( idx );
				plugin->CreatePluginMenu( pluginsMenu );
			}

			//keep the dynamic load library
			m_dl.push_back( dl );
		}
		Frame::Get()->GetDockingManager().Update();

		//save the plugins data
		PluginsData pluginsData;
		pluginsData.SetInfo(actualPlugins);
		PluginConfig::Instance()->WriteObject(wxT("plugins_data"), &pluginsData);
	}
}

IEditor *PluginManager::GetActiveEditor()
{
	return (IEditor*) Frame::Get()->GetMainBook()->GetActiveEditor();
}

IConfigTool* PluginManager::GetConfigTool()
{
	return EditorConfigST::Get();
}

void PluginManager::HookPopupMenu( wxMenu *menu, MenuType type )
{
	std::map<wxString, IPlugin*>::iterator iter = m_plugins.begin();
	for ( ; iter != m_plugins.end(); iter++ ) {
		iter->second->HookPopupMenu( menu, type );
	}
}

void PluginManager::UnHookPopupMenu( wxMenu *menu, MenuType type )
{
	std::map<wxString, IPlugin*>::iterator iter = m_plugins.begin();
	for ( ; iter != m_plugins.end(); iter++ ) {
		iter->second->UnHookPopupMenu( menu, type );
	}
}

TreeItemInfo PluginManager::GetSelectedTreeItemInfo( TreeType type )
{
	TreeItemInfo info;
	switch ( type ) {
	case TreeFileExplorer:
		return Frame::Get()->GetFileExplorer()->GetFileTree()->GetSelectedItemInfo();
	case TreeFileView:
		return Frame::Get()->GetWorkspaceTab()->GetFileView()->GetSelectedItemInfo();
	default:
		return info;
	}
}

wxTreeCtrl *PluginManager::GetTree(TreeType type)
{
	switch ( type ) {
	case TreeFileExplorer:
		return Frame::Get()->GetFileExplorer()->GetFileTree();
	case TreeFileView:
		return Frame::Get()->GetWorkspaceTab()->GetFileView();
	default:
		return NULL;
	}
}

Notebook *PluginManager::GetOutputPaneNotebook()
{
	return Frame::Get()->GetOutputPane()->GetNotebook();
}

Notebook *PluginManager::GetWorkspacePaneNotebook()
{
	return Frame::Get()->GetWorkspacePane()->GetNotebook();
}

bool PluginManager::OpenFile(const wxString &fileName, const wxString &projectName, int lineno)
{
	return Frame::Get()->GetMainBook()->OpenFile(fileName, projectName, lineno);
}

wxString PluginManager::GetStartupDirectory() const
{
	return ManagerST::Get()->GetStarupDirectory();
}

void PluginManager::AddProject(const wxString &path)
{
	ManagerST::Get()->AddProject(path);
}

bool PluginManager::IsWorkspaceOpen() const
{
	return ManagerST::Get()->IsWorkspaceOpen();
}

TagsManager *PluginManager::GetTagsManager()
{
	return TagsManagerST::Get();
}

Workspace *PluginManager::GetWorkspace()
{
	return WorkspaceST::Get();
}

bool PluginManager::AddFilesToVirtualFolder(wxTreeItemId &item, wxArrayString &paths)
{
	return Frame::Get()->GetWorkspaceTab()->GetFileView()->AddFilesToVirtualFolder(item, paths);
}

bool PluginManager::AddFilesToVirtualFolder(const wxString &vdFullPath, wxArrayString &paths)
{
	return Frame::Get()->GetWorkspaceTab()->GetFileView()->AddFilesToVirtualFolder(vdFullPath, paths);
}

bool PluginManager::AddFilesToVirtualFolderIntelligently(const wxString &vdFullPath, wxArrayString &paths)
{
	return Frame::Get()->GetWorkspaceTab()->GetFileView()->AddFilesToVirtualFolderIntelligently(vdFullPath, paths);
}

int PluginManager::GetToolbarIconSize()
{
	//for now return 24 by default
	OptionsConfigPtr options = EditorConfigST::Get()->GetOptions();
	if (options) {
		return options->GetIconsSize();
	}
	return 24;
}

wxAuiManager* PluginManager::GetDockingManager()
{
	return &Frame::Get()->GetDockingManager();
}

EnvironmentConfig* PluginManager::GetEnv()
{
	return EnvironmentConfig::Instance();
}

JobQueue* PluginManager::GetJobQueue()
{
	return JobQueueSingleton::Instance();
}
wxString PluginManager::GetProjectExecutionCommand(const wxString& projectName, wxString& wd)
{
	return ManagerST::Get()->GetProjectExecutionCommand(projectName, wd, false);
}

wxApp* PluginManager::GetTheApp()
{
	return wxTheApp;
}

void PluginManager::ReloadWorkspace()
{
	wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, XRCID("reload_workspace"));
	Frame::Get()->GetEventHandler()->AddPendingEvent( evt );
}

IPlugin* PluginManager::GetPlugin(const wxString& pluginName)
{
	std::map<wxString, IPlugin*>::iterator iter = m_plugins.find(pluginName);
	if (iter != m_plugins.end()) {
		return iter->second;
	}
	return NULL;
}

wxEvtHandler *PluginManager::GetOutputWindow()
{
	return Frame::Get()->GetOutputPane()->GetOutputWindow();
}

bool PluginManager::SaveAll()
{
	return Frame::Get()->GetMainBook()->SaveAll(true, false);
}

wxString PluginManager::GetInstallDirectory() const
{
	return ManagerST::Get()->GetPluginsDir(); 
}

wxString PluginManager::GetConfigDir() const
{
	return ManagerST::Get()->GetConfigDir(); 
}
IKeyboard* PluginManager::GetKeyboardManager()
{
	return &m_keyboardMgr;
}

bool PluginManager::CreateVirtualDirectory(const wxString& parentPath, const wxString& vdName)
{
	return Frame::Get()->GetWorkspaceTab()->GetFileView()->CreateVirtualDirectory(parentPath, vdName);
}

OptionsConfigPtr PluginManager::GetEditorSettings()
{
	// First try to use LEditor::GetOptions, as it takes account of local preferences
	LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
	if (editor) {
		return editor->GetOptions();
	}
	// Failing that...
	return EditorConfigST::Get()->GetOptions();
}

void PluginManager::FindAndSelect(const wxString& pattern, const wxString& name)
{
	LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
	if (editor) {
		editor->FindAndSelect(pattern, name);
		editor->SetActive();
	}
}

TagEntryPtr PluginManager::GetTagAtCaret(bool scoped, bool impl)
{
	LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
	if (!editor)
		return NULL;
	return editor->GetContext()->GetTagAtCaret(scoped, impl);
}

bool PluginManager::AllowToolbar()
{
	long v;
	if (EditorConfigST::Get()->GetLongValue(wxT("UseSingleToolbar"), v)) {
		return v ? false : true;
	} else {
		// entry does not exist
#ifdef __WXMAC__
		return false;
#else
		return true;
#endif
	}
}

void PluginManager::EnableToolbars()
{
	// In case, plugins are now allowed to insert toolbars, disable the toolbars_menu item
	if (AllowToolbar() == false) {
		int ii = Frame::Get()->GetMenuBar()->FindMenu( wxT( "View" ) );
		if ( ii != wxNOT_FOUND ) {
			wxMenu *viewMenu = Frame::Get()->GetMenuBar()->GetMenu( ii );
			wxMenuItem *item = viewMenu->FindItem( XRCID("toolbars_menu") );
			if (item) {
				item->Enable(false);
			}
		}
	}
}

void PluginManager::SetStatusMessage(const wxString &msg, int col, int id)
{
	Frame::Get()->SetStatusMessage(msg, col, id);
}

void PluginManager::ProcessCommandQueue()
{
	ManagerST::Get()->ProcessCommandQueue();
}

void PluginManager::PushQueueCommand(const QueueCommand& cmd)
{
	ManagerST::Get()->PushQueueCommand(cmd);
}

void PluginManager::StopAndClearQueue()
{
	ManagerST::Get()->StopBuild();
}

bool PluginManager::IsBuildInProgress() const
{
	return ManagerST::Get()->IsBuildInProgress();
}

bool PluginManager::IsBuildEndedSuccessfully() const
{
	return ManagerST::Get()->IsBuildEndedSuccessfully();
}

wxString PluginManager::GetProjectNameByFile(const wxString& fullPathFileName)
{
	return ManagerST::Get()->GetProjectNameByFile(fullPathFileName);
}

BuildManager* PluginManager::GetBuildManager()
{
	return BuildManagerST::Get();
}

BuildSettingsConfig* PluginManager::GetBuildSettingsConfigManager()
{
	return BuildSettingsConfigST::Get();
}

bool PluginManager::ClosePage(const wxString &text)
{
	return Frame::Get()->GetMainBook()->ClosePage(text);
}

wxWindow *PluginManager::FindPage(const wxString &text)
{
	return Frame::Get()->GetMainBook()->FindPage(text);
}

bool PluginManager::AddPage(wxWindow *win, const wxString &text, const wxBitmap &bmp, bool selected)
{
	return Frame::Get()->GetMainBook()->AddPage(win, text, bmp, selected);
}

bool PluginManager::SelectPage(wxWindow *win)
{
	return Frame::Get()->GetMainBook()->SelectPage(win);
}

bool PluginManager::OpenFile(const BrowseRecord& rec)
{
	return Frame::Get()->GetMainBook()->OpenFile(rec);
}

NavMgr* PluginManager::GetNavigationMgr()
{
	return NavMgr::Get();
}

void PluginManager::HookProjectSettingsTab(wxNotebook* book, const wxString &projectName, const wxString &configName)
{
	std::map<wxString, IPlugin*>::iterator iter = m_plugins.begin();
	for ( ; iter != m_plugins.end(); iter++ ) {
		iter->second->HookProjectSettingsTab( book, projectName, configName );
	}
}

void PluginManager::UnHookProjectSettingsTab(wxNotebook* book, const wxString &projectName, const wxString &configName)
{
	std::map<wxString, IPlugin*>::iterator iter = m_plugins.begin();
	for ( ; iter != m_plugins.end(); iter++ ) {
		iter->second->UnHookProjectSettingsTab( book, projectName, configName );
	}
}

IEditor* PluginManager::NewEditor()
{
	return Frame::Get()->GetMainBook()->NewEditor();
}

IMacroManager* PluginManager::GetMacrosManager()
{
	return MacroManager::Instance();

}
bool PluginManager::IsShutdownInProgress() const
{
	return ManagerST::Get()->IsShutdownInProgress();
}
