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
/*
* This file is part of the Mingw32 package.
*
* unistd.h maps (roughly) to io.h
*/

#ifndef __STRICT_ANSI__
#ifdef __WXGTK__
#include "/usr/include/unistd.h"
#elif defined(__WXMAC__)
#include "/usr/include/unistd.h"
#else
#include <io.h>
#include <process.h>
#endif
#endif 
