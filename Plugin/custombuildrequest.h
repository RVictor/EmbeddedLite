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
#ifndef __custombuildrequest__
#define __custombuildrequest__

#include "shell_command.h"
#include "build_config.h"

class CustomBuildRequest : public ShellCommand
{
	wxString m_fileName;

protected:
	void DoUpdateCommand(IManager *manager, wxString &cmd, ProjectPtr proj, BuildConfigPtr bldConf);

public:
	CustomBuildRequest(wxEvtHandler *owner, const QueueCommand &buildInfo, const wxString &fileName);
	virtual ~CustomBuildRequest();

public:
	//process the request
	virtual void Process(IManager *manager = NULL);

	//setters/getters
	const wxString &GetProjectName() const { return m_info.GetProject(); }
};
#endif // __custombuildrequest__
