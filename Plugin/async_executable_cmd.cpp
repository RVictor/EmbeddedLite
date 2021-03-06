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
#include <wx/xrc/xmlres.h>
#include <wx/tokenzr.h>

#include "async_executable_cmd.h"


const wxEventType wxEVT_ASYNC_PROC_ADDLINE    = XRCID("async_proc_addline");
const wxEventType wxEVT_ASYNC_PROC_ADDERRLINE = XRCID("async_proc_adderrline");
const wxEventType wxEVT_ASYNC_PROC_STARTED    = XRCID("async_proc_started");
const wxEventType wxEVT_ASYNC_PROC_ENDED      = XRCID("async_proc_ended");

static int AsyncExeTimerID = wxNewId();

BEGIN_EVENT_TABLE(AsyncExeCmd, wxEvtHandler)
	EVT_TIMER(AsyncExeTimerID, AsyncExeCmd::OnTimer)
END_EVENT_TABLE()

AsyncExeCmd::AsyncExeCmd(wxEvtHandler *owner)
		: m_proc(NULL)
		, m_owner(owner)
		, m_busy(false)
		, m_stop(false)
{
	m_timer = new wxTimer(this, AsyncExeTimerID);
}

AsyncExeCmd::~AsyncExeCmd()
{
	delete m_timer;
	m_timer = NULL;

	if (m_proc) {
		delete m_proc;
		m_proc = NULL;
	}
};

void AsyncExeCmd::AppendLine(const wxString &line, bool isErr)
{
	if ( !m_owner)
		return;

	wxCommandEvent event(isErr ?  wxEVT_ASYNC_PROC_ADDERRLINE : wxEVT_ASYNC_PROC_ADDLINE);
	event.SetEventObject(this);
	event.SetString(line);
	m_owner->ProcessEvent(event);
}

void AsyncExeCmd::Stop()
{
	m_stop = true;
	//kill the build process
	if (m_proc) {
		m_proc->Terminate();
	}
}

void AsyncExeCmd::SendStartMsg()
{
	if ( !m_owner)
		return;

	wxCommandEvent event(wxEVT_ASYNC_PROC_STARTED);
	event.SetEventObject(this);

	wxString msg;
	msg << wxT("Current working directory: ") << wxGetCwd() << wxT("\n");
	msg << wxT("Running program: ") << m_cmdLine << wxT("\n");
	event.SetString(msg);
	m_owner->ProcessEvent(event);
}

void AsyncExeCmd::SendEndMsg(int exitCode)
{
	if ( !m_owner)
		return;

	wxCommandEvent event(wxEVT_ASYNC_PROC_ENDED);
	event.SetEventObject(this);
	wxString message(wxT("Program exited with return code: "));
	message << exitCode << wxT("\n");
	event.SetString(message);
	m_owner->ProcessEvent(event);
}

void AsyncExeCmd::OnTimer(wxTimerEvent &event)
{
	wxUnusedVar(event);
	if ( m_stop ) {
		m_proc->Terminate();
		return;
	}
	PrintOutput();
}

void AsyncExeCmd::PrintOutput()
{
	if (m_proc->GetRedirect()) {
		wxString data, errors;
		m_proc->HasInput(data, errors);
		DoPrintOutput(data, errors);
	}
}

void AsyncExeCmd::DoPrintOutput(const wxString &out, const wxString &err)
{
	if (!out.IsEmpty()) {
		wxStringTokenizer tt(out, wxT("\n"));
		while (tt.HasMoreTokens()) {
			AppendLine(tt.NextToken() + wxT("\n"));
		}
	}

	if (!err.IsEmpty()) {
		wxStringTokenizer tt(err, wxT("\n"));
		while (tt.HasMoreTokens()) {
			AppendLine(tt.NextToken() + wxT("\n"), true);
		}
	}
}

void AsyncExeCmd::ProcessEnd(wxProcessEvent& event)
{
	//read all input before stopping the timer
	if ( !m_stop ) {
		if (m_proc->GetRedirect()) {
			wxString err;
			wxString out;
			m_proc->ReadAll(out, err);
			DoPrintOutput(out, err);
			out.Empty();
			err.Empty();
		}
	}

	//stop the timer if needed
	if(m_proc->GetRedirect()){
		m_timer->Stop();
	}
	
	m_busy = false;
	m_stop = false;

	SendEndMsg(event.GetExitCode());
}

void AsyncExeCmd::Execute(const wxString &cmdLine, bool hide, bool redirect)
{
	m_cmdLine = cmdLine;
	SetBusy(true);
	SendStartMsg();

	m_proc = new clProcess(wxNewId(), m_cmdLine, redirect);
	if (m_proc) {
		if (m_proc->Start(hide) == 0) {
			delete m_proc;
			m_proc = NULL;
			SetBusy(false);
		} else {
			//process started successfully, start timer if needed
			if(m_proc->GetRedirect()){
				m_timer->Start(10);
			}
		}
	}
}

void AsyncExeCmd::Terminate()
{
	m_proc->Terminate();
}
