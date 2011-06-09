#ifndef __breakptpropertiesdlg__
#define __breakptpropertiesdlg__

/**
@file
Subclass of BreakpointPropertiesDlg, which is generated by wxFormBuilder.
*/

#include "breakpoint_dlg_base.h"
#include "debugger.h"

/** Implementing BreakpointPropertiesDlg */
class BreakptPropertiesDlg : public BreakpointPropertiesDlg
{
protected:
	void EndModal( int retCode );
	// Handlers for BreakptPropertiesDlg events.
	void OnCheckBreakLineno( wxCommandEvent& event );
	void OnCheckBreakFunction( wxCommandEvent& event );
	void OnCheckBreakMemory( wxCommandEvent& event );
	void OnBrowse( wxCommandEvent& event );
	void OnCheckBreakLinenoUI( wxUpdateUIEvent& event );
	void OnCheckBreakFunctionUI( wxUpdateUIEvent& event );
	void OnCheckBreakMemoryUI( wxUpdateUIEvent& event );
	void OnPageChanging(wxChoicebookEvent &event);
	void OnPageChanged(wxChoicebookEvent &event);

	bool its_a_breakpt;	// Holds whether it's a break or a watch. Used for UpdateUI
	enum whichbreakchk { wbc_line, wbc_function, wbc_memory };
	enum whichbreakchk whichBreakcheck;	// Holds which of lineno, function or memory checks should be ticked. Used for UpdateUI

public:
	BreakptPropertiesDlg( wxWindow* parent );
	~BreakptPropertiesDlg();

	/**
	 * \brief Insert the data from an existing breakpoint into the dialog fields
	 * \param parent
	 * \param bp
	 */
	void EnterBPData( const BreakpointInfo &bp );

	BreakpointInfo b;	// Used to return the amended settings
};

#endif // __breakptpropertiesdlg__
