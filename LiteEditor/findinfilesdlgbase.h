///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 25 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __findinfilesdlgbase__
#define __findinfilesdlgbase__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/combobox.h>
#include "dirpicker.h"
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/checkbox.h>
#include <wx/statbox.h>
#include <wx/radiobut.h>
#include <wx/statline.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FindInFilesDialogBase
///////////////////////////////////////////////////////////////////////////////
class FindInFilesDialogBase : public wxDialog 
{
	private:
	
	protected:
		wxComboBox* m_findString;
		DirPicker  *m_dirPicker;
		wxButton* m_btnAddPath;
		wxListBox* m_listPaths;
		wxButton* m_btnRemovelPath;
		wxButton* m_btnClearPaths;
		wxComboBox* m_fileTypes;
		wxCheckBox* m_matchCase;
		wxCheckBox* m_matchWholeWord;
		wxCheckBox* m_regualrExpression;
		wxCheckBox* m_fontEncoding;
		wxCheckBox* m_printScope;
		wxCheckBox* m_checkBoxSaveFilesBeforeSearching;
		wxRadioButton* m_resInNewTab;
		
		wxRadioButton* m_resInActiveTab;
		
		wxStaticLine* m_staticline1;
		wxButton* m_find;
		wxButton* m_replaceAll;
		wxButton* m_stop;
		wxButton* m_cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnAddPath( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemovePath( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemovePathUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnClearPaths( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClearPathsUI( wxUpdateUIEvent& event ) { event.Skip(); }
		virtual void OnClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFindWhatUI( wxUpdateUIEvent& event ) { event.Skip(); }
		
	
	public:
		
		FindInFilesDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Find in files"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~FindInFilesDialogBase();
	
};

#endif //__findinfilesdlgbase__
