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
#include "plugindata.h"

PluginInfo::PluginInfo()
		: enabled(true)
{
}

PluginInfo::~PluginInfo()
{
}

void PluginInfo::DeSerialize(Archive &arch)
{
	arch.Read(wxT("enabled"), enabled);
	arch.Read(wxT("name"), name);
	arch.Read(wxT("author"), author);
	arch.Read(wxT("description"), description);
	arch.Read(wxT("version"), version);
}
void PluginInfo::Serialize(Archive &arch)
{
	arch.Write(wxT("enabled"), enabled);
	arch.Write(wxT("name"), name);
	arch.Write(wxT("author"), author);
	arch.Write(wxT("description"), description);
	arch.Write(wxT("version"), version);
}
