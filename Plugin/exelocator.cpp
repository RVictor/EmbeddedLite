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
#include "exelocator.h"
#include <wx/filename.h>
#include "procutils.h"

bool ExeLocator::Locate(const wxString &name, wxString &where)
{
	wxString command;

	// Incase the name is a full path, just test for the file existance
	wxFileName fn(name);
	if( fn.IsAbsolute() && fn.FileExists() ) {
		where = name;
		return true;
	}

	wxArrayString output;
	command << wxT("which \"") << name << wxT("\"");
	ProcUtils::SafeExecuteCommand(command, output);

	if(output.IsEmpty() == false){
		wxString interstingLine = output.Item(0);

		if(interstingLine.Trim().Trim(false).IsEmpty()){
			return false;
		}

		if(	!interstingLine.StartsWith(wxT("which: no ")) &&
			!interstingLine.Contains(wxT("command not found")) &&
			!interstingLine.StartsWith(wxT("no "))){
			where = output.Item(0);
			where = where.Trim().Trim(false);
			return true;
		}
	}
	return false;
}
