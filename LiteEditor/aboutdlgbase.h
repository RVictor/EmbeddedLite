///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  5 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __aboutdlgbase__
#define __aboutdlgbase__

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/html/htmlwin.h>
#include <wx/notebook.h>
#include <wx/stattext.h>
#include <wx/hyperlink.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class AboutDlgBase
///////////////////////////////////////////////////////////////////////////////
class AboutDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebook1;
		wxPanel* m_panel1;
		wxStaticBitmap* m_bitmap;
		wxPanel* m_panel4;
		wxHtmlWindow* m_htmlWin3;
		wxStaticText* m_staticTextHomePage;
		wxHyperlinkCtrl* m_hyperlink1;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticTextInformation;
		wxButton* m_buttonOk;
	
	public:
		
		AboutDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About CodeLite"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 438,423 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~AboutDlgBase();
	
};

#endif //__aboutdlgbase__
