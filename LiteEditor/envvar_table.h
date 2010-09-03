#ifndef __envvar_table__
#define __envvar_table__

/**
@file
Subclass of EnvVarsTableDlgBase, which is generated by wxFormBuilder.
*/

#include "env_var_dlg_base.h"

/** Implementing EnvVarsTableDlgBase */
class EnvVarsTableDlg : public EnvVarsTableDlgBase
{
protected:
	void DoAddPage(const wxString &name, const wxString &content, bool select);

protected:
	// Handlers for EnvVarsTableDlgBase events.
	void OnLeftUp( wxMouseEvent& event );
	void OnButtonOk( wxCommandEvent& event );
	void OnNewSet( wxCommandEvent& event );
	void OnDeleteSet( wxCommandEvent& event );
	void OnDeleteSetUI( wxUpdateUIEvent& event );

public:
	/** Constructor */
	EnvVarsTableDlg( wxWindow* parent );
	virtual ~EnvVarsTableDlg();
};

#endif // __envvar_table__