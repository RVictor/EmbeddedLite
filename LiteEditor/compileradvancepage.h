#ifndef __compileradvancepage__
#define __compileradvancepage__

/**
@file
Subclass of CompilerAdvanceBase, which is generated by wxFormBuilder.
*/

#include "compiler_pages.h"
#include "advanced_settings.h"
#include "compiler.h"

/** Implementing CompilerAdvanceBase */
class CompilerAdvancePage : public CompilerAdvanceBase, public ICompilerSubPage
{
	wxString m_cmpname;
public:
	/** Constructor */
	CompilerAdvancePage( wxWindow* parent, const wxString &cmpname );
	virtual void Save(CompilerPtr cmp);
};

#endif // __compileradvancepage__
