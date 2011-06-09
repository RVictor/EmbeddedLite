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
#ifndef DIR_SAVER_H
#define DIR_SAVER_H

#include "wx/string.h"
#include "wx/filefn.h"

#ifdef WXMAKINGDLL_CODELITE
#    define WXDLLIMPEXP_CL WXEXPORT
#elif defined(WXUSINGDLL_CODELITE)
#    define WXDLLIMPEXP_CL WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_CL
#endif // WXMAKINGDLL_CODELITE

// Utility class that helps keeping the current directory
class WXDLLIMPEXP_CL DirSaver 
{
	wxString m_curDir;
public:
	DirSaver(){
		m_curDir = wxGetCwd();
	}

	virtual ~DirSaver(){
		wxSetWorkingDirectory(m_curDir);
	}
};
#endif // DIR_SAVER_H
