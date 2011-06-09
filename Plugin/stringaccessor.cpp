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
#include "stringaccessor.h"
StringAccessor::StringAccessor(const wxString &str)
: m_str(str)
{
}

StringAccessor::~StringAccessor()
{
}

char StringAccessor::safeAt(size_t pos)
{
	if( pos >= m_str.size() ) {
		return 0;
	}
	return m_str.at(pos);
}

bool StringAccessor::isWordChar(char ch)
{
	int ascii_value = (int)ch;
	if( (ascii_value >= 48 && ascii_value <= 57)  	|| 	// 0-9
		(ascii_value >= 65 && ascii_value <= 90)  	|| 	// A-Z
		(ascii_value >= 97 && ascii_value <= 122) 	|| 	// a-z
//		(ascii_value == 126) 						||  // ~
		(ascii_value == 95))							// _
	{
		return true;
	}
	return false;
}

bool StringAccessor::match(const char* str, size_t from)
{
	size_t size = strlen(str);
	for(size_t i=0; i<size; i++){
		if(str[i] != safeAt(from + i)){
			return false;
		}
	}
	return true;
}
