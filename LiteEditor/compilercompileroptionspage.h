#ifndef __compilercompileroptionspage__
#define __compilercompileroptionspage__

/**
@file
Subclass of CompilerCompilerOptionsBase, which is generated by wxFormBuilder.
*/

#include "compiler_pages.h"
#include "compiler.h"
#include "advanced_settings.h"

/** Implementing CompilerCompilerOptionsBase */
class CompilerCompilerOptionsPage : public CompilerCompilerOptionsBase, public ICompilerSubPage
{
	wxString m_cmpname;
	long m_selectedCmpOption;
public:
	/** Constructor */
	CompilerCompilerOptionsPage( wxWindow* parent, const wxString &cmpname );
	virtual void Save(CompilerPtr cmp);

protected:
	// Virtual event handlers from wxFormBuilder base class
	virtual void OnCompilerOptionActivated( wxListEvent& event );
	virtual void OnCompilerOptionDeSelected( wxListEvent& event );
	virtual void OnCompilerOptionSelected( wxListEvent& event );
	virtual void OnNewCompilerOption( wxCommandEvent& event );
	virtual void OnDeleteCompilerOption( wxCommandEvent& event );
};

/** Implementing CompilerOptionDlgBase */
class CompilerOptionDialog : public CompilerOptionDlgBase
{
public:
	CompilerOptionDialog(wxWindow* parent, const wxString& title, const wxString& name, const wxString& help, wxWindowID id = wxID_ANY)
	: CompilerOptionDlgBase(parent, id, title)
	{
		m_sName = name;
		m_sHelp = help;
	}
};

class CompilerCompilerOptionDialog : public CompilerOptionDialog
{
public:
	CompilerCompilerOptionDialog(wxWindow* parent, const wxString& name, const wxString& help)
	: CompilerOptionDialog(parent, wxT("Compiler option"), name, help)
	{
	}
};

#endif // __compilercompileroptionspage__