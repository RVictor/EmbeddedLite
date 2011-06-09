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
#include "pluginsdata.h"
PluginsData::PluginsData()
{
}

PluginsData::~PluginsData()
{
}

void PluginsData::DeSerialize(Archive &arch)
{
	size_t size(0);
	arch.Read(wxT("count"), size);

	m_info.clear();
	wxString key(wxEmptyString);
	for (size_t i=0; i<size; i++) {
		PluginInfo info;
		key = wxEmptyString;
		key << wxT("plugin_info") << i;
		arch.Read(key, &info);
		m_info[info.GetName()] = info;
	}
}

void PluginsData::Serialize(Archive &arch)
{
	arch.Write(wxT("count"), m_info.size());
	std::map< wxString, PluginInfo >::iterator iter = m_info.begin();
	for (size_t i=0; iter != m_info.end(); iter++, i++) {
		wxString key;
		key << wxT("plugin_info") << i;
		arch.Write(key, &iter->second);
	}
}
