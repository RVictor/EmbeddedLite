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
#include "setters_getters_data.h"

SettersGettersData::SettersGettersData()
{
}


SettersGettersData::~SettersGettersData()
{
}

void SettersGettersData::Serialize(Archive &arch)
{
	arch.Write(wxT("m_flags"), m_flags);
}

void SettersGettersData::DeSerialize(Archive &arch)
{
	arch.Read(wxT("m_flags"), m_flags);
}


