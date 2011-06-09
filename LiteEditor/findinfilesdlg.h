/**
  \file 

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef FIND_IN_FILES_DLG_H
#define FIND_IN_FILES_DLG_H

#include "findinfilesdlgbase.h"
#include "findreplacedlg.h"
#include "search_thread.h"

class FindInFilesDialog: public FindInFilesDialogBase
{
	FindReplaceData m_data;

	void       DoSearch();
	void       DoSearchReplace();
	void       DoSaveSearchPaths();
	SearchData DoGetSearchData();
	void       DoSaveOpenFiles();

	virtual void OnClick(wxCommandEvent &event);
	virtual void OnClose(wxCloseEvent &event);
	virtual void OnAddPath( wxCommandEvent& event );
	virtual void OnRemovePath( wxCommandEvent& event );
	virtual void OnClearPaths( wxCommandEvent& event );
	virtual void OnClearPathsUI( wxUpdateUIEvent& event );
	virtual void OnRemovePathUI( wxUpdateUIEvent& event );
	void OnCharEvent(wxKeyEvent &event);

	virtual void OnFindWhatUI( wxUpdateUIEvent& event );

public:
	FindInFilesDialog(wxWindow* parent, wxWindowID id, const FindReplaceData& data);
	virtual ~FindInFilesDialog();

	void SetSearchData(const SearchData &data);
	void SetRootDir   (const wxString &rootDir);

	FindReplaceData& GetData() {
		return m_data;
	}

	virtual bool Show();
};

#endif // FIND_IN_FILES_DLG_H
