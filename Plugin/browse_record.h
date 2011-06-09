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
#ifndef BROWSE_HISTORY_H
#define BROWSE_HISTORY_H

#include "wx/string.h"

class BrowseRecord
{
public:
	wxString filename;
	wxString project;
	int lineno;
	int position;
public:
	BrowseRecord() : filename(wxEmptyString), project(wxEmptyString), lineno(wxNOT_FOUND), position(wxNOT_FOUND) {}

	~BrowseRecord() {}
};

#endif //BROWSE_HISTORY_H
