//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : workspacetab.cpp
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
#include <wx/xrc/xmlres.h>
#include <wx/combobox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include "project_settings_dlg.h"
#include "globals.h"
#include "manager.h"
#include "fileview.h"
#include "editor_config.h"
#include "frame.h"
#include "macros.h"
#include "workspace_pane.h"
#include "workspacetab.h"

WorkspaceTab::WorkspaceTab(wxWindow *parent, const wxString &caption)
    : wxPanel(parent)
    , m_caption(caption)
    , m_isLinkedToEditor(true)
{
	long link(1);
	EditorConfigST::Get()->GetLongValue(wxT("LinkWorkspaceViewToEditor"), link);
//rvv	m_isLinkedToEditor = link ? true : false;

	m_isLinkedToEditor = false;     //rvv

	CreateGUIControls();
    ConnectEvents();
}

WorkspaceTab::~WorkspaceTab()
{
	Disconnect( XRCID("link_editor"),        wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnLinkEditor));
	Disconnect( XRCID("collapse_all"),       wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnCollapseAll));
	Disconnect( XRCID("collapse_all"),       wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnCollapseAllUI));
	Disconnect( XRCID("go_home"),            wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnGoHome));
	Disconnect( XRCID("go_home"),            wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnGoHomeUI));
	Disconnect( XRCID("project_properties"), wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnProjectSettingsUI));
	Disconnect( XRCID("project_properties"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnProjectSettings));
	Disconnect( XRCID("set_project_active"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnShowProjectListPopup));
	Disconnect( XRCID("set_project_active"), wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnProjectSettingsUI));

    wxTheApp->Disconnect(XRCID("show_in_workspace"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnShowFile),   NULL, this);
    wxTheApp->Disconnect(XRCID("show_in_workspace"), wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnShowFileUI), NULL, this);

    wxTheApp->Disconnect(wxEVT_WORKSPACE_LOADED,      wxCommandEventHandler(WorkspaceTab::OnWorkspaceLoaded),     NULL, this);
    wxTheApp->Disconnect(wxEVT_WORKSPACE_CLOSED,      wxCommandEventHandler(WorkspaceTab::OnWorkspaceClosed),     NULL, this);
    wxTheApp->Disconnect(wxEVT_PROJ_ADDED,            wxCommandEventHandler(WorkspaceTab::OnProjectAdded),        NULL, this);
    wxTheApp->Disconnect(wxEVT_PROJ_REMOVED,          wxCommandEventHandler(WorkspaceTab::OnProjectRemoved),      NULL, this);
    wxTheApp->Disconnect(wxEVT_ACTIVE_EDITOR_CHANGED, wxCommandEventHandler(WorkspaceTab::OnActiveEditorChanged), NULL, this);
    wxTheApp->Disconnect(wxEVT_EDITOR_CLOSING,        wxCommandEventHandler(WorkspaceTab::OnEditorClosing),       NULL, this);
}

void WorkspaceTab::CreateGUIControls()
{
	wxBoxSizer *sz = new wxBoxSizer(wxVERTICAL);
	SetSizer(sz);

	wxToolBar *tb = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER);
	tb->AddTool(XRCID("link_editor"), wxEmptyString, wxXmlResource::Get()->LoadBitmap(wxT("link_editor")), wxT("Link Editor"), wxITEM_CHECK);
	tb->ToggleTool(XRCID("link_editor"), m_isLinkedToEditor);
	tb->AddTool(XRCID("collapse_all"), wxEmptyString, wxXmlResource::Get()->LoadBitmap(wxT("collapse")), wxT("Collapse All"), wxITEM_NORMAL);
	tb->AddTool(XRCID("go_home"), wxEmptyString, wxXmlResource::Get()->LoadBitmap(wxT("gohome")), wxT("Goto Active Project"), wxITEM_NORMAL);
	tb->AddSeparator();
	tb->AddTool(XRCID("project_properties"), wxEmptyString, wxXmlResource::Get()->LoadBitmap(wxT("project_settings")), wxT("Open Active Project Settings..."), wxITEM_NORMAL);
	tb->AddTool(XRCID("set_project_active"), wxEmptyString, wxXmlResource::Get()->LoadBitmap(wxT("set_active")), wxT("Select Active Project"), wxITEM_NORMAL);
	tb->AddSeparator();

	// add the 'multiple/single' tree style
	long val (0);
	if(EditorConfigST::Get()->GetLongValue(wxT("WspTreeMultipleSelection"), val) == false) {val = 0;}

	tb->AddTool(XRCID("set_multi_selection"), wxEmptyString, wxXmlResource::Get()->LoadBitmap(wxT("outline16")), wxT("Toggle Multiple / Single Selection"), wxITEM_CHECK);
	tb->ToggleTool(XRCID("set_multi_selection"), val ? true : false);
	tb->Realize();
	sz->Add(tb, 0, wxEXPAND, 0);

	m_fileView = new FileViewTree(this, wxID_ANY);
	sz->Add(m_fileView, 1, wxEXPAND|wxTOP, 2);
}

void WorkspaceTab::ConnectEvents()
{
	Connect( XRCID("link_editor"),        wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnLinkEditor));
	Connect( XRCID("set_multi_selection"),wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnToggleMultiSelection));
	Connect( XRCID("set_multi_selection"),wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnCollapseAllUI));
	Connect( XRCID("collapse_all"),       wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnCollapseAll));
	Connect( XRCID("collapse_all"),       wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnCollapseAllUI));
	Connect( XRCID("go_home"),            wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnGoHome));
	Connect( XRCID("go_home"),            wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnGoHomeUI));
	Connect( XRCID("project_properties"), wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnProjectSettingsUI));
	Connect( XRCID("project_properties"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnProjectSettings));
	Connect( XRCID("set_project_active"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnShowProjectListPopup));
	Connect( XRCID("set_project_active"), wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnProjectSettingsUI));

    wxTheApp->Connect(XRCID("show_in_workspace"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (WorkspaceTab::OnShowFile),   NULL, this);
    wxTheApp->Connect(XRCID("show_in_workspace"), wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(WorkspaceTab::OnShowFileUI), NULL, this);

    wxTheApp->Connect(wxEVT_WORKSPACE_LOADED,      wxCommandEventHandler(WorkspaceTab::OnWorkspaceLoaded),     NULL, this);
    wxTheApp->Connect(wxEVT_WORKSPACE_CLOSED,      wxCommandEventHandler(WorkspaceTab::OnWorkspaceClosed),     NULL, this);
    wxTheApp->Connect(wxEVT_PROJ_ADDED,            wxCommandEventHandler(WorkspaceTab::OnProjectAdded),        NULL, this);
    wxTheApp->Connect(wxEVT_PROJ_REMOVED,          wxCommandEventHandler(WorkspaceTab::OnProjectRemoved),      NULL, this);
    wxTheApp->Connect(wxEVT_ACTIVE_EDITOR_CHANGED, wxCommandEventHandler(WorkspaceTab::OnActiveEditorChanged), NULL, this);
    wxTheApp->Connect(wxEVT_EDITOR_CLOSING,        wxCommandEventHandler(WorkspaceTab::OnEditorClosing),       NULL, this);
}

void WorkspaceTab::OnLinkEditor(wxCommandEvent &e)
{
	m_isLinkedToEditor = !m_isLinkedToEditor;
	EditorConfigST::Get()->SaveLongValue(wxT("LinkWorkspaceViewToEditor"), m_isLinkedToEditor ? 1 : 0);
	if (m_isLinkedToEditor) {
        OnActiveEditorChanged(e);
	}
}

void WorkspaceTab::OnCollapseAll(wxCommandEvent &e)
{
	wxUnusedVar(e);
    if (!m_fileView->GetRootItem().IsOk())
        return;
    m_fileView->Freeze();
    m_fileView->CollapseAll();
    m_fileView->Expand(m_fileView->GetRootItem());
	// count will probably be 0 below, so ensure we can at least see the root item
	m_fileView->EnsureVisible(m_fileView->GetRootItem());
    m_fileView->Thaw();

	wxArrayTreeItemIds arr;
	size_t count = m_fileView->GetSelections( arr );

	if ( count == 1 ) {
		wxTreeItemId sel = arr.Item(0);
		if (sel.IsOk()) {
			m_fileView->EnsureVisible(sel);
		}
	}
}

void WorkspaceTab::OnCollapseAllUI(wxUpdateUIEvent &e)
{
	e.Enable(ManagerST::Get()->IsWorkspaceOpen());
}

void WorkspaceTab::OnGoHome(wxCommandEvent &e)
{
	wxUnusedVar(e);
	wxString activeProject = ManagerST::Get()->GetActiveProjectName();
	if (activeProject.IsEmpty())
		return;
	m_fileView->ExpandToPath(activeProject, wxFileName());

	wxArrayTreeItemIds arr;
	size_t count = m_fileView->GetSelections( arr );

	if ( count == 1 ) {
		wxTreeItemId sel = arr.Item(0);
		if (sel.IsOk() && m_fileView->ItemHasChildren(sel))
			m_fileView->Expand(sel);
	}
	ManagerST::Get()->ShowWorkspacePane(m_caption);
}

void WorkspaceTab::OnGoHomeUI(wxUpdateUIEvent &e)
{
	e.Enable(!ManagerST::Get()->GetActiveProjectName().IsEmpty());
}

void WorkspaceTab::OnProjectSettings(wxCommandEvent& e)
{
	wxString projectName = ManagerST::Get()->GetActiveProjectName();
	wxString title( projectName );
	title << wxT( " Project Settings" );

	//open the project properties dialog
	BuildMatrixPtr matrix = ManagerST::Get()->GetWorkspaceBuildMatrix();

	//find the project configuration name that matches the workspace selected configuration
	ProjectSettingsDlg dlg( Frame::Get(), matrix->GetProjectSelectedConf( matrix->GetSelectedConfigurationName(), projectName ), projectName, title );
	dlg.ShowModal();

	//mark this project as modified
	ProjectPtr proj = ManagerST::Get()->GetProject(projectName);
	if (proj) {
		proj->SetModified(true);
	}
}

void WorkspaceTab::OnProjectSettingsUI(wxUpdateUIEvent& e)
{
	e.Enable(!ManagerST::Get()->GetActiveProjectName().IsEmpty());
}

static int wxStringCmpFunc(const wxString& item1, const wxString& item2)
{
	return item1.CmpNoCase(item2);
}

void WorkspaceTab::OnShowProjectListPopup(wxCommandEvent& e)
{
	wxUnusedVar(e);

#ifdef __WXMSW__
	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
#endif

	wxMenu popupMenu;

	wxArrayString projects;
	ManagerST::Get()->GetProjectList(projects);
	projects.Sort(wxStringCmpFunc);
	for (size_t i = 0; i < projects.GetCount(); i++) {
		wxString text = projects.Item(i);
		bool selected = ManagerST::Get()->GetActiveProjectName() == text;
		wxMenuItem *item = new wxMenuItem(&popupMenu, i, text, text, wxITEM_CHECK);

		//set the font
#ifdef __WXMSW__
		if (selected) {
			font.SetWeight(wxBOLD);
		}
		item->SetFont(font);
#endif
		popupMenu.Append(item);
		item->Check(selected);

		//restore font
#ifdef __WXMSW__
		font.SetWeight(wxNORMAL);
#endif
	}

    popupMenu.Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(WorkspaceTab::OnMenuSelection), NULL, this);
	PopupMenu(&popupMenu);
}

void WorkspaceTab::OnMenuSelection(wxCommandEvent& e)
{
	wxArrayString projects;
	ManagerST::Get()->GetProjectList(projects);
	projects.Sort(wxStringCmpFunc);

	size_t sel = (size_t)e.GetId();
	if(sel < projects.GetCount()) {
		m_fileView->MarkActive(projects.Item(sel));
	}
}

void WorkspaceTab::OnShowFile(wxCommandEvent& e)
{
    LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
    if (editor && !editor->GetProject().IsEmpty()) {
        m_fileView->ExpandToPath(editor->GetProject(), editor->GetFileName());
        ManagerST::Get()->ShowWorkspacePane(m_caption);
    }
    e.Skip();
}

void WorkspaceTab::OnShowFileUI(wxUpdateUIEvent& e)
{
	LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
	e.Enable(editor && !editor->GetProject().IsEmpty());
}

void WorkspaceTab::OnActiveEditorChanged(wxCommandEvent& e)
{
    e.Skip();
    if (m_isLinkedToEditor) {
        LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
        if (editor && !editor->GetProject().IsEmpty()) {
            m_fileView->ExpandToPath(editor->GetProject(), editor->GetFileName());
        }
    }
}

void WorkspaceTab::OnWorkspaceLoaded(wxCommandEvent& e)
{
    e.Skip();
    Freeze();
    m_fileView->BuildTree();
    OnGoHome(e);
    Thaw();
    SendCmdEvent(wxEVT_FILE_VIEW_INIT_DONE);
}

void WorkspaceTab::OnEditorClosing(wxCommandEvent& e)
{
    e.Skip();
}

void WorkspaceTab::OnWorkspaceClosed(wxCommandEvent& e)
{
    e.Skip();
    m_fileView->DeleteAllItems();
    SendCmdEvent(wxEVT_FILE_VIEW_INIT_DONE);
}

void WorkspaceTab::OnProjectAdded(wxCommandEvent& e)
{
    e.Skip();
    const wxString *projName = (const wxString *) e.GetClientData();
    m_fileView->BuildTree();
    if (projName && !projName->IsEmpty()) {
        m_fileView->ExpandToPath(*projName, wxFileName());
    }
    SendCmdEvent(wxEVT_FILE_VIEW_REFRESHED);
}

void WorkspaceTab::OnProjectRemoved(wxCommandEvent& e)
{
    e.Skip();
    Freeze();
    m_fileView->BuildTree();
    OnGoHome(e);
    Thaw();
    SendCmdEvent(wxEVT_FILE_VIEW_REFRESHED);
}

void WorkspaceTab::OnToggleMultiSelection(wxCommandEvent& e)
{
	EditorConfigST::Get()->SaveLongValue(wxT("WspTreeMultipleSelection"), e.IsChecked() ? 1 : 0);
	// Reload the tree
	int answer = wxMessageBox(_("Workspace reload is required\nWould you like to reload workspace now?"), wxT("EmbeddedLite"), wxICON_INFORMATION|wxYES_NO|wxCANCEL, this);
	if ( answer == wxYES ) {
		wxCommandEvent e(wxEVT_COMMAND_MENU_SELECTED, XRCID("reload_workspace"));
		Frame::Get()->GetEventHandler()->AddPendingEvent(e);
	}
}
