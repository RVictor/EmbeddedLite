/**
  \file macromanager.h

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef MACROMANAGER_H
#define MACROMANAGER_H

#include "imacromanager.h" // Base class: IMacroManager
#include <wx/string.h>

class IManager;
class MacroManager : public IMacroManager {

	static MacroManager* ms_instance;

public:
	static MacroManager* Instance();
	static void Release();

private:
	MacroManager();
	virtual ~MacroManager();

public:
	/*
	 * The following macro will be expanded into their real values:
	 * $(ProjectPath)
	 * $(WorkspacePath)
	 * $(ProjectName)
	 * $(IntermediateDirectory)
	 * $(ConfigurationName)
	 * $(OutDir)
	 * $(ProjectFiles)
	 * $(ProjectFiles)
	 * $(ProjectFilesAbs)
	 * $(CurrentFileName)
	 * $(CurrentFilePath)
	 * $(CurrentFileExt)
	 * $(CurrentFileFullPath)
	 * $(User)
	 * $(Date)
	 * $(EmbeddedLitePath)
	 * $(CurrentSelection)
	 * $(ProjectOutputFile)
	 */
	wxString Expand(const wxString &expression, IManager *manager, const wxString &project, const wxString &confToBuild = wxEmptyString);
};

#endif // MACROMANAGER_H
