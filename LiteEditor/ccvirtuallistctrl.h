//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : ccvirtuallistctrl.h              
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
 #ifndef __ccvirtuallistctrl__
#define __ccvirtuallistctrl__

#include <wx/listctrl.h>
#include <vector>

struct CCItemInfo {
	int imgId;
	wxString displayName;
	
	void Reset() {
		imgId = wxNOT_FOUND;
		displayName = wxEmptyString;
	}
};

class CCVirtualListCtrl : public wxListView {
	std::vector<CCItemInfo> m_tags;
public:
	CCVirtualListCtrl( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 486,300 ), long style = wxTAB_TRAVERSAL );
	~CCVirtualListCtrl();

public:

	void SetItems(const std::vector<CCItemInfo> &tags) { m_tags = tags; }
	
	virtual wxListItemAttr* OnGetItemAttr(long item) const;
	virtual int OnGetItemColumnImage(long item, long column) const;
	virtual int OnGetItemImage(long item) const;
	virtual wxString OnGetItemText(long item, long column) const;
	int FindMatch(const wxString &word);
	
};
#endif // __ccvirtuallistctrl__
