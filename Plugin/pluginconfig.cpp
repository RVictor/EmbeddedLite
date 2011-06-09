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
#include "pluginconfig.h"
PluginConfig* PluginConfig::ms_instance = 0;

PluginConfig::PluginConfig()
{
}

PluginConfig::~PluginConfig()
{
}

PluginConfig* PluginConfig::Instance()
{
	if(ms_instance == 0){
		ms_instance = new PluginConfig();
	}
	return ms_instance;
}

void PluginConfig::Release()
{
	if(ms_instance){
		delete ms_instance;
	}
	ms_instance = 0;
}

