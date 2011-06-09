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
#ifndef EMBEDDEDLITE_COMMENT_H
#define EMBEDDEDLITE_COMMENT_H

#include <wx/string.h>
#include "smart_ptr.h"

class Comment
{
	wxString m_comment;
	wxString m_file;
	int      m_line;

public:
	/**
	 * construct a Comment object
	 * \param comment comment string
	 * \param file comment file name
	 * \param line comment line number
	 */
	Comment(const wxString &comment, const wxString & file, const int line);

	/**
	 * Copy constructor
	 */
	Comment(const Comment& rhs);

	/**
	 * Destructor
	 */
	virtual ~Comment() {};

	/// asgginment operator
	Comment& operator=(const Comment& rhs);

	/// accessors
	/**
	 * Get the file name
	 * \return file name
	 */
	const wxString & GetFile() const {
		return m_file;
	}

	/**
	 * Get string comment
	 * \return comment
	 */
	const wxString & GetComment() const {
		return m_comment;
	}

	/**
	 * Get the line number
	 * \return line number
	 */
	const int & GetLine() const {
		return m_line;
	}
};

typedef SmartPtr<Comment> CommentPtr;

#endif // EMBEDDEDLITE_COMMENT_H
