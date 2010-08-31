//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : mainbook.h
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
#ifndef MAINBOOK_H
#define MAINBOOK_H

#include <set>
#include <wx/panel.h>
#include "sessionmanager.h"
#include "navbar.h"
#include "quickfindbar.h"
#include "custom_notebook.h"
#include "filehistory.h"
#include "message_pane.h"

class MessagePane;
class MainBook : public wxPanel
{
private:
    FileHistory   m_recentFiles;
	NavBar       *m_navBar;
	Notebook     *m_book;
	QuickFindBar *m_quickFindBar;
    wxWindow     *m_currentPage;

    std::set<wxWindow*> m_detachedTabs;
	MessagePane  *m_messagePane;
	
private:
    void CreateGuiControls();
    void ConnectEvents    ();

    void OnMouseDClick       (wxMouseEvent      &e);
    void OnFocus             (wxFocusEvent      &e);
    void OnPaneClosed        (wxAuiManagerEvent &e);
    void OnPageClosing       (NotebookEvent     &e);
    void OnPageClosed        (NotebookEvent     &e);
    void OnProjectFileAdded  (wxCommandEvent    &e);
	void OnProjectFileRemoved(wxCommandEvent    &e);
    void OnWorkspaceLoaded   (wxCommandEvent    &e);
    void OnWorkspaceClosed   (wxCommandEvent    &e);

	bool AskUserToSave(LEditor *editor);
	bool DoSelectPage (wxWindow *win  );

public:
	MainBook(wxWindow *parent);
	~MainBook();

	void ClearFileHistory();
	void GetRecentlyOpenedFiles(wxArrayString &files);
	FileHistory &GetRecentlyOpenedFilesClass() { return m_recentFiles; }

	void ShowQuickBar (bool s = true)       { m_quickFindBar->Show(s); }
	void ShowMessage  (const wxString &message, bool showHideButton = true, const wxBitmap &bmp = wxNullBitmap, const ButtonDetails &btn1 = ButtonDetails(), const ButtonDetails &btn2 = ButtonDetails(), const ButtonDetails &btn3 = ButtonDetails());
	
	void ShowNavBar   (bool s = true);
	void UpdateNavBar (LEditor *editor);
	bool IsNavBarShown()                    { return m_navBar->IsShown(); }

    void SaveSession   (SessionEntry &session, wxArrayInt& intArr);
    void RestoreSession(SessionEntry &session);

    LEditor *GetActiveEditor();
    void     GetAllEditors  (std::vector<LEditor*> &editors);
	LEditor *FindEditor     (const wxString &fileName);
    bool     CloseEditor    (const wxString &fileName) { return ClosePage(FindEditor(fileName)); }

    wxWindow *GetCurrentPage() { return m_currentPage; }
    wxWindow *FindPage      (const wxString &text);

    LEditor *NewEditor();

	LEditor *OpenFile(const wxString &file_name, const wxString &projectName = wxEmptyString,
	                  int lineno = wxNOT_FOUND, long position = wxNOT_FOUND, bool addjump = true);
	LEditor *OpenFile(const BrowseRecord &rec)
        { return OpenFile(rec.filename, rec.project, rec.lineno, rec.position, false); }

    bool AddPage   (wxWindow *win, const wxString &text, const wxBitmap &bmp = wxNullBitmap, bool selected = false);
    bool SelectPage(wxWindow *win);

    bool DetachPage(wxWindow *win);
    bool DockPage  (wxWindow *win);
    bool IsDetached(wxWindow *win);

    bool UserSelectFiles(std::vector<std::pair<wxFileName,bool> > &files, const wxString &title, const wxString &caption,
                         bool cancellable = true);

	bool SaveAll(bool askUser, bool includeUntitled);

    void ReloadExternallyModified(bool prompt);

    bool ClosePage      (const wxString &text) { return ClosePage(FindPage(text)); }
    bool ClosePage      (wxWindow *win);
	bool CloseAllButThis(wxWindow *win);
	bool CloseAll       (bool cancellable);

	wxString GetPageTitle(wxWindow *win);
	void     SetPageTitle(wxWindow *page, const wxString &name);
	long     GetBookStyle();

    // TODO: replace these functions with event handlers
	void ApplySettingsChanges   ();
    void UnHighlightAll         ();
    void DelAllBreakpointMarkers();
    void SetViewEOL             (bool visible);
    void HighlightWord          (bool hl);
    void ShowWhitespace         (int ws);
    void UpdateColours          ();
    void UpdateBreakpoints      ();
	void MarkEditorReadOnly		(LEditor *editor, bool ro);
};

#endif //MAINBOOK_H
