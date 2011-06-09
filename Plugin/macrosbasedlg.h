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
#ifndef __macrosbasedlg__
#define __macrosbasedlg__

#include <wx/listctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class MacrosBaseDlg
///////////////////////////////////////////////////////////////////////////////
class MacrosBaseDlg : public wxDialog 
{
	private:
	
	protected:
		wxListCtrl* m_listCtrlMacros;
		wxStaticLine* m_staticline2;
		wxButton* m_buttonOk;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnItemRightClick( wxListEvent& event ){ event.Skip(); }
		
	
	public:
		MacrosBaseDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Available Macros:"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 646,507 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~MacrosBaseDlg();
	
};

#endif //__macrosbasedlg__
