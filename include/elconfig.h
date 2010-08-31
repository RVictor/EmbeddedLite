/**
  \file elconfig.h

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef _ELCONFIG_H_
#define _ELCONFIG_H_

#include "elversion.h"

#define EL_PROJECT_EXT            wxT("emlprj")
#define EL_WORKSPACE_EXT          wxT("emlwsp")

  /**
    Returns application version as a string.
    \return application version as a string.
  */
  wxString  AppGetVersion(bool bIncludeBuild=true);


#endif // _ELCONFIG_H_

