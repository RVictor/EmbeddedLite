#ifndef __renamefiledlg__
#define __renamefiledlg__

/**
@file
Subclass of RenameFileBaseDlg, which is generated by wxFormBuilder.
*/

#include "renamefilebasedlg.h"
#include "include_finder.h"
#include <map>

/** Implementing RenameFileBaseDlg */
class RenameFileDlg : public RenameFileBaseDlg
{
	std::map<int, IncludeStatement> m_entries;

protected:
	// Handlers for RenameFileBaseDlg events.
	void OnFileSelected( wxCommandEvent& event );
	void DoSelectItem( int idx );

public:
	/** Constructor */
	RenameFileDlg( wxWindow* parent, const wxString &replaceWith, std::vector<IncludeStatement> &matches );
	virtual ~RenameFileDlg();
	std::vector<IncludeStatement> GetMatches()     const;
	wxString                      GetReplaceWith() const;
};

#endif // __renamefiledlg__
