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
#ifndef __stringaccessor__
#define __stringaccessor__

#include <wx/string.h>

class StringAccessor {
	wxString m_str;
	
public:
	StringAccessor(const wxString &str);
	virtual ~StringAccessor();
	
	size_t length() { return m_str.size(); }
	char safeAt(size_t pos);
	bool isWordChar(char ch);
	
	void setStr(const wxString& str) {this->m_str = str;}
	const wxString& getStr() const {return m_str;}
	bool match(const char *str, size_t from);
};
#endif // __stringaccessor__
