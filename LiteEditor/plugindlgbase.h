///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Mar 19 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __plugindlgbase__
#define __plugindlgbase__

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/checklst.h>
#include <wx/html/htmlwin.h>
#include <wx/statline.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PluginMgrDlgBase
///////////////////////////////////////////////////////////////////////////////
class PluginMgrDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxCheckListBox* m_checkListPluginsList;
		wxStaticText* m_staticText2;
		wxHtmlWindow* m_htmlWinDesc;
		wxStaticLine* m_staticline1;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnItemSelected( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		PluginMgrDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Manage Plugins:"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 672,425 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~PluginMgrDlgBase();
	
};

#endif //__plugindlgbase__
