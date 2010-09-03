//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : compile_request.cpp
//
// -------------------------------------------------------------------------
// A
//              _____           _      _     _ _
//             /  __ \         | |    | |   (_) |
//             | /  \/ ___   __| | ___| |    _| |_ ___
//             | |    / _ \ / _  |/ _ \ |   | | __/ _ )
//             | \__/\ (_) | (_| |  __/ |___| | ||  __/
//              \____/\___/ \__,_|\___\_____/_|\__\___|
//
//                                                  F i l e
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
#include "compile_request.h"
#include <wx/app.h>
#include <wx/log.h>
#include "asyncprocess.h"
#include "imanager.h"
#include "macros.h"
#include "compiler.h"
#include "build_settings_config.h"
#include "globals.h"
#include "build_config.h"
#include "environmentconfig.h"
#include "buildmanager.h"
#include "wx/process.h"
#include "workspace.h"
#include "dirsaver.h"
#include "plugin.h"

CompileRequest::CompileRequest(wxEvtHandler *owner, const QueueCommand &buildInfo, const wxString &fileName, bool runPremakeOnly, bool preprocessOnly)
		: ShellCommand(owner, buildInfo)
		, m_fileName(fileName)
		, m_premakeOnly(runPremakeOnly)
		, m_preprocessOnly(preprocessOnly)
{
}

CompileRequest::~CompileRequest()
{
	//no need to delete the process, it will be deleted by the wx library
}

//do the actual cleanup
void CompileRequest::Process(IManager *manager)
{
	wxString  cmd;
	wxString  errMsg;
	StringMap om;

	BuildSettingsConfig *bsc(manager ? manager->GetBuildSettingsConfigManager() : BuildSettingsConfigST::Get());
	BuildManager *bm(manager ? manager->GetBuildManager() : BuildManagerST::Get());
	Workspace *w(manager ? manager->GetWorkspace() : WorkspaceST::Get());
	EnvironmentConfig *env(manager ? manager->GetEnv() : EnvironmentConfig::Instance());

	ProjectPtr proj = w->FindProjectByName(m_info.GetProject(), errMsg);
	if (!proj) {
		AppendLine(wxT("Cant find project: ") + m_info.GetProject());
		return;
	}

	// since this code can be called from inside the application OR
	// from inside a DLL, we use the application pointer from the manager
	// when available, otherwise, events will not be processed inside
	// plugins
	wxApp *app = manager ? manager->GetTheApp() : wxTheApp;
	wxString pname (proj->GetName());

	//BuilderPtr builder = bm->GetBuilder(wxT("GNU makefile for g++/gcc"));
	BuilderPtr builder = bm->GetSelectedBuilder();
	if (m_fileName.IsEmpty() == false) {
		//we got a complie request of a single file
		cmd = m_preprocessOnly ? builder->GetPreprocessFileCmd(m_info.GetProject(), m_info.GetConfiguration(), m_fileName, errMsg)
		      : builder->GetSingleFileCmd(m_info.GetProject(), m_info.GetConfiguration(), m_fileName);
	} else if (m_info.GetProjectOnly()) {

		switch ( m_info.GetKind() ) {
		case QueueCommand::ReBuild:
			cmd = builder->GetPORebuildCommand(m_info.GetProject(), m_info.GetConfiguration());
			break;
		default:
		case QueueCommand::Build:
			cmd = builder->GetPOBuildCommand(m_info.GetProject(), m_info.GetConfiguration());
			break;
		}

	} else {
		cmd = builder->GetBuildCommand(m_info.GetProject(), m_info.GetConfiguration());
	}

	// Notify plugins that a compile process is going to start
	wxCommandEvent event(wxEVT_BUILD_STARTING);
	event.SetClientData((void*)&pname);
	event.SetString( m_info.GetConfiguration() );

	if (app->ProcessEvent(event)) {

		// the build is being handled by some plugin, no need to build it
		// using the standard way
		return;
	}

	// Send the EVENT_STARTED : even if this event is sent, next event will
	// be post, so no way to be sure the the build process has not started
	SendStartMsg();

	//if we require to run the makefile generation command only, replace the 'cmd' with the
	//generation command line
	BuildConfigPtr bldConf = w->GetProjBuildConf(m_info.GetProject(), m_info.GetConfiguration());
	if (m_premakeOnly && bldConf) {
		BuildConfigPtr bldConf = w->GetProjBuildConf(m_info.GetProject(), m_info.GetConfiguration());
		if (bldConf) {
			cmd = bldConf->GetMakeGenerationCommand();
		}

	}

	if (bldConf) {
		wxString cmpType = bldConf->GetCompilerType();
		CompilerPtr cmp = bsc->GetCompiler(cmpType);
		if (cmp) {
			wxString value( cmp->GetPathVariable() );
			if (value.Trim().Trim(false).IsEmpty() == false) {
				wxLogMessage(wxString::Format(wxT("Setting PATH to '%s'"), value.c_str()));
				om[wxT("PATH")] = value.Trim().Trim(false);
			}
		}
	}

	if (cmd.IsEmpty()) {
		//if we got an error string, use it
		if (errMsg.IsEmpty() == false) {
			AppendLine(errMsg);
		} else {
			AppendLine(wxT("Command line is empty. Build aborted."));
		}
		return;
	}

	WrapInShell(cmd);
	DirSaver ds;

	DoSetWorkingDirectory(proj, false, m_fileName.IsEmpty() == false);

	//expand the variables of the command
	cmd = ExpandAllVariables(cmd, w, m_info.GetProject(), m_info.GetConfiguration(), m_fileName);

	//print the build command
	AppendLine(cmd + wxT("\n"));
	if (m_info.GetProjectOnly() || m_fileName.IsEmpty() == false) {
		// set working directory
		DoSetWorkingDirectory(proj, false, m_fileName.IsEmpty() == false);
	}

	// print the prefix message of the build start. This is important since the parser relies
	// on this message
	if (m_info.GetProjectOnly() || m_fileName.IsEmpty() == false) {
		wxString configName(m_info.GetConfiguration());

		//also, send another message to the main frame, indicating which project is being built
		//and what configuration
		wxString text;
		text << BUILD_PROJECT_PREFIX << m_info.GetProject() << wxT(" - ") << configName << wxT(" ]");
		if (m_fileName.IsEmpty()) {
			text << wxT("----------\n");
		} else if (m_preprocessOnly) {
			text << wxT(" (Preprocess Single File)----------\n");
		} else {
			text << wxT(" (Single File Build)----------\n");
		}
		AppendLine(text);
	}

	EnvSetter envir(env, &om);
	m_proc = CreateAsyncProcess(this, cmd);
	if (!m_proc ) {
		wxString message;
		message << wxT("Failed to start build process, command: ") << cmd << wxT(", process terminated with exit code: 0");
		AppendLine(message);
		return;
	}
}
