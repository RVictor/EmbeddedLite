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
#ifndef __quickfindbar__
#define __quickfindbar__

#include <wx/panel.h>
#include "quickfindbarbase.h"

class wxScintilla;
class CQuickFindBar : public QuickFindBarBase
{
	wxScintilla  *m_sci;
	size_t        m_flags;
protected:
	void ShowReplaceControls(bool show = true);
	void DoSearch(bool fwd, bool incr);
	wxTextCtrl *GetFocusedControl();
	void DoShowControls();
	
	// General events
	void OnCopy         (wxCommandEvent &e);
	void OnPaste        (wxCommandEvent &e);
	void OnSelectAll    (wxCommandEvent &e);
	void OnEditUI       (wxUpdateUIEvent &e);


	// Control events
	void OnHide         (wxCommandEvent &e);
	void OnNext         (wxCommandEvent &e);
	void OnPrev         (wxCommandEvent &e);
	void OnText         (wxCommandEvent &e);
	void OnKeyDown      (wxKeyEvent      &e);
	void OnEnter        (wxCommandEvent &e);
	void OnReplace      (wxCommandEvent &e);
	void OnUpdateUI     (wxUpdateUIEvent &e);
	void OnReplaceUI    (wxUpdateUIEvent &e);
	void OnReplaceEnter (wxCommandEvent &e);
	void OnCheckBoxCase ( wxCommandEvent& event );
	void OnCheckBoxRegex( wxCommandEvent& event );
	void OnCheckBoxWord ( wxCommandEvent& event );
	void OnToggleReplaceControls( wxCommandEvent& event );

public:
	CQuickFindBar(wxWindow *parent, wxWindowID id = wxID_ANY);
	int GetCloseButtonId();
	bool Show(bool s = true);

	wxScintilla *GetEditor() {
		return m_sci;
	}
	void SetEditor(wxScintilla *sci) ;
};

#endif // __quickfindbar__
