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
#include "detachedpanesinfo.h"
DetachedPanesInfo::DetachedPanesInfo(wxArrayString arr)
: m_panes(arr)
{
}

DetachedPanesInfo::~DetachedPanesInfo()
{
}

void DetachedPanesInfo::DeSerialize(Archive& arch)
{
	arch.Read(wxT("m_panes"), m_panes);
}

void DetachedPanesInfo::Serialize(Archive& arch)
{
	arch.Write(wxT("m_panes"), m_panes);
}

