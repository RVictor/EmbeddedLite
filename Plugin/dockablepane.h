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
#ifndef __dockablepane__
#define __dockablepane__

#include <wx/panel.h>
#include <wx/toolbar.h>
class Notebook;

extern const wxEventType wxEVT_CMD_NEW_DOCKPANE;
extern const wxEventType wxEVT_CMD_DELETE_DOCKPANE;

class DockablePane : public wxPanel {
	wxWindow *m_child;
	Notebook *m_book;
	wxString m_text;
	wxBitmap m_bmp;
	
	void ClosePane(wxCommandEvent &e);
	
	void OnEraseBg(wxEraseEvent &e) { wxUnusedVar(e); }
	void OnPaint(wxPaintEvent &e);
	
	DECLARE_EVENT_TABLE();
	
public:
	DockablePane(wxWindow *parent, Notebook *book, wxWindow *child, const wxString &title, const wxBitmap &bmp = wxNullBitmap, wxSize size = wxDefaultSize);
	virtual ~DockablePane();
	wxString GetName() {return m_text;}
    Notebook *GetBook() {return m_book;}
};
#endif // __dockablepane__
