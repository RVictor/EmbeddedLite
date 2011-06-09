//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : workspace_pane.h
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
#ifndef WORKSPACE_PANE_H
#define WORKSPACE_PANE_H

#include <wx/filename.h>
#include <wx/panel.h>

// Forward Declarations
class Notebook;
class FileViewTree;
class WindowStack;
class OpenWindowsPanel;
class CFileSystemBrowser;
class CSolutionTab;

class CSolutionPanel: public wxPanel
{
  private:
	  wxString              m_caption;
	  wxChoice*             m_pSolutionConfig;
	  wxAuiManager*         m_mgr;

	  Notebook*             m_book;
	  WindowStack*          m_winStack;
	  OpenWindowsPanel*     m_pOpenWindowsPanel;
	  CFileSystemBrowser*   m_pFileSystemBrowser;
	  CSolutionTab*         m_pSolutionTab;

	  void CreateGUIControls();
    void Connect();
    void ShowCurrentOutline();

    // Solution event handlers
    void OnSolutionConfig(wxCommandEvent &e);
    void OnSolutionClosed(wxCommandEvent &e);
    void OnFileRetagged(wxCommandEvent &e);
    void OnProjectFileAdded(wxCommandEvent &e);
    void OnProjectFileRemoved(wxCommandEvent &e);
    void OnSymbolsUpdated(wxCommandEvent &e);
    void OnSymbolsDeleted(wxCommandEvent &e);
    void OnSymbolsAdded(wxCommandEvent &e);
    void OnActiveEditorChanged(wxCommandEvent &e);
    void OnEditorClosing(wxCommandEvent &e);
    void OnAllEditorsClosed(wxCommandEvent &e);

	  // Configuration mgr handlers
	  void OnConfigurationManager(wxCommandEvent &e);
	  void OnConfigurationManagerUI(wxUpdateUIEvent &e);
	  void OnConfigurationManagerChoice(wxCommandEvent &event);

  public:
	  CSolutionPanel(wxWindow *parent, const wxString &caption, wxAuiManager *mgr);
	  ~CSolutionPanel();

	  // Getters
	  const wxString& GetCaption() const { return m_caption; }
	  Notebook* GetNotebook() { return m_book; }
    CSolutionTab* GetSolutionTab() { return m_pSolutionTab; }
	  CFileSystemBrowser* GetFileSystemBrowser() { return m_pFileSystemBrowser; }
};

#endif // WORKSPACE_PANE_H

