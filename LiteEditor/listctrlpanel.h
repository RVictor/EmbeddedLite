//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : listctrlpanel.h
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
 #ifndef __listctrlpanel__
#define __listctrlpanel__

/**
@file
Subclass of ListCtrlPanelBase, which is generated by wxFormBuilder.
*/

#include "listctrlpanelbase.h"
#include "debugger.h"

/** Implementing ListCtrlPanelBase */
class ListCtrlPanel : public ListCtrlPanelBase
{
protected:
	// Handlers for ListCtrlPanelBase events.
	void OnItemActivated( wxListEvent& event );
	void OnItemRightClicked(wxListEvent& event);
	void OnCopyBacktrace(wxCommandEvent &event);
	void SetColumnText ( long indx, long column, const wxString &rText );
	wxString GetColumnText(long index, long column);

public:
	/** Constructor */
	ListCtrlPanel( wxWindow* parent );
	void Update(const StackEntryArray &stackArr);
	void Clear();
	void SetCurrentLevel(const int level);

private:
	int m_currLevel;
};

#endif // __listctrlpanel__
