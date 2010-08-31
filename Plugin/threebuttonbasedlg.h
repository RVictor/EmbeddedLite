//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : threebuttonbasedlg.h              
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
// C++ code generated with wxFormBuilder (version Feb 20 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __threebuttonbasedlg__
#define __threebuttonbasedlg__

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ThreeButtonBaseDlg
///////////////////////////////////////////////////////////////////////////////
class ThreeButtonBaseDlg : public wxDialog 
{
	private:
	
	protected:
		wxPanel* m_mainPanel;
		wxStaticBitmap* m_bitmap;
		wxStaticText* m_message;
		wxCheckBox* m_checkBoxRememberMyAnwer;
		wxStaticLine* m_staticline;
		wxButton* m_buttonYes;
		wxButton* m_buttonNo;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonYes( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonNo( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonCancel( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		ThreeButtonBaseDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 399,147 ), long style = wxDEFAULT_DIALOG_STYLE );
		~ThreeButtonBaseDlg();
	
};

#endif //__threebuttonbasedlg__
