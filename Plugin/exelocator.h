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
#ifndef ITEMLOCATOR_H
#define ITEMLOCATOR_H

#include "wx/string.h"

class ExeLocator 
{
public:
	ExeLocator (){}
	~ExeLocator (){}

	static bool Locate(const wxString &name, wxString &where);
};

#endif //ITEMLOCATOR_H


