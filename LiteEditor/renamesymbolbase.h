//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : renamesymbolbase.h              
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

///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __renamesymbolbase__
#define __renamesymbolbase__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/checklst.h>
#include "cl_editor.h"
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class RenameSymbolBase
///////////////////////////////////////////////////////////////////////////////
class RenameSymbolBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText31;
		wxTextCtrl* m_textCtrlNewName;
		wxStaticText* m_staticText1;
		wxCheckListBox* m_checkListCandidates;
		LEditor *m_preview;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnItemSelected( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnItemDClicked( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnItemChecked( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		RenameSymbolBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Rename Symbol"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 895,579 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~RenameSymbolBase();
	
};

#endif //__renamesymbolbase__
