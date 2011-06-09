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
#ifndef __detachedpanesinfo__
#define __detachedpanesinfo__

#include "serialized_object.h"
#include <wx/arrstr.h>

class DetachedPanesInfo : public SerializedObject
{
	wxArrayString m_panes;
public:
	DetachedPanesInfo(wxArrayString arr);
	DetachedPanesInfo(){}
	virtual ~DetachedPanesInfo();

public:
	virtual void DeSerialize(Archive &arch);
	virtual void Serialize(Archive &arch);


//Getters
	const wxArrayString& GetPanes() const {
		return m_panes;
	}

	//Setters
	void SetPanes(const wxArrayString& panes) {
		this->m_panes = panes;
	}

};
#endif // __detachedpanesinfo__
