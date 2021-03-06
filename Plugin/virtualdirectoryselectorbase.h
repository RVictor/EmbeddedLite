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
#ifndef __virtualdirectoryselectorbase__
#define __virtualdirectoryselectorbase__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/treectrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class VirtualDirectorySelectorBase
///////////////////////////////////////////////////////////////////////////////
class VirtualDirectorySelectorBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxTreeCtrl* m_treeCtrl;
		wxStaticText* m_staticTextPreview;
		wxStaticLine* m_staticline1;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnItemSelected( wxTreeEvent& event ){ event.Skip(); }
		virtual void OnButtonOK( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonOkUI( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void OnButtonCancel( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		VirtualDirectorySelectorBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Virtual Directory Selector"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~VirtualDirectorySelectorBase();
	
};

#endif //__virtualdirectoryselectorbase__
