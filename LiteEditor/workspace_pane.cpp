/**
  \file 

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#include <wx/app.h>
#include <wx/xrc/xmlres.h>
#include "parse_thread.h"
#include "editor_config.h"
#include "configuration_manager_dlg.h"
#include "detachedpanesinfo.h"
#include "dockablepane.h"
#include "manager.h"
#include "frame.h"
#include "cl_editor.h"
#include "custom_notebook.h"
#include "cpp_symbol_tree.h"
#include "windowstack.h"
#include "macros.h"
#include "fileview.h"
#include "openwindowspanel.h"
#include "fileexplorer.h"
#include "workspacetab.h"
#include "workspace_pane.h"

#define OPEN_CONFIG_MGR_STR wxT("<Open Configuration Manager...>")

CSolutionPanel::CSolutionPanel(wxWindow *parent, const wxString &caption, wxAuiManager *mgr):
  wxPanel(parent),
  m_caption(caption),
  m_mgr(mgr),
  m_pOpenWindowsPanel(NULL),
  m_winStack(NULL)
{
	CreateGUIControls();
  Connect();
}

CSolutionPanel::~CSolutionPanel()
{
}

#define ADD_WORKSPACE_PAGE(win, name) \
	if( detachedPanes.Index(name) != wxNOT_FOUND ) {\
		new DockablePane(GetParent(), m_book, win, name, wxNullBitmap, wxSize(200, 200));\
	} else {\
		m_book->AddPage(win, name, name, wxNullBitmap, true);\
	}

void
CSolutionPanel::CreateGUIControls()
{
	wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(mainSizer);

    // selected configuration:

	mainSizer->Add(new wxStaticText(this, wxID_ANY, wxT("Selected Configuration:")), 0, wxALIGN_CENTER_HORIZONTAL| wxALL, 2);

	wxBoxSizer *hsz = new wxBoxSizer(wxHORIZONTAL);
	mainSizer->Add(hsz, 0, wxEXPAND|wxTOP|wxBOTTOM, 5);

	wxArrayString choices;
	m_pSolutionConfig = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices);
	m_pSolutionConfig->Enable(false);
	m_pSolutionConfig->Append(OPEN_CONFIG_MGR_STR);
	ConnectChoice(m_pSolutionConfig, CSolutionPanel::OnConfigurationManagerChoice);
	hsz->Add(m_pSolutionConfig, 1, wxEXPAND| wxALL, 1);
	
#ifdef __WXMAC__
	m_pSolutionConfig->SetWindowVariant(wxWINDOW_VARIANT_SMALL);
#endif

    // add notebook for tabs
	long bookStyle = wxVB_LEFT|wxVB_FIXED_WIDTH;
	EditorConfigST::Get()->GetLongValue(wxT("SolutionView"), bookStyle);
	m_book = new Notebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, bookStyle);

	// Calculate the widthest tab (the one with the 'Solution' label)
	int xx, yy;
	wxFont fnt = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxWindow::GetTextExtent(wxT("Solution"), &xx, &yy, NULL, NULL, &fnt);
	m_book->SetFixedTabWidth(xx + 20);

    m_book->SetAuiManager(m_mgr, m_caption);
	mainSizer->Add(m_book, 1, wxEXPAND | wxALL, 1);

    // create tabs (possibly detached)
	DetachedPanesInfo dpi;
	EditorConfigST::Get()->ReadObject(wxT("DetachedPanesList"), &dpi);
	wxArrayString detachedPanes = dpi.GetPanes();

	m_pSolutionTab = new CSolutionTab(m_book, wxT("Solution"));
	ADD_WORKSPACE_PAGE(m_pSolutionTab, m_pSolutionTab->GetCaption());

	m_pFileSystemBrowser = new CFileSystemBrowser(m_book, wxT("File system"));
	ADD_WORKSPACE_PAGE(m_pFileSystemBrowser, m_pFileSystemBrowser->GetCaption());
/*rvv
	m_winStack = new WindowStack(m_book, wxID_ANY);
  //rvv
	ADD_WORKSPACE_PAGE(m_winStack, wxT("Outline"));
*/
//rvv	m_pOpenWindowsPanel = new OpenWindowsPanel(m_book, wxT("Tabs"));
//rvv	ADD_WORKSPACE_PAGE(m_pOpenWindowsPanel, m_pOpenWindowsPanel->GetCaption());

	if (m_book->GetPageCount() > 0) 
	{
		m_book->SetSelection((size_t)0);
	}

	m_mgr->Update();
}

void CSolutionPanel::Connect()
{
	wxTheApp->Connect(wxEVT_WORKSPACE_LOADED,         wxCommandEventHandler(CSolutionPanel::OnSolutionConfig),     NULL, this);
	wxTheApp->Connect(wxEVT_WORKSPACE_CONFIG_CHANGED, wxCommandEventHandler(CSolutionPanel::OnSolutionConfig),     NULL, this);
	wxTheApp->Connect(wxEVT_WORKSPACE_CLOSED,         wxCommandEventHandler(CSolutionPanel::OnSolutionClosed),     NULL, this);
	wxTheApp->Connect(wxEVT_PROJ_FILE_ADDED,          wxCommandEventHandler(CSolutionPanel::OnProjectFileAdded),    NULL, this);
	wxTheApp->Connect(wxEVT_PROJ_FILE_REMOVED,        wxCommandEventHandler(CSolutionPanel::OnProjectFileRemoved),  NULL, this);
	wxTheApp->Connect(wxEVT_SYNBOL_TREE_UPDATE_ITEM,  wxCommandEventHandler(CSolutionPanel::OnSymbolsUpdated),      NULL, this);
	wxTheApp->Connect(wxEVT_SYNBOL_TREE_DELETE_ITEM,  wxCommandEventHandler(CSolutionPanel::OnSymbolsDeleted),      NULL, this);
	wxTheApp->Connect(wxEVT_SYNBOL_TREE_ADD_ITEM,     wxCommandEventHandler(CSolutionPanel::OnSymbolsAdded),        NULL, this);
	wxTheApp->Connect(wxEVT_FILE_RETAGGED,            wxCommandEventHandler(CSolutionPanel::OnFileRetagged),        NULL, this);
	wxTheApp->Connect(wxEVT_ACTIVE_EDITOR_CHANGED,    wxCommandEventHandler(CSolutionPanel::OnActiveEditorChanged), NULL, this);
	wxTheApp->Connect(wxEVT_EDITOR_CLOSING,           wxCommandEventHandler(CSolutionPanel::OnEditorClosing),       NULL, this);
	wxTheApp->Connect(wxEVT_ALL_EDITORS_CLOSED,       wxCommandEventHandler(CSolutionPanel::OnAllEditorsClosed),    NULL, this);

    wxTheApp->Connect(XRCID("configuration_manager"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler (CSolutionPanel::OnConfigurationManager),   NULL, this);
    wxTheApp->Connect(XRCID("configuration_manager"), wxEVT_UPDATE_UI,             wxUpdateUIEventHandler(CSolutionPanel::OnConfigurationManagerUI), NULL, this);
}


extern wxImageList* CreateSymbolTreeImages();

void
CSolutionPanel::ShowCurrentOutline()
{
/*rvv
    LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
    if (!editor || editor->GetProjectName().IsEmpty()) {
        m_winStack->SelectNone();
        return;
    }
    wxString path = editor->GetFileName().GetFullPath();
    if (m_winStack->GetSelectedKey() != path) {
        m_winStack->Freeze();
        if (m_winStack->Find(path) == NULL) {
            CppSymbolTree *tree = new CppSymbolTree(m_winStack, wxID_ANY);
            tree->SetSymbolsImages(CreateSymbolTreeImages());
            tree->BuildTree(path);
            m_winStack->Add(tree, path);
        }
        m_winStack->Select(path);
        m_winStack->Thaw();
    }
*/    
}

void CSolutionPanel::OnActiveEditorChanged(wxCommandEvent& e)
{
    e.Skip();
    ShowCurrentOutline();
}

void CSolutionPanel::OnAllEditorsClosed(wxCommandEvent& e)
{
    e.Skip();
//rvv    m_winStack->Clear();
}

void CSolutionPanel::OnEditorClosing(wxCommandEvent& e)
{
    e.Skip();
    IEditor *editor = (IEditor*) e.GetClientData();
    if (editor && !editor->GetProjectName().IsEmpty()) {
//rvv        m_winStack->Delete(editor->GetFileName().GetFullPath());
    }
}

void CSolutionPanel::OnFileRetagged(wxCommandEvent& e)
{
/*rvv
    e.Skip();
    std::vector<wxFileName> *files = (std::vector<wxFileName>*) e.GetClientData();
	if (files && !files->empty()) {
        m_winStack->Freeze();
        // toss out any out-of-date outlines
        for (size_t i = 0; i < files->size(); i++) {
            m_winStack->Delete(files->at(i).GetFullPath());
        }
        ShowCurrentOutline(); // in case active editor's file was one of them
        m_winStack->Thaw();
    }
*/    
}

void CSolutionPanel::OnProjectFileAdded(wxCommandEvent& e)
{
    e.Skip();
    ShowCurrentOutline(); // in case the active editor's file is now tagged
}

void CSolutionPanel::OnProjectFileRemoved(wxCommandEvent& e)
{
/*rvv
    e.Skip();
    wxArrayString *files = (wxArrayString*) e.GetClientData();
	if (files && !files->IsEmpty()) {
        for (size_t i = 0; i < files->Count(); i++) {
            m_winStack->Delete(files->Item(i));
        }
        ShowCurrentOutline(); // in case active editor's file is no longer tagged
    }
*/    
}

void CSolutionPanel::OnSymbolsAdded(wxCommandEvent& e)
{
/*rvv
    e.Skip();
    ParseThreadEventData *data = (ParseThreadEventData*) e.GetClientData();
	if (data && !data->GetItems().empty()) {
        CppSymbolTree *tree = (CppSymbolTree*) m_winStack->Find(data->GetFileName());
        if (tree) {
            tree->AddSymbols(data->GetItems());
        }
    }
*/    
}

void CSolutionPanel::OnSymbolsDeleted(wxCommandEvent& e)
{
/*rvv
    e.Skip();
    ParseThreadEventData *data = (ParseThreadEventData*) e.GetClientData();
	if (data && !data->GetItems().empty()) {
        CppSymbolTree *tree = (CppSymbolTree*) m_winStack->Find(data->GetFileName());
        if (tree) {
            tree->DeleteSymbols(data->GetItems());
        }
    }
*/    
}

void CSolutionPanel::OnSymbolsUpdated(wxCommandEvent& e)
{
/*
    e.Skip();
    ParseThreadEventData *data = (ParseThreadEventData*) e.GetClientData();
	if (data && !data->GetItems().empty()) {
        CppSymbolTree *tree = (CppSymbolTree*) m_winStack->Find(data->GetFileName());
        if (tree) {
            tree->UpdateSymbols(data->GetItems());
        }
    }
*/    
}

void CSolutionPanel::OnSolutionConfig(wxCommandEvent& e)
{
    e.Skip();

    BuildMatrixPtr matrix = SolutionST::Get()->GetBuildMatrix();
	std::list<CSolitionConfigurationPtr> confs = matrix->GetConfigurations();

	m_pSolutionConfig->Freeze();
    m_pSolutionConfig->Enable(true);
	m_pSolutionConfig->Clear();
	for (std::list<CSolitionConfigurationPtr>::iterator iter = confs.begin() ; iter != confs.end(); iter++) {
		m_pSolutionConfig->Append((*iter)->GetName());
	}
	if (m_pSolutionConfig->GetCount() > 0) {
        m_pSolutionConfig->SetStringSelection(matrix->GetSelectedConfigurationName());
	}
	m_pSolutionConfig->Append(OPEN_CONFIG_MGR_STR);
	m_pSolutionConfig->Thaw();
}

void CSolutionPanel::OnSolutionClosed(wxCommandEvent& e)
{
    e.Skip();
    m_pSolutionConfig->Clear();
    m_pSolutionConfig->Enable(false);
//rvv    m_winStack->Clear();
}

void CSolutionPanel::OnConfigurationManagerUI(wxUpdateUIEvent& e)
{
	e.Enable(ManagerST::Get()->IsWorkspaceOpen());
}

void CSolutionPanel::OnConfigurationManagerChoice(wxCommandEvent &event)
{
	wxString selection = m_pSolutionConfig->GetStringSelection();
	if(selection == OPEN_CONFIG_MGR_STR){
		wxCommandEvent e(wxEVT_COMMAND_MENU_SELECTED, XRCID("configuration_manager"));
		e.SetEventObject(this);
		ProcessEvent(e);
		return;
	}

	BuildMatrixPtr matrix = ManagerST::Get()->GetWorkspaceBuildMatrix();
	matrix->SetSelectedConfigurationName(selection);
	ManagerST::Get()->SetWorkspaceBuildMatrix(matrix);

	// Set the focus to the active editor if any
	LEditor *editor = Frame::Get()->GetMainBook()->GetActiveEditor();
	if(editor)
		editor->SetActive();

}

void CSolutionPanel::OnConfigurationManager(wxCommandEvent& e)
{
	wxUnusedVar(e);
	ConfigurationManagerDlg dlg(this);
	dlg.ShowModal();

	BuildMatrixPtr matrix = ManagerST::Get()->GetWorkspaceBuildMatrix();
	m_pSolutionConfig->SetStringSelection(matrix->GetSelectedConfigurationName());
}
