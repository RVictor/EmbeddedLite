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
#include "precompiled_header.h"
#include "comment.h"
#include <iostream>

Comment::Comment(const Comment& rhs)
{
	*this = rhs;
}

Comment::Comment(const wxString & comment, const wxString & file, const int line)
		: m_comment( comment )
		, m_file( file )
		, m_line( line )
{
	m_comment.erase(m_comment.find_last_not_of(_T("\n\r\t"))+1);
}

Comment & Comment::operator=(const Comment &rhs)
{
	if ( this == &rhs )
		return *this;

	m_comment = rhs.m_comment;
	m_file    = rhs.m_file;
	m_line    = rhs.m_line;
	return *this;
}
