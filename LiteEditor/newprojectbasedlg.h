///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 25 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __newprojectbasedlg__
#define __newprojectbasedlg__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/listctrl.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class NewProjectBaseDlg
///////////////////////////////////////////////////////////////////////////////
class NewProjectBaseDlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText2;
		wxChoice* m_chCategories;
		wxStaticText* m_staticText3;
		wxListCtrl* m_listTemplates;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_txtProjName;
		wxStaticText* m_staticText5;
		wxTextCtrl* m_textCtrlProjectPath;
		wxButton* m_button1;
		wxStaticText* m_staticText6;
		wxChoice* m_chCompiler;
		wxCheckBox* m_cbSeparateDir;
		wxStaticText* m_stxtFullFileName;
		wxTextCtrl* m_txtDescription;
		wxStaticLine* m_staticline;
		wxButton* m_button2;
		wxButton* m_button3;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCategorySelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTemplateSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnProjectNameChanged( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnProjectPathUpdated( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBrowseProjectPath( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCreate( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		NewProjectBaseDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("New project"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~NewProjectBaseDlg();
	
};

#endif //__newprojectbasedlg__
