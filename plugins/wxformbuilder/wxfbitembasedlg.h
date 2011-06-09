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
#ifndef __wxfbitembasedlg__
#define __wxfbitembasedlg__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class wxFBItemBaseDlg
///////////////////////////////////////////////////////////////////////////////
class wxFBItemBaseDlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxTextCtrl* m_textCtrlClassName;
		
		wxStaticText* m_staticText2;
		wxTextCtrl* m_textCtrlTitle;
		
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textCtrlVD;
		wxButton* m_buttonBrowseVD;
		wxStaticLine* m_staticline1;
		wxButton* m_buttonGenerate;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBrowseVD( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnGenerate( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnGenerateUI( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		wxFBItemBaseDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 513,182 ), long style = wxDEFAULT_DIALOG_STYLE );
		~wxFBItemBaseDlg();
	
};

#endif //__wxfbitembasedlg__
