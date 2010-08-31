///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Apr 16 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __tabgroupbasedlgs__
#define __tabgroupbasedlgs__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/checkbox.h>
#include <wx/statline.h>
#include <wx/dialog.h>
#include <wx/checklst.h>
#include <wx/textctrl.h>
#include <wx/combobox.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class LoadTabGroupBaseDlg
///////////////////////////////////////////////////////////////////////////////
class LoadTabGroupBaseDlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxButton* m_buttonBrowse;
		wxListBox* m_listBox;
		wxCheckBox* m_replaceCheck;
		wxStaticLine* m_staticline3;
		wxButton* m_button6;
		wxButton* m_button5;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnBrowse( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnItemActivated( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		LoadTabGroupBaseDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Load a tab group"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~LoadTabGroupBaseDlg();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SaveTabGroupBaseDlg
///////////////////////////////////////////////////////////////////////////////
class SaveTabGroupBaseDlg : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxCheckListBox* m_ListTabs;
		wxButton* m_buttonCheckAll;
		wxButton* m_buttonClearAll;
		wxStaticLine* m_staticline1;
		wxStaticText* m_staticText4;
		wxTextCtrl* m_textName;
		wxStaticText* m_staticText3;
		wxComboBox* m_comboPath;
		wxButton* m_buttonBrowse;
		wxStaticLine* m_staticline11;
		wxButton* m_button6;
		wxButton* m_button5;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCheckAll( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckAllUpdateUI( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void OnClearAll( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnClearAllUpdateUI( wxUpdateUIEvent& event ){ event.Skip(); }
		virtual void OnBrowse( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		SaveTabGroupBaseDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Save a tab group"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~SaveTabGroupBaseDlg();
	
};

#endif //__tabgroupbasedlgs__
