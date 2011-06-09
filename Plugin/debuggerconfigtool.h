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
#ifndef DEBUGGERCONFIGTOOL_H
#define DEBUGGERCONFIGTOOL_H

#include "configtool.h"

class DebuggerConfigTool : public ConfigTool {
private:
	DebuggerConfigTool();
public:	
	~DebuggerConfigTool();
	static DebuggerConfigTool *Get();
};
#endif //DEBUGGERCONFIGTOOL_H
