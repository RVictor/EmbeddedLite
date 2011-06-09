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
#ifndef CONF_OBJECT_H
#define CONF_OBJECT_H

#include "wx/xml/xml.h"
#include "smart_ptr.h"

#ifdef WXMAKINGDLL_LE_SDK
#    define WXDLLIMPEXP_LE_SDK WXEXPORT
#elif defined(WXUSINGDLL_LE_SDK)
#    define WXDLLIMPEXP_LE_SDK WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_LE_SDK
#endif // WXMAKINGDLL_LE_SDK

class wxXmlNode;

/**
 * Interface of configuration objects
 */
class WXDLLIMPEXP_LE_SDK ConfObject {
public:
	ConfObject(){};
	virtual ~ConfObject(){}

	virtual wxXmlNode *ToXml() const = 0;
};

typedef SmartPtr<ConfObject> ConfObjectPtr;
#endif // CONFIGURATION_OBJECT_H
