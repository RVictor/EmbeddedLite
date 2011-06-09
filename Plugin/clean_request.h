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
#ifndef CLEAN_REQUEST_H
#define CLEAN_REQUEST_H
#include "shell_command.h"

/**
 * \class CleanRequest
 * \brief
 * \author Eran
 * \date 07/22/07
 */
class CleanRequest : public ShellCommand {
public:
	/**
	 * Construct a compilation clean request. The compiler thread will clean the selected project and all
	 * its dependencies as appeard in the build order dialog
	 * \param projectName the selected project to build
	 */
	CleanRequest(wxEvtHandler *owner, const QueueCommand &info);

	///dtor
	virtual ~CleanRequest();

	//process the request
	virtual void Process(IManager *manager = NULL);

	//setters/getters
	const wxString &GetProjectName() const { return m_info.GetProject(); }
};

#endif // CLEAN_REQUEST_H
