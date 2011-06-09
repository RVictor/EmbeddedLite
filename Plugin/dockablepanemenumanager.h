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
#ifndef __dockablepanemenumanager__
#define __dockablepanemenumanager__

#include <wx/menu.h>
#include <map>

class wxAuiManager;

//extern const wxEventType wxEVT_CMD_HIDE_PANE;
//extern const wxEventType wxEVT_CMD_SHOW_PANE;
//
class DockablePaneMenuManager : public wxEvtHandler {
	wxMenuBar *m_mb;
	std::map<int, wxString> m_id2nameMap;
	wxAuiManager *m_aui;
	
	void OnDockpaneMenuItem(wxCommandEvent &e);
	void OnDockpaneMenuItemUI(wxUpdateUIEvent &e);
	
public:
	DockablePaneMenuManager(wxMenuBar *mb, wxAuiManager *aui);
	virtual ~DockablePaneMenuManager();
	
	void AddMenu(const wxString &name);
	void RemoveMenu(const wxString &name);
	
	wxString NameById(int id);
	wxArrayString GetDeatchedPanesList();
	bool IsPaneDetached(const wxString &name);
};
#endif // __dockablepanemenumanager__
