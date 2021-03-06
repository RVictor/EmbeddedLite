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
#include "environmentconfig.h"
#include "processreaderthread.h"
#include <wx/xrc/xmlres.h>
#include "globals.h"
#include "workspace.h"
#include "shell_command.h"
#include "wx/tokenzr.h"
#include "asyncprocess.h"

const wxEventType wxEVT_SHELL_COMMAND_ADDLINE = XRCID("wxEVT_SHELL_COMMAND_ADDLINE");
const wxEventType wxEVT_SHELL_COMMAND_STARTED = XRCID("wxEVT_SHELL_COMMAND_STARTED");
const wxEventType wxEVT_SHELL_COMMAND_PROCESS_ENDED = XRCID("wxEVT_SHELL_COMMAND_PROCESS_ENDED");
const wxEventType wxEVT_SHELL_COMMAND_STARTED_NOCLEAN = XRCID("wxEVT_SHELL_COMMAND_STARTED_NOCLEAN");

BEGIN_EVENT_TABLE(ShellCommand, wxEvtHandler)
EVT_COMMAND(wxID_ANY, wxEVT_PROC_DATA_READ,  ShellCommand::OnProcessOutput)
EVT_COMMAND(wxID_ANY, wxEVT_PROC_TERMINATED, ShellCommand::OnProcessTerminated)
END_EVENT_TABLE()

ShellCommand::ShellCommand(wxEvtHandler *owner, const QueueCommand &buildInfo):
  m_proc(NULL),
  m_owner(owner),
  m_info(buildInfo)
{
}

void
ShellCommand::AppendLine(const wxString &line)
{
	if (NULL != m_owner)
  {
	  wxCommandEvent event(wxEVT_SHELL_COMMAND_ADDLINE);
	  event.SetString(line);
      event.SetInt(m_info.GetKind());
	  m_owner->AddPendingEvent(event);

	  m_lines.Add(line);
	}
}

void
ShellCommand::Stop()
{
	//kill the build process
	CleanUp();
}

void
ShellCommand::SendStartMsg()
{
	if (NULL != m_owner)
  {
    wxCommandEvent event(m_info.GetCleanLog() ? wxEVT_SHELL_COMMAND_STARTED : wxEVT_SHELL_COMMAND_STARTED_NOCLEAN);
    event.SetString(m_info.GetSynopsis());
    m_owner->AddPendingEvent(event);
  }
}

void
ShellCommand::SendEndMsg()
{
	if (NULL != m_owner)
  {
	  wxCommandEvent event(wxEVT_SHELL_COMMAND_PROCESS_ENDED);
      event.SetString(m_info.GetSynopsis());
	  m_owner->AddPendingEvent(event);
	}
}

void
ShellCommand::DoPrintOutput(const wxString &out)
{
	// Write the buffer as-is to the build tab !!
	AppendLine(out);
}

void
ShellCommand::CleanUp()
{
	if (NULL != m_proc)
	{
		delete m_proc;
		m_proc = NULL;
	}
	SendEndMsg();
}

void
ShellCommand::DoSetWorkingDirectory(ProjectPtr proj, bool isCustom, bool isFileOnly)
{
	//when using custom build, user can select different working directory
	if (NULL != proj)
	{
		if (isCustom)
		{
			//first set the path to the project working directory
			::wxSetWorkingDirectory(proj->GetFileName().GetPath());

			BuildConfigPtr buildConf = SolutionST::Get()->GetProjBuildConf(m_info.GetProject(), m_info.GetConfiguration());
			if (NULL != buildConf)
			{
				wxString wd = buildConf->GetCustomBuildWorkingDir();
				if (wd.IsEmpty())
				{
					// use the project path
					wd = proj->GetFileName().GetPath();
				}
				else
				{
					// expand macros from path
					wd = ExpandAllVariables(wd, SolutionST::Get(), proj->GetName(), buildConf->GetName(), wxEmptyString);
				}
				::wxSetWorkingDirectory(wd);
			}
		}
		else
		{
			if(m_info.GetProjectOnly() || isFileOnly)
			{
				//first set the path to the project working directory
				::wxSetWorkingDirectory(proj->GetFileName().GetPath());
			}
		}
	}
}

void
ShellCommand::OnProcessOutput(wxCommandEvent& e)
{
	ProcessEventData *ped = (ProcessEventData*)e.GetClientData();
	
	if (NULL != ped)
	{
		DoPrintOutput(ped->GetData());
		delete ped;
	}
	e.Skip();
}

void
ShellCommand::OnProcessTerminated(wxCommandEvent& e)
{
	ProcessEventData *ped = (ProcessEventData*)e.GetClientData();
	delete ped;
	CleanUp();
}

