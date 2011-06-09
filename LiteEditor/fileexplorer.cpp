//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : fileexplorer.cpp
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
 #include "wx/xrc/xmlres.h"
#include "fileexplorer.h"
#include "fileexplorertree.h"
#include "wx/sizer.h"
#include "wx/tokenzr.h"

#include "macros.h"
#include "globals.h"
#include "plugin.h"
#include "editor_config.h"
#include "manager.h"
#include "workspace_pane.h"
#include "frame.h"

CFileSystemBrowser::CFileSystemBrowser(wxWindow *parent, const wxString &caption): 
  wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(250, 300)), 
  m_caption(caption), 
  m_bSyncWithEditor(false)
#ifdef __WXMSW__
#if wxUSE_FSVOLUME
, m_thread(this)
#endif
#endif
{
	long link(0);
	EditorConfigST::Get()->GetLongValue(wxT("SyncFileSystemBrowserToEditor"), link);
	m_bSyncWithEditor = link ? true : false;
	CreateGUIControls();
}

CFileSystemBrowser::~CFileSystemBrowser()
{
}

void CFileSystemBrowser::CreateGUIControls()
{
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainSizer);

	wxToolBar *tb = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT|wxTB_HORIZONTAL|wxTB_NODIVIDER);

	mainSizer->Add(tb, 0, wxEXPAND);

#ifdef __WXMSW__
#if wxUSE_FSVOLUME
	wxArrayString volumes;
	Connect(wxEVT_THREAD_VOLUME_COMPLETED, wxCommandEventHandler(CFileSystemBrowser::OnVolumes), NULL, this);

	m_thread.Create();
	m_thread.Run();

	m_volumes = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, volumes, 0 );
	mainSizer->Add(m_volumes, 0, wxEXPAND|wxALL, 1);
	ConnectChoice(m_volumes, CFileSystemBrowser::OnVolumeChanged);
#endif
#endif

	m_fileTree = new CFileSystemBrowserTree(this, wxID_ANY);
	m_fileTree->Connect(wxVDTC_ROOT_CHANGED, wxCommandEventHandler(CFileSystemBrowser::OnRootChanged), NULL, this);
	mainSizer->Add(m_fileTree, 1, wxEXPAND|wxALL, 1);

	tb->AddTool(XRCID("link_editor"), wxEmptyString, wxXmlResource::Get()->LoadBitmap(wxT("link_editor")), wxT("Link Editor"), wxITEM_CHECK);
	tb->ToggleTool(XRCID("link_editor"), m_bSyncWithEditor);
	tb->AddTool(XRCID("collapse_all"), wxEmptyString, wxXmlResource::Get()->LoadBitmap(wxT("collapse")), wxT("Collapse All"), wxITEM_NORMAL);
	tb->AddTool(XRCID("go_home"), wxEmptyString, wxXmlResource::Get()->LoadBitmap(wxT("gohome")), wxT("Goto Current Directory"), wxITEM_NORMAL);
	tb->Realize();

	Connect( XRCID("link_editor"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CFileSystemBrowser::OnLinkEditor ));
	Connect( XRCID("collapse_all"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CFileSystemBrowser::OnCollapseAll ));
	Connect( XRCID("go_home"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CFileSystemBrowser::OnGoHome ));

	mainSizer->Layout();

    wxTheApp->Connect(XRCID("show_in_filesystembrowser"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(CFileSystemBrowser::OnShowFile), NULL, this);
    wxTheApp->Connect(XRCID("show_in_filesystembrowser"), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(CFileSystemBrowser::OnShowFileUI), NULL, this);
    wxTheApp->Connect(wxEVT_WORKSPACE_LOADED, wxCommandEventHandler(CFileSystemBrowser::OnWorkspaceLoaded), NULL, this);
    wxTheApp->Connect(wxEVT_ACTIVE_EDITOR_CHANGED, wxCommandEventHandler(CFileSystemBrowser::OnActiveEditorChanged), NULL, this);
}

void CFileSystemBrowser::OnCollapseAll(wxCommandEvent &e)
{
	wxUnusedVar(e);
    m_fileTree->CollapseAll();
    wxTreeItemId root = m_fileTree->GetRootItem();
    if (root.IsOk()) {
        m_fileTree->Expand(m_fileTree->GetRootItem());
    }
	wxTreeItemId sel = m_fileTree->GetSelection();
	if (sel.IsOk()) {
		m_fileTree->EnsureVisible(sel);
    }
	m_fileTree->SetFocus();
}

void CFileSystemBrowser::OnGoHome(wxCommandEvent &e)
{
	wxUnusedVar(e);
	m_fileTree->ExpandToPath(wxGetCwd());
	m_fileTree->SetFocus();
}

void CFileSystemBrowser::OnLinkEditor(wxCommandEvent &e)
{
	m_bSyncWithEditor = !m_bSyncWithEditor;
	EditorConfigST::Get()->SaveLongValue(wxT("SyncFileSystemBrowserToEditor"), m_bSyncWithEditor ? 1 : 0);
    if (m_bSyncWithEditor) {
        OnActiveEditorChanged(e);
    }
}

void CFileSystemBrowser::OnShowFile(wxCommandEvent& e)
{
    LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
    if (editor && editor->GetFileName().FileExists()) {
        m_fileTree->ExpandToPath(editor->GetFileName());
        ManagerST::Get()->ShowWorkspacePane(m_caption);
    }
    e.Skip();
}

void CFileSystemBrowser::OnShowFileUI(wxUpdateUIEvent& e)
{
	LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
	e.Enable(editor && editor->GetFileName().FileExists());
}

void CFileSystemBrowser::OnActiveEditorChanged(wxCommandEvent& e)
{
    e.Skip();
    if (m_bSyncWithEditor) {
        LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
        if (editor && editor->GetFileName().FileExists()) {
            m_fileTree->ExpandToPath(editor->GetFileName());
        }
    }
}

void CFileSystemBrowser::OnWorkspaceLoaded(wxCommandEvent& e)
{
    e.Skip();
    wxUnusedVar(e);
    if (m_bSyncWithEditor) {
        m_fileTree->ExpandToPath(SolutionST::Get()->GetWorkspaceFileName().GetPath());
    }
}

void CFileSystemBrowser::OnRootChanged(wxCommandEvent &e)
{
	wxTreeItemId root = m_fileTree->GetRootItem();
	if(root.IsOk()){
		wxString vol = m_fileTree->GetItemText(root);
#ifdef __WXMSW__
#if wxUSE_FSVOLUME
        if(m_volumes->FindString(vol) == wxNOT_FOUND) {
            m_volumes->AppendString(vol);
        }
		m_volumes->SetStringSelection(vol);
#endif
#endif
        SendCmdEvent(wxEVT_FILE_EXP_INIT_DONE);
	}
	e.Skip();
}

#ifdef __WXMSW__
#if wxUSE_FSVOLUME
void CFileSystemBrowser::OnVolumeChanged(wxCommandEvent &e)
{
	wxUnusedVar(e);
	m_fileTree->SetRootPath(m_volumes->GetStringSelection());
	m_fileTree->SetFocus();
}

void CFileSystemBrowser::OnVolumes(wxCommandEvent &e)
{
	wxString curvol = m_volumes->GetStringSelection();
	wxArrayString volumes = wxStringTokenize(e.GetString(), wxT(";"), wxTOKEN_STRTOK);
	int where = volumes.Index(curvol);
	if(where != wxNOT_FOUND){
		volumes.RemoveAt((size_t)where);
	}
	m_volumes->Append(volumes);
}
#endif
#endif
