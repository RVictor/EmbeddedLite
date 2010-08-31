#ifndef __localstable__
#define __localstable__

#include "simpletablebase.h"
#include <wx/arrstr.h>
#include "debuggerobserver.h"
#include "debuggersettings.h"

class LocalsTable : public LocalsTableBase {

	std::vector<DebuggerCmdData> m_dbgCmds;
	std::map<wxString, long>     m_expression2Idx;
public:
	static wxString GetRealType (const wxString &gdbType);

protected:
	void     DoShowDetails (long item);
	long     DoGetIdxByName(const wxString      &name);
	long     DoGetIdxByVar (const LocalVariable &var, const wxString &kind);
	bool     DoShowInline  (const LocalVariable &var, long item);

public:
	LocalsTable(wxWindow *parent);
	virtual ~LocalsTable();

public:
	virtual void OnItemActivated( wxListEvent& event );
	virtual void OnItemSelected( wxListEvent& event );

	void UpdateLocals  (const LocalVariables &locals);
	void UpdateFuncArgs(const LocalVariables &args  );
	void UpdateInline  (const DebuggerEvent &event  );
	void Clear         ();
	void Initialize    ();
};
#endif // __localstable__
