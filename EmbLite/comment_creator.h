/**
  \file comment_creator.h 

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef COMMENT_CREATOT_H
#define COMMENT_CREATOT_H

#include "wx/string.h"

/**
 * \ingroup EmbeddedLite
 * an interface class to the comment creator. 
 *
 * \version 1.0
 * first version
 *
 * \date 07-20-2007
 *
 * \author Eran
 *
 */
class CommentCreator
{
protected:
	wxChar m_keyPrefix;
	
public:
	CommentCreator(wxChar keyPrefix = wxT('\\')) : m_keyPrefix(keyPrefix) {}
	virtual ~CommentCreator(){}
	virtual wxString CreateComment() = 0;
};
#endif //COMMENT_CREATOT_H
