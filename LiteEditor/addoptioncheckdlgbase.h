///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug  4 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __addoptioncheckdlgbase__
#define __addoptioncheckdlgbase__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/checklst.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class AddOptionCheckDialogBase
///////////////////////////////////////////////////////////////////////////////
class AddOptionCheckDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxCheckListBox* m_checkListOptions;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textOptions;
		wxTextCtrl* m_textCmdLn;
		wxStaticLine* m_staticline5;
		wxStdDialogButtonSizer* m_sdbSizer;
		wxButton* m_sdbSizerOK;
		wxButton* m_sdbSizerCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOptionToggled( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOptionsText( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		AddOptionCheckDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,450 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~AddOptionCheckDialogBase();
	
};

#endif //__addoptioncheckdlgbase__
