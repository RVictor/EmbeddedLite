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
#ifndef __cppwordscanner__
#define __cppwordscanner__

#include <wx/arrstr.h>
#include "cpptoken.h"

//class TokenDb;

class CppWordScanner {
	wxString m_text;
	wxSortedArrayString m_arr;
//	TokenDb *m_db;
	wxString m_filename;
protected:
	void doFind(const wxString &filter, CppTokensMap &l);
	
public:
	CppWordScanner(const wxString &file_name);
	~CppWordScanner();
	
	void FindAll(CppTokensMap &l);
	void Match(const wxString &word, CppTokensMap &l);
//	void SetDatabase(TokenDb *db);
};
#endif // __cppwordscanner__
