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
#include "debuggerconfigtool.h"

DebuggerConfigTool::DebuggerConfigTool()
{
}

DebuggerConfigTool::~DebuggerConfigTool()
{
}

DebuggerConfigTool *DebuggerConfigTool::Get()
{
	static DebuggerConfigTool theTool;
	return &theTool;
}
