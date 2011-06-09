/**
  \file gizmos.cpp

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#include "gizmos.h"
#include <wx/app.h>
#include <wx/menu.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include "workspace.h"
#include "ctags_manager.h"
#include "newplugindata.h"
#include "entry.h"
#include <wx/xrc/xmlres.h>
#include "pluginwizard.h"
#include "globals.h"
#include "dirsaver.h"
#include "workspace.h"
#include "wx/ffile.h"
#include "newclassdlg.h"
#include "newwxprojectdlg.h"
#include <algorithm>
#include "elconfig.h"


static wxString MI_NEW_WX_PROJECT      = wxT("Create new wxWidgets project...");
static wxString MI_NEW_EMBEDDEDLITE_PLUGIN = wxT("Create new EmbeddedLite plugin...");
static wxString MI_NEW_NEW_CLASS       = wxT("Create new C++ class...");

enum {
	ID_MI_NEW_WX_PROJECT = 9000,
	ID_MI_NEW_EMBEDDEDLITE_PLUGIN,
	ID_MI_NEW_NEW_CLASS
};

static GizmosPlugin* theGismos = NULL;

//Define the plugin entry point
extern "C" EXPORT IPlugin *CreatePlugin(IManager *manager)
{
	if (theGismos == 0) {
		theGismos = new GizmosPlugin(manager);
	}
	return theGismos;
}

extern "C" EXPORT PluginInfo GetPluginInfo()
{
	PluginInfo info;
	info.SetAuthor(wxT("Eran Ifrah"));
	info.SetName(wxT("Gizmos"));
	info.SetDescription(wxT("Gizmos Plugin - a collection of useful wizards for C++:\nnew Class Wizard, new wxWidgets Wizard, new Plugin Wizard"));
	info.SetVersion(wxT("v1.0"));
	return info;
}

extern "C" EXPORT int GetPluginInterfaceVersion()
{
	return PLUGIN_INTERFACE_VERSION;
}

/// Ascending sorting function
struct ascendingSortOp {
	bool operator()(const TagEntryPtr &rStart, const TagEntryPtr &rEnd) {
		return rEnd->GetName().Cmp(rStart->GetName()) > 0;
	}
};

//-------------------------------------
// helper methods
//-------------------------------------
static void ExpandVariables(wxString &content, const NewWxProjectInfo &info)
{
	content.Replace(wxT("$(ProjectName)"), info.GetName());
	wxString projname = info.GetName();
	projname.MakeLower();

	wxString appfilename = projname + wxT("_app");
	wxString framefilename = projname + wxT("_frame");

	content.Replace(wxT("$(MainFile)"),      projname);
	content.Replace(wxT("$(AppFile)"),       appfilename);
	content.Replace(wxT("$(MainFrameFile)"), framefilename);
	content.Replace(wxT("$(Unicode)"),       info.GetFlags() & wxWidgetsUnicode ? wxT("yes") : wxT("no"));
	content.Replace(wxT("$(Static)"),        info.GetFlags() & wxWidgetsStatic ? wxT("yes") : wxT("no"));
	content.Replace(wxT("$(Universal)"),     info.GetFlags() & wxWidgetsUniversal ? wxT("yes") : wxT("no"));
	content.Replace(wxT("$(WinResFlag)"),    info.GetFlags() & wxWidgetsWinRes ? wxT("yes") : wxT("no"));
	content.Replace(wxT("$(MWindowsFlag)"),  info.GetFlags() & wxWidgetsSetMWindows ? wxT("-mwindows") : wxEmptyString);
	content.Replace(wxT("$(PCHFlag)"),       info.GetFlags() & wxWidgetsPCH ? wxT("WX_PRECOMP") : wxEmptyString);
	content.Replace(wxT("$(PCHCmpOptions)"), info.GetFlags() & wxWidgetsPCH ? wxT("-Winvalid-pch;-include wx_pch.h") : wxEmptyString);
	content.Replace(wxT("$(PCHFileName)"),   info.GetFlags() & wxWidgetsPCH ? wxT("wx_pch.h") : wxEmptyString);

	if( info.GetFlags() & wxWidgetsWinRes ) content.Replace(wxT("$(WinResFile)"), wxT("<File Name=\"resources.rc\" />") );
	if( info.GetFlags() & wxWidgetsPCH )content.Replace(wxT("$(PCHFile)"), wxT("<File Name=\"wx_pch.h\" />") );

	wxString othersettings;
	if( info.GetVersion() != wxT("Default") ) othersettings << wxT("--version=") << info.GetVersion();
	if( !info.GetPrefix().IsEmpty() ) othersettings << wxT(" --prefix=") << info.GetPrefix();
	content.Replace(wxT("$(Other)"), othersettings);

	//create the application class name
	wxString initial = appfilename.Mid(0, 1);
	initial.MakeUpper();
	appfilename.SetChar(0, initial.GetChar(0));

	//create the main frame class name
	wxString framename(projname);
	wxString appname(projname);

	framename << wxT("Frame");
	appname << wxT("App");

	initial = framename.Mid(0, 1);
	initial.MakeUpper();
	framename.SetChar(0, initial.GetChar(0));

	initial = appname.Mid(0, 1);
	initial.MakeUpper();
	appname.SetChar(0, initial.GetChar(0));

	content.Replace(wxT("$(AppName)"), appname);
	content.Replace(wxT("$(MainFrameName)"), framename);
}

static void WriteFile(const wxString &fileName, const wxString &content)
{
	wxFFile file;
	if (!file.Open(fileName, wxT("w+b"))) {
		return;
	}

	file.Write(content);
	file.Close();
}

static void WriteNamespacesDeclaration(const wxArrayString& namespacesList, wxString& buffer)
{
	for (int i = 0; i < namespacesList.Count(); i++)
	{
		buffer << wxT("namespace ") << namespacesList[i] << wxT("\n{\n\n");
	}
}

GizmosPlugin::GizmosPlugin(IManager *manager)
		: IPlugin(manager)
{
	m_longName = wxT("Gizmos Plugin - a collection of useful utils for C++");
	m_shortName = wxT("Gizmos");
}

GizmosPlugin::~GizmosPlugin()
{
}

clToolBar *GizmosPlugin::CreateToolBar(wxWindow *parent)
{
	clToolBar *tb(NULL);
	if (m_mgr->AllowToolbar()) {
		//support both toolbars icon size
		int size = m_mgr->GetToolbarIconSize();


		tb = new clToolBar(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, clTB_DEFAULT_STYLE);
		tb->SetToolBitmapSize(wxSize(size, size));

		if (size == 24) {
			tb->AddTool(XRCID("gizmos_options"), wxT("Gizmos..."), wxXmlResource::Get()->LoadBitmap(wxT("plugin24")), wxT("Open Gizmos quick menu"));
		} else {
			tb->AddTool(XRCID("gizmos_options"), wxT("Gizmos..."), wxXmlResource::Get()->LoadBitmap(wxT("plugin16")), wxT("Open Gizmos quick menu"));
		}

		// When using AUI, make this toolitem a dropdown button
#if USE_AUI_TOOLBAR
		tb->SetToolDropDown(XRCID("gizmos_options"), true);
		m_mgr->GetTheApp()->Connect(XRCID("gizmos_options"), wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN, wxAuiToolBarEventHandler(GizmosPlugin::OnGizmosAUI), NULL, (wxEvtHandler*)this);
#endif
		tb->Realize();
	}

	//Connect the events to us
#if !USE_AUI_TOOLBAR
	m_mgr->GetTheApp()->Connect(XRCID("gizmos_options"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GizmosPlugin::OnGizmos   ), NULL, (wxEvtHandler*)this);
#endif
	m_mgr->GetTheApp()->Connect(XRCID("gizmos_options"), wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(GizmosPlugin::OnGizmosUI), NULL, (wxEvtHandler*)this);

	m_mgr->GetTheApp()->Connect(ID_MI_NEW_EMBEDDEDLITE_PLUGIN, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GizmosPlugin::OnNewPlugin), NULL, (wxEvtHandler*)this);
	m_mgr->GetTheApp()->Connect(ID_MI_NEW_EMBEDDEDLITE_PLUGIN, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(GizmosPlugin::OnNewPluginUI), NULL, (wxEvtHandler*)this);


	m_mgr->GetTheApp()->Connect(ID_MI_NEW_NEW_CLASS, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GizmosPlugin::OnNewClass), NULL, (wxEvtHandler*)this);
	m_mgr->GetTheApp()->Connect(ID_MI_NEW_NEW_CLASS, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(GizmosPlugin::OnNewClassUI), NULL, (wxEvtHandler*)this);

	m_mgr->GetTheApp()->Connect(ID_MI_NEW_WX_PROJECT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GizmosPlugin::OnNewWxProject), NULL, (wxEvtHandler*)this);
	m_mgr->GetTheApp()->Connect(ID_MI_NEW_WX_PROJECT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(GizmosPlugin::OnNewWxProjectUI), NULL, (wxEvtHandler*)this);
	return tb;
}

void GizmosPlugin::CreatePluginMenu(wxMenu *pluginsMenu)
{
	wxMenu *menu = new wxMenu();
	wxMenuItem *item(NULL);
	item = new wxMenuItem(menu, ID_MI_NEW_EMBEDDEDLITE_PLUGIN, _("New EmbeddedLite Plugin Wizard..."), wxEmptyString, wxITEM_NORMAL);
	menu->Append(item);
	item = new wxMenuItem(menu, ID_MI_NEW_NEW_CLASS, _("New Class Wizard..."), wxEmptyString, wxITEM_NORMAL);
	menu->Append(item);
	item = new wxMenuItem(menu, ID_MI_NEW_WX_PROJECT, _("New wxWidgets Project Wizard..."), wxEmptyString, wxITEM_NORMAL);
	menu->Append(item);
	pluginsMenu->Append(wxID_ANY, _("Gizmos"), menu);
}

void GizmosPlugin::HookPopupMenu(wxMenu *menu, MenuType type)
{
	if (type == MenuTypeFileView_Folder) {
		//Create the popup menu for the virtual folders
		wxMenuItem *item(NULL);

		item = new wxMenuItem(menu, wxID_SEPARATOR);
		menu->Prepend(item);
		m_vdDynItems.push_back(item);

		item = new wxMenuItem(menu, ID_MI_NEW_NEW_CLASS, wxT("&New Class..."), wxEmptyString, wxITEM_NORMAL);
		menu->Prepend(item);
		m_vdDynItems.push_back(item);
	}
}

void GizmosPlugin::UnHookPopupMenu(wxMenu *menu, MenuType type)
{
	if (type == MenuTypeFileView_Folder) {
		std::vector<wxMenuItem*>::iterator iter = m_vdDynItems.begin();
		for (; iter != m_vdDynItems.end(); iter++) {
			menu->Destroy(*iter);
		}
		m_vdDynItems.clear();
	}
}

void GizmosPlugin::UnPlug()
{
	m_mgr->GetTheApp()->Disconnect(XRCID("gizmos_options"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(GizmosPlugin::OnGizmos   ), NULL, (wxEvtHandler*)this);
	m_mgr->GetTheApp()->Disconnect(XRCID("gizmos_options"), wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(GizmosPlugin::OnGizmosUI), NULL, (wxEvtHandler*)this);
}

void GizmosPlugin::OnNewPlugin(wxCommandEvent &e)
{
	wxUnusedVar(e);
	DoCreateNewPlugin();
}

void GizmosPlugin::DoCreateNewPlugin()
{
	//Load the wizard
	PluginWizard *wiz = new PluginWizard(NULL, wxID_ANY);
	NewPluginData data;
	if (wiz->Run(data)) {
		//load the template file and replace all variables with the
		//actual values provided by user
		wxString filename(m_mgr->GetStartupDirectory() + wxT("/templates/gizmos/liteeditor-plugin.project.wizard"));
		wxString content;
		if (!ReadFileWithConversion(filename, content)) {
			return;
		}
		//convert the paths provided by user to relative paths
		DirSaver ds;
		wxSetWorkingDirectory(data.GetProjectPath());
		wxFileName fn(data.GetCodelitePath());

		if (!fn.MakeRelativeTo(wxGetCwd())) {
			wxLogMessage(wxT("Warning: Failed to convert paths to relative path."));
		}

#ifdef __WXMSW__
		wxString dllExt(wxT("dll"));
#else
		wxString dllExt(wxT("so"));
#endif
		wxString clpath = fn.GetFullPath();

		content.Replace(wxT("$(EmbeddedLitePath)"), clpath);
		content.Replace(wxT("$(DllExt)"), dllExt);
		content.Replace(wxT("$(PluginName)"), data.GetPluginName());
		wxString baseFileName = data.GetPluginName();
		baseFileName.MakeLower();
		content.Replace(wxT("$(BaseFileName)"), baseFileName);
		content.Replace(wxT("$(ProjectName)"), data.GetPluginName());

		//save the file to the disk
		wxString projectFileName;
		projectFileName << data.GetProjectPath() << wxT("/") << data.GetPluginName() << wxT(".") << EL_PROJECT_EXT;
		wxFFile file;
		if (!file.Open(projectFileName, wxT("w+b"))) {
			return;
		}

		file.Write(content);
		file.Close();

		//Create the plugin source and header files
		wxString srcFile(baseFileName + wxT(".cpp"));
		wxString headerFile(baseFileName + wxT(".h"));

		//---------------------------------------------------------------
		//write the content of the file based on the file template
		//---------------------------------------------------------------

		//Generate the source files
		filename = m_mgr->GetStartupDirectory() + wxT("/templates/gizmos/plugin.cpp.wizard");
		content.Clear();
		if (!ReadFileWithConversion(filename, content)) {
			wxMessageBox(_("Failed to load wizard's file 'plugin.cpp.wizard'"), wxT("EmbeddedLite"), wxICON_WARNING | wxOK);
			return;
		}

		// Expand macros
		content.Replace(wxT("$(PluginName)"), data.GetPluginName());
		content.Replace(wxT("$(BaseFileName)"), baseFileName);
		content.Replace(wxT("$(PluginShortName)"), data.GetPluginName());
		content.Replace(wxT("$(PluginLongName)"), data.GetPluginDescription());
		content.Replace(wxT("$(UserName)"), wxGetUserName().c_str());

		file.Open(srcFile, wxT("w+b"));
		file.Write(content);
		file.Close();

		//create the header file
		filename = m_mgr->GetStartupDirectory() + wxT("/templates/gizmos/plugin.h.wizard");
		content.Clear();
		if (!ReadFileWithConversion(filename, content)) {
			wxMessageBox(_("Failed to load wizard's file 'plugin.h.wizard'"), wxT("EmbeddedLite"), wxICON_WARNING | wxOK);
			return;
		}

		// Expand macros
		content.Replace(wxT("$(PluginName)"), data.GetPluginName());
		content.Replace(wxT("$(BaseFileName)"), baseFileName);
		content.Replace(wxT("$(PluginShortName)"), data.GetPluginName());
		content.Replace(wxT("$(PluginLongName)"), data.GetPluginDescription());
		content.Replace(wxT("$(UserName)"), wxGetUserName().c_str());

		file.Open(headerFile, wxT("w+b"));
		file.Write(content);
		file.Close();

		//add the new project to the workspace
		wxString errMsg;

		//convert the path to be full path as required by the
		//workspace manager
		m_mgr->AddProject(projectFileName);
	}
	wiz->Destroy();
}

void GizmosPlugin::OnNewPluginUI(wxUpdateUIEvent &e)
{
	CHECK_CL_SHUTDOWN();
	//we enable the button only when workspace is opened
	e.Enable(m_mgr->IsWorkspaceOpen());
}

void GizmosPlugin::OnNewClassUI(wxUpdateUIEvent &e)
{
	CHECK_CL_SHUTDOWN();
	//we enable the button only when workspace is opened
	e.Enable(m_mgr->IsWorkspaceOpen());
}

void GizmosPlugin::OnNewClass(wxCommandEvent &e)
{
	wxUnusedVar(e);
	DoCreateNewClass();
}

void GizmosPlugin::DoCreateNewClass()
{
	NewClassDlg *dlg = new NewClassDlg(NULL, m_mgr);
	if (dlg->ShowModal() == wxID_OK) {
		//do something with the information here
		NewClassInfo info;
		dlg->GetNewClassInfo(info);

		CreateClass(info);
	}
	dlg->Destroy();
}

void GizmosPlugin::CreateClass(const NewClassInfo &info)
{
	wxString macro(info.blockGuard);
	if( macro.IsEmpty() ) {
		// use the name instead
		macro = info.name;
		macro.MakeUpper();
		macro << wxT("_H");
	}

	wxString srcFile;
	srcFile << info.path << wxFileName::GetPathSeparator() << info.fileName << wxT(".cpp");

	wxString hdrFile;
	hdrFile << info.path << wxFileName::GetPathSeparator() << info.fileName << wxT(".h");

	//create cpp + h file
	wxString cpp;
	wxString header;

	//----------------------------------------------------
	// header file
	//----------------------------------------------------
	header << wxT("#ifndef ") << macro << wxT("\n");
	header << wxT("#define ") << macro << wxT("\n");
	header << wxT("\n");

	wxString closeMethod;
	if (info.isInline)
		closeMethod = wxT("\n\t{\n\t}\n");
	else
		closeMethod = wxT(";\n");

	// Add include for base classes
	if (info.parents.empty() == false) {
		for (size_t i=0; i< info.parents.size(); i++) {

			ClassParentInfo pi = info.parents.at(i);

			// Include the header name only (no paths)
			wxFileName includeFileName(pi.fileName);
			header << wxT("#include \"") << includeFileName.GetFullName() << wxT("\" // Base class: ") << pi.name << wxT("\n");
		}
		header << wxT("\n");
	}

	// Open namespace
	if (!info.namespacesList.IsEmpty()) {
		WriteNamespacesDeclaration (info.namespacesList, header);
	}

	header << wxT("class ") << info.name;

	if (info.parents.empty() == false) {
		header << wxT(" : ");
		for (size_t i=0; i< info.parents.size(); i++) {
			ClassParentInfo pi = info.parents.at(i);
			header << pi.access << wxT(" ") << pi.name << wxT(", ");
		}
		header = header.BeforeLast(wxT(','));
	}
	header << wxT(" {\n\n");

	if (info.isSingleton) {
		header << wxT("\tstatic ") << info.name << wxT("* ms_instance;\n\n");
	}

	if (info.isAssingable == false) {
		//declare copy constructor & assingment operator as private
		header << wxT("private:\n");
		header << wxT("\t") << info.name << wxT("(const ") << info.name << wxT("& rhs)") << closeMethod;
		header << wxT("\t") << info.name << wxT("& operator=(const ") << info.name << wxT("& rhs)") << closeMethod;
		header << wxT("\n");
	}

	if (info.isSingleton) {
		header << wxT("public:\n");
		header << wxT("\tstatic ") << info.name << wxT("* Instance();\n");
		header << wxT("\tstatic void Release();\n\n");

		header << wxT("private:\n");
		header << wxT("\t") << info.name << wxT("();\n");

		if (info.isVirtualDtor) {
			header << wxT("\tvirtual ~") << info.name << wxT("();\n\n");
		} else {
			header << wxT("\t~") << info.name << wxT("();\n\n");
		}
	} else {
		header << wxT("public:\n");
		header << wxT("\t") << info.name << wxT("()") << closeMethod;
		if (info.isVirtualDtor) {
			header << wxT("\tvirtual ~") << info.name << wxT("()") << closeMethod << wxT("\n");
		} else {
			header << wxT("\t~") << info.name << wxT("()") << closeMethod << wxT("\n");
		}

	}

	//add virtual function declaration
	wxString v_decl = DoGetVirtualFuncDecl(info);
	if (v_decl.IsEmpty() == false) {
		header << wxT("public:\n");
		header << v_decl;
	}

	header << wxT("};\n\n");

	// Close namespaces
	for (int i = 0; i < info.namespacesList.Count(); i++)
	{
		header << wxT("}\n\n");
	}

	header << wxT("#endif // ") << macro << wxT("\n");

	wxFFile file;

	file.Open(hdrFile, wxT("w+b"));
	file.Write(header);
	file.Close();

	//if we have a selected virtual folder, add the files to it
	wxArrayString paths;
	paths.Add(hdrFile);

	//----------------------------------------------------
	// source file
	//----------------------------------------------------
	if (!info.isInline)
	{
		cpp << wxT("#include \"") << info.fileName << wxT(".h\"\n\n");

		// Open namespace
		if (!info.namespacesList.IsEmpty()) {
			WriteNamespacesDeclaration (info.namespacesList, cpp);
		}

		if (info.isSingleton) {
			cpp << info.name << wxT("* ") << info.name << wxT("::ms_instance = 0;\n\n");
		}
		//ctor/dtor
		cpp << info.name << wxT("::") << info.name << wxT("()\n");
		cpp << wxT("{\n}\n\n");
		cpp << info.name << wxT("::~") << info.name << wxT("()\n");
		cpp << wxT("{\n}\n\n");
		if (info.isSingleton) {
			cpp << info.name << wxT("* ") << info.name << wxT("::Instance()\n");
			cpp << wxT("{\n");
			cpp << wxT("\tif(ms_instance == 0){\n");
			cpp << wxT("\t\tms_instance = new ") << info.name << wxT("();\n");
			cpp << wxT("\t}\n");
			cpp << wxT("\treturn ms_instance;\n");
			cpp << wxT("}\n\n");

			cpp << wxT("void ") << info.name << wxT("::Release()\n");
			cpp << wxT("{\n");
			cpp << wxT("\tif(ms_instance){\n");
			cpp << wxT("\t\tdelete ms_instance;\n");
			cpp << wxT("\t}\n");
			cpp << wxT("\tms_instance = 0;\n");
			cpp << wxT("}\n\n");
		}

		cpp << DoGetVirtualFuncImpl(info);

		// Close namespaces
		for (int i = 0; i < info.namespacesList.Count(); i++)
		{
			cpp << wxT("}\n\n");
		}

		file.Open(srcFile, wxT("w+b"));
		file.Write(cpp);
		file.Close();

		paths.Add(srcFile);
	}

	// We have a .cpp and an .h file, and there may well be a :src and an :include folder available
	// So try to place the files appropriately. If that fails, dump both in the selected folder
	if (! m_mgr->AddFilesToGroupFolderIntelligently(info.virtualDirectory, paths) )
		m_mgr->AddFilesToGroupFolder(info.virtualDirectory, paths);
}

void GizmosPlugin::OnNewWxProject(wxCommandEvent &e)
{
	wxUnusedVar(e);
	DoCreateNewWxProject();
}

void GizmosPlugin::DoCreateNewWxProject()
{
	NewWxProjectDlg *dlg = new NewWxProjectDlg(NULL, m_mgr);
	if (dlg->ShowModal() == wxID_OK) {
		//Create the project
		NewWxProjectInfo info;
		dlg->GetProjectInfo(info);
		CreateWxProject(info);
	}
	dlg->Destroy();
}

void GizmosPlugin::OnNewWxProjectUI(wxUpdateUIEvent &e)
{
	CHECK_CL_SHUTDOWN();
	//we enable the button only when workspace is opened
	e.Enable(m_mgr->IsWorkspaceOpen());
}

void GizmosPlugin::CreateWxProject(NewWxProjectInfo &info)
{
	//TODO:: Implement this ...
	wxString basedir = m_mgr->GetStartupDirectory();

	//we first create the project files
	if (info.GetType() == wxProjectTypeGUI) {

		//we are creating a project of type GUI
		wxString projectConent;
		wxString mainFrameCppContent;
		wxString mainFrameHContent;
		wxString appCppConent;
		wxString apphConent;
		wxString rcContent;
		wxString pchContent;

		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/wxproject.project.wizard"), projectConent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/mainframe.cpp.wizard"), mainFrameCppContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/mainframe.h.wizard"), mainFrameHContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/app.h.wizard"), apphConent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/app.cpp.wizard"), appCppConent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/resources.rc.wizard"), rcContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/wx_pch.h.wizard"), pchContent)) {
			return;
		}

		ExpandVariables(projectConent, info);
		ExpandVariables(mainFrameCppContent, info);
		ExpandVariables(mainFrameHContent, info);
		ExpandVariables(apphConent, info);
		ExpandVariables(appCppConent, info);

		//Write the files content into the project directory
		DirSaver ds;
		wxSetWorkingDirectory(info.GetPath());

		wxString projname = info.GetName();
		projname.MakeLower();

		wxString appfilename = projname + wxT("_app");
		wxString framefilename = projname + wxT("_frame");

		WriteFile(framefilename + wxT(".cpp"), mainFrameCppContent);
		WriteFile(framefilename + wxT(".h"), mainFrameHContent);
		WriteFile(appfilename + wxT(".h"), apphConent);
		WriteFile(appfilename+ wxT(".cpp"), appCppConent);
		if( info.GetFlags() & wxWidgetsWinRes ) WriteFile(wxT("resources.rc"), rcContent);
		if( info.GetFlags() & wxWidgetsPCH ) WriteFile(wxT("wx_pch.h"), pchContent);
		WriteFile(info.GetName() + wxT(".") + EL_PROJECT_EXT, projectConent);

		//If every this is OK, add the project as well
		m_mgr->AddProject(info.GetName() + wxT(".") + EL_PROJECT_EXT);

	} else if (info.GetType() == wxProjectTypeGUIFBDialog) {

		//we are creating a project of type GUI (dialog generated by wxFormBuilder)

		wxString projectContent;
		wxString mainFrameCppContent;
		wxString mainFrameHContent;
		wxString appCppContent;
		wxString apphContent;
		wxString fbContent;
		wxString rcContent;
		wxString pchContent;

		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/wxproject-fb.project.wizard"), projectContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/gui-dialog.cpp.wizard"), mainFrameCppContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/gui-dialog.h.wizard"), mainFrameHContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/main-dialog.h.wizard"), apphContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/main-dialog.cpp.wizard"), appCppContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/gui-dialog.fbp.wizard"), fbContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/resources.rc.wizard"), rcContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/wx_pch.h.wizard"), pchContent)) {
			return;
		}

		ExpandVariables(projectContent, info);
		ExpandVariables(mainFrameCppContent, info);
		ExpandVariables(mainFrameHContent, info);
		ExpandVariables(apphContent, info);
		ExpandVariables(appCppContent, info);

		//Write the files content into the project directory
		DirSaver ds;
		wxSetWorkingDirectory(info.GetPath());

		wxString projname = info.GetName();
		projname.MakeLower();

		wxString appfilename = projname + wxT("_app");

		WriteFile(wxT("gui.cpp"), mainFrameCppContent);
		WriteFile(wxT("gui.h"), mainFrameHContent);
		WriteFile(appfilename + wxT(".h"), apphContent);
		WriteFile(appfilename+ wxT(".cpp"), appCppContent);
		WriteFile(wxT("gui.fbp"), fbContent);
		if( info.GetFlags() & wxWidgetsWinRes ) WriteFile(wxT("resources.rc"), rcContent);
		if( info.GetFlags() & wxWidgetsPCH ) WriteFile(wxT("wx_pch.h"), pchContent);
		WriteFile(info.GetName() + wxT(".") + EL_PROJECT_EXT, projectContent);

		//If every this is OK, add the project as well
		m_mgr->AddProject(info.GetName() + wxT(".") + EL_PROJECT_EXT);

	} else if (info.GetType() == wxProjectTypeGUIFBFrame) {

		//we are creating a project of type GUI (dialog generated by wxFormBuilder)

		wxString projectContent;
		wxString mainFrameCppContent;
		wxString mainFrameHContent;
		wxString appCppContent;
		wxString apphContent;
		wxString fbContent;
		wxString pchContent;
		wxString rcContent;

		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/wxproject-fb.project.wizard"), projectContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/gui-frame.cpp.wizard"), mainFrameCppContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/gui-frame.h.wizard"), mainFrameHContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/main-frame.h.wizard"), apphContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/main-frame.cpp.wizard"), appCppContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/gui-frame.fbp.wizard"), fbContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/resources.rc.wizard"), rcContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/wx_pch.h.wizard"), pchContent)) {
			return;
		}

		ExpandVariables(projectContent, info);
		ExpandVariables(mainFrameCppContent, info);
		ExpandVariables(mainFrameHContent, info);
		ExpandVariables(apphContent, info);
		ExpandVariables(appCppContent, info);

		//Write the files content into the project directory
		DirSaver ds;
		wxSetWorkingDirectory(info.GetPath());

		wxString projname = info.GetName();
		projname.MakeLower();

		wxString appfilename = projname + wxT("_app");

		WriteFile(wxT("gui.cpp"), mainFrameCppContent);
		WriteFile(wxT("gui.h"), mainFrameHContent);
		WriteFile(appfilename + wxT(".h"), apphContent);
		WriteFile(appfilename+ wxT(".cpp"), appCppContent);
		WriteFile(wxT("gui.fbp"), fbContent);
		if( info.GetFlags() & wxWidgetsWinRes ) WriteFile(wxT("resources.rc"), rcContent);
		if( info.GetFlags() & wxWidgetsPCH ) WriteFile(wxT("wx_pch.h"), pchContent);
		WriteFile(info.GetName() + wxT(".") + EL_PROJECT_EXT, projectContent);

		//If every this is OK, add the project as well
		m_mgr->AddProject(info.GetName() + wxT(".") + EL_PROJECT_EXT);

	} else if (info.GetType() == wxProjectTypeSimpleMain) {

		//we are creating a project of type console app

		wxString projectConent;
		wxString appCppConent;
		wxString pchContent;
		wxString rcContent;

		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/wxmain.project.wizard"), projectConent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/main.cpp.wizard"), appCppConent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/resources.rc.wizard"), rcContent)) {
			return;
		}
		if (!ReadFileWithConversion(basedir + wxT("/templates/gizmos/wx_pch.h.wizard"), pchContent)) {
			return;
		}

		ExpandVariables(projectConent, info);
		ExpandVariables(appCppConent, info);

		//Write the files content into the project directory
		DirSaver ds;
		wxSetWorkingDirectory(info.GetPath());

		wxString projname = info.GetName();
		projname.MakeLower();

		wxString appfilename = projname;
		WriteFile(appfilename+ wxT(".cpp"), appCppConent);
		if( info.GetFlags() & wxWidgetsWinRes ) WriteFile(wxT("resources.rc"), rcContent);
		if( info.GetFlags() & wxWidgetsPCH ) WriteFile(wxT("wx_pch.h"), pchContent);
		WriteFile(info.GetName() + wxT(".") + EL_PROJECT_EXT, projectConent);

		//If every this is OK, add the project as well
		m_mgr->AddProject(info.GetName() + wxT(".") + EL_PROJECT_EXT);
	}
}

wxString GizmosPlugin::DoGetVirtualFuncImpl(const NewClassInfo &info)
{
	if (info.implAllVirtual == false && info.implAllPureVirtual == false)
		return wxEmptyString;

	//get list of all parent virtual functions
	std::vector< TagEntryPtr > tmp_tags;
	std::vector< TagEntryPtr > no_dup_tags;
	std::vector< TagEntryPtr > tags;
	for (std::vector< TagEntryPtr >::size_type i=0; i< info.parents.size(); i++) {
		ClassParentInfo pi = info.parents.at(i);

		// Load all prototypes / functions of the parent scope
		m_mgr->GetTagsManager()->TagsByScope(pi.name, wxT("prototype"), tmp_tags, false);
		m_mgr->GetTagsManager()->TagsByScope(pi.name, wxT("function"),  tmp_tags, false);
	}

	// and finally sort the results
	std::sort(tmp_tags.begin(), tmp_tags.end(), ascendingSortOp());
	GizmosRemoveDuplicates(tmp_tags, no_dup_tags);

	//filter out all non virtual functions
	for (std::vector< TagEntryPtr >::size_type i=0; i< no_dup_tags.size(); i++) {
		TagEntryPtr tt = no_dup_tags.at(i);
		bool collect(false);
		if (info.implAllVirtual) {
			collect = m_mgr->GetTagsManager()->IsVirtual(tt);
		} else if (info.implAllPureVirtual) {
			collect = m_mgr->GetTagsManager()->IsPureVirtual(tt);
		}

		if (collect) {
			tags.push_back(tt);
		}
	}

	wxString impl;
	for (std::vector< TagEntryPtr >::size_type i=0; i< tags.size(); i++) {
		TagEntryPtr tt = tags.at(i);
		impl << m_mgr->GetTagsManager()->FormatFunction(tt, FunctionFormat_Impl, info.name);
	}
	return impl;
}

wxString GizmosPlugin::DoGetVirtualFuncDecl(const NewClassInfo &info)
{
	if (info.implAllVirtual == false && info.implAllPureVirtual == false)
		return wxEmptyString;

	//get list of all parent virtual functions
	std::vector< TagEntryPtr > tmp_tags;
	std::vector< TagEntryPtr > no_dup_tags;
	std::vector< TagEntryPtr > tags;
	for (std::vector< TagEntryPtr >::size_type i=0; i< info.parents.size(); i++) {
		ClassParentInfo pi = info.parents.at(i);

		// Load all prototypes / functions of the parent scope
		m_mgr->GetTagsManager()->TagsByScope(pi.name, wxT("prototype"), tmp_tags, false);
		m_mgr->GetTagsManager()->TagsByScope(pi.name, wxT("function"),  tmp_tags, false);
	}

	// and finally sort the results
	std::sort(tmp_tags.begin(), tmp_tags.end(), ascendingSortOp());
	GizmosRemoveDuplicates(tmp_tags, no_dup_tags);

	//filter out all non virtual functions
	for (std::vector< TagEntryPtr >::size_type i=0; i< no_dup_tags.size(); i++) {
		TagEntryPtr tt = no_dup_tags.at(i);

		// Skip c-tors/d-tors
		if(tt->IsDestructor() || tt->IsConstructor())
			continue;

		if (info.implAllVirtual && m_mgr->GetTagsManager()->IsVirtual(tt)) {
			tags.push_back(tt);

		} else if (info.implAllPureVirtual && m_mgr->GetTagsManager()->IsPureVirtual(tt)) {
			tags.push_back(tt);
		}
	}

	wxString decl;
	for (std::vector< TagEntryPtr >::size_type i=0; i< tags.size(); i++) {
		TagEntryPtr tt = tags.at(i);
		wxString ff = m_mgr->GetTagsManager()->FormatFunction(tt);

		if (info.isInline)
			ff.Replace (wxT(";"), wxT("\n\t{\n\t}"));

		decl << wxT("\t") << ff;
	}
	return decl;
}

void GizmosPlugin::OnGizmos(wxCommandEvent& e)
{
	// open a popup menu
	wxUnusedVar(e);
	wxPoint pt;
	DoPopupButtonMenu(pt);
}

void GizmosPlugin::OnGizmosUI(wxUpdateUIEvent& e)
{
	CHECK_CL_SHUTDOWN();
	e.Enable(m_mgr->IsWorkspaceOpen());
}

void GizmosPlugin::GizmosRemoveDuplicates(std::vector<TagEntryPtr>& src, std::vector<TagEntryPtr>& target)
{
	std::map<wxString, TagEntryPtr> uniqueSet;
	for (size_t i=0; i<src.size(); i++) {

		wxString  signature = src.at(i)->GetSignature();
		wxString  key              = m_mgr->GetTagsManager()->NormalizeFunctionSig(signature, 0);
		int       hasDefaultValues = signature.Find(wxT("="));

		key.Prepend(src.at(i)->GetName());
		if ( uniqueSet.find(key) != uniqueSet.end() ) {
			// we already got an instance of this method,
			// incase we have default values in the this Tag, keep this
			// TagEntryPtr, otherwise keep the previous tag
			if(hasDefaultValues != wxNOT_FOUND) {
				uniqueSet[key] = src.at(i);
			}

		} else {
			// First time
			uniqueSet[key] = src.at(i);
		}
	}

	// copy the unique set to the output vector
	std::map<wxString, TagEntryPtr>::iterator iter = uniqueSet.begin();
	for(; iter != uniqueSet.end(); iter++) {
		target.push_back( iter->second );
	}
}

void GizmosPlugin::DoPopupButtonMenu(wxPoint pt)
{
#ifdef __WXMSW__
	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
#endif

	wxMenu popupMenu;

	std::map<wxString, int> options;
	options[MI_NEW_EMBEDDEDLITE_PLUGIN] = ID_MI_NEW_EMBEDDEDLITE_PLUGIN;
	options[MI_NEW_NEW_CLASS      ] = ID_MI_NEW_NEW_CLASS;
	options[MI_NEW_WX_PROJECT     ] = ID_MI_NEW_WX_PROJECT;

	std::map<wxString, int>::iterator iter = options.begin();
	for (; iter != options.end(); iter++) {
		int      id   = (*iter).second;
		wxString text = (*iter).first;
		wxMenuItem *item = new wxMenuItem(&popupMenu, id, text, text, wxITEM_NORMAL);
		popupMenu.Append(item);
	}
	m_mgr->GetTheApp()->GetTopWindow()->PopupMenu(&popupMenu, pt);
}

#if USE_AUI_TOOLBAR
void GizmosPlugin::OnGizmosAUI(wxAuiToolBarEvent& e)
{
    if (e.IsDropDownClicked())
    {
        wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(e.GetEventObject());
        tb->SetToolSticky(e.GetId(), true);

		// line up our menu with the button
        wxRect rect = tb->GetToolRect(e.GetId());
        wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());
        pt = m_mgr->GetTheApp()->GetTopWindow()->ScreenToClient(pt);

		DoPopupButtonMenu(pt);
		tb->SetToolSticky(e.GetId(), false);
	}
}
#endif


