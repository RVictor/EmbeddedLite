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
#ifndef __dynamiclibrary__
#define __dynamiclibrary__

#include "wx/dynlib.h"
#include "wx/string.h"

class clDynamicLibrary {

#if defined (__WXMSW__)
	wxDynamicLibrary m_lib;
#else //Mac OSX
	void *m_dllhandle;
#endif
    wxString m_error;
    
public:
	clDynamicLibrary();
	~clDynamicLibrary();
	
	bool Load(const wxString &name);
	void Detach();
	void* GetSymbol(const wxString &name, bool *success);
    
    wxString GetError() {return m_error;}
};
#endif // __dynamiclibrary__
