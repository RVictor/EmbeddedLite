//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : memoryviewbase.h              
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

#ifndef __memoryviewbase__
#define __memoryviewbase__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MemoryViewBase
///////////////////////////////////////////////////////////////////////////////
class MemoryViewBase : public wxPanel 
{
	private:
	
	protected:
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrlExpression;
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrlSize;
		wxStaticLine* m_staticline1;
		wxTextCtrl* m_textCtrlMemory;
		wxButton* m_buttonEvaluate;
		wxButton* m_buttonUpdate;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnTextEntered( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextDClick( wxMouseEvent& event ){ event.Skip(); }
		virtual void OnEvaluate( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnEvaluateUI( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void OnUpdate( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		MemoryViewBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 596,300 ), long style = wxTAB_TRAVERSAL );
		~MemoryViewBase();
	
};

#endif //__memoryviewbase__
