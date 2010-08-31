//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : dockablepanemenumanager.cpp              
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

#include <wx/control.h>
#include <wx/aui/aui.h>
#include <wx/app.h>
#include <wx/xrc/xmlres.h>
#include "dockablepanemenumanager.h"
#include <wx/log.h>

//const wxEventType wxEVT_CMD_HIDE_PANE = wxNewEventType();
//const wxEventType wxEVT_CMD_SHOW_PANE = wxNewEventType();

DockablePaneMenuManager::DockablePaneMenuManager(wxMenuBar* mb, wxAuiManager *aui)
		: m_mb(mb)
		, m_aui(aui)
{
}

DockablePaneMenuManager::~DockablePaneMenuManager()
{
}

void DockablePaneMenuManager::RemoveMenu(const wxString& name)
{
    int itemId = wxXmlResource::GetXRCID(name.c_str());
    std::map<int, wxString>::iterator iter = m_id2nameMap.find(itemId);
    if (iter != m_id2nameMap.end()) {
        m_id2nameMap.erase(iter);
    }
#if 0
	int idx = m_mb->FindMenu(wxT("View"));
	if (idx != wxNOT_FOUND) {
		wxMenu *menu = m_mb->GetMenu(idx);
		menu->Destroy(itemId);

		wxTheApp->Disconnect(itemId, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(DockablePaneMenuManager::OnDockpaneMenuItem), NULL, this);
		wxTheApp->Disconnect(itemId, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(DockablePaneMenuManager::OnDockpaneMenuItemUI), NULL, this);
	}
#endif
}

void DockablePaneMenuManager::AddMenu(const wxString& name)
{
    int itemId = wxXmlResource::GetXRCID(name.c_str());
    m_id2nameMap[itemId] = name;
#if 0
	int idx = m_mb->FindMenu(wxT("View"));
	if (idx != wxNOT_FOUND) {
		wxMenu *menu = m_mb->GetMenu(idx);

		wxMenuItem *item = new wxMenuItem(menu, itemId, name, wxEmptyString, wxITEM_CHECK);
		menu->Append(item);
		item->Check(true);

		wxTheApp->Connect(itemId, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(DockablePaneMenuManager::OnDockpaneMenuItem), NULL, this);
		wxTheApp->Connect(itemId, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(DockablePaneMenuManager::OnDockpaneMenuItemUI), NULL, this);
	}
#endif
}

wxString DockablePaneMenuManager::NameById(int id)
{
	std::map<int, wxString>::iterator iter = m_id2nameMap.find(id);
	if (iter != m_id2nameMap.end()) {
		return iter->second;
	}
	return wxEmptyString;
}

void DockablePaneMenuManager::OnDockpaneMenuItem(wxCommandEvent& e)
{
	wxString name = NameById(e.GetId());
	wxAuiPaneInfo &info = m_aui->GetPane(name);
	if ( info.IsOk() ) {
		if (e.IsChecked()) {
			// need to show the item
			info.Show();
		} else {
			info.Hide();
		}
		m_aui->Update();
	}
}

void DockablePaneMenuManager::OnDockpaneMenuItemUI(wxUpdateUIEvent& e)
{
	wxString name = NameById(e.GetId());
	wxAuiPaneInfo &info = m_aui->GetPane(name);
	if (info.IsOk()) {
		if ( info.IsShown() ) {
			e.Check(true);
		} else {
			e.Check(false);
		}
	}
}

wxArrayString DockablePaneMenuManager::GetDeatchedPanesList()
{
	wxArrayString arr;
	std::map<int, wxString>::iterator iter = m_id2nameMap.begin();
	for(; iter != m_id2nameMap.end(); iter++){
		arr.Add(iter->second);
	}
	return arr;
}

bool DockablePaneMenuManager::IsPaneDetached(const wxString& name)
{
	std::map<int, wxString>::iterator iter = m_id2nameMap.begin();
	for(; iter != m_id2nameMap.end(); iter++){
		if(iter->second == name) {
			return true;
		}
	}
	return false;
}
