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
#ifndef EXTDBDATA_H
#define EXTDBDATA_H

#include <wx/arrstr.h>

struct ExtDbData {
	wxString dbName;
	wxString rootPath;
	wxArrayString includeDirs;
	bool attachDb;
	wxString fileMasking;
	bool parseFilesWithoutExtension;
};

#endif //EXTDBDATA_H
