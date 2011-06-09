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
#ifndef CPP_COMMENT_CREATOR_H
#define CPP_COMMENT_CREATOR_H

#include "wx/string.h"
#include "entry.h"
#include "comment_creator.h"

class CppCommentCreator : public CommentCreator
{
	TagEntryPtr m_tag;
public:
	CppCommentCreator(TagEntryPtr tag, wxChar keyPrefix);
	virtual ~CppCommentCreator();
	virtual wxString CreateComment();

private:
	wxString FunctionComment();
};

#endif //CPP_COMMENT_CREATOR_H
