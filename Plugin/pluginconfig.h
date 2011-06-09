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
#ifndef __pluginconfig__
#define __pluginconfig__
#include "configtool.h"

class PluginConfig : public ConfigTool {

	static PluginConfig* ms_instance;

public:
	static PluginConfig* Instance();
	static void Release();

private:
	PluginConfig();
	virtual ~PluginConfig();

};
#endif // __pluginconfig__
