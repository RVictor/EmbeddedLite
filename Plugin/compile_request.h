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
#ifndef COMPILE_REQUEST_H
#define COMPILE_REQUEST_H

#include "shell_command.h"

class CompileRequest : public ShellCommand {
	wxString m_fileName;
	bool     m_premakeOnly;
    bool     m_preprocessOnly;

public:
	/**
	 * Construct a compilation request. The compiler thread will build the selected project and all
	 * its dependencies as appeard in the build order dialog
	 * \param projectName the selected project to build
	 * \param configurationName the workspace selected configuration
	 */
	CompileRequest(	wxEvtHandler *owner, const QueueCommand &buildInfo,
					const wxString &fileName = wxEmptyString,
					bool runPremakeOnly = false,
                    bool preprocessOnly = false
					);

	///dtor
	virtual ~CompileRequest();

	//process the request
	virtual void Process(IManager *manager = NULL);

	//setters/getters
	const wxString &GetProjectName() const { return m_info.GetProject(); }
};

#endif // COMPILE_REQUEST_H
