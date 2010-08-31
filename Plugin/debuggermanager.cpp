//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : debuggermanager.cpp
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
#include "debuggermanager.h"
#include "wx/filename.h"
#include <wx/dir.h>
#include <wx/log.h>
#include "editor_config.h"
#include <wx/msgdlg.h>
#include "debuggerconfigtool.h"

//---------------------------------------------------------
DebuggerMgr *DebuggerMgr::ms_instance = NULL;

DebuggerMgr::DebuggerMgr()
{
}

DebuggerMgr::~DebuggerMgr()
{
	std::vector<clDynamicLibrary*>::iterator iter = m_dl.begin();
	for(; iter != m_dl.end(); iter++){
		(*iter)->Detach();
		delete (*iter);
	}
	m_dl.clear();
	m_debuggers.clear();

}

DebuggerMgr& DebuggerMgr::Get()
{
	if(!ms_instance){
		ms_instance = new DebuggerMgr();
	}
	return *ms_instance;
}

void DebuggerMgr::Free()
{
	delete ms_instance;
	ms_instance = NULL;
}

bool DebuggerMgr::LoadDebuggers(const wxString& strDebuggersDir)
{
	wxString ext;
#if defined (__WXMSW__)
	ext = wxT("dll");
#else
	ext = wxT("so");
#endif
	wxString fileSpec(wxT("*.")+ext);

	//get list of dlls
	wxArrayString files;

  wxString debuggersPath(strDebuggersDir, wxConvUTF8);
	debuggersPath += wxT("/debuggers");

	wxDir::GetAllFiles(debuggersPath, &files, fileSpec, wxDIR_FILES);

	for(size_t i=0; i<files.GetCount(); i++){
		clDynamicLibrary *dl = new clDynamicLibrary();
		wxString fileName(files.Item(i));
		if(!dl->Load(fileName)){
			wxLogMessage(wxT("Failed to load debugger's dll: ") + fileName);
            if (!dl->GetError().IsEmpty())
                wxLogMessage(dl->GetError());
			delete dl;
			continue;
		}

		bool success(false);
		GET_DBG_INFO_FUNC pfn = (GET_DBG_INFO_FUNC)dl->GetSymbol(wxT("GetDebuggerInfo"), &success);
		if(!success){
            wxLogMessage(wxT("Failed to find GetDebuggerInfo() in dll: ") + fileName);
            if (!dl->GetError().IsEmpty())
                wxLogMessage(dl->GetError());
			//dl->Unload();
			delete dl;
			continue;
		}

		DebuggerInfo info = pfn();
		//Call the init method to create an instance of the debugger
		success = false;
		GET_DBG_CREATE_FUNC pfnInitDbg = (GET_DBG_CREATE_FUNC)dl->GetSymbol(info.initFuncName, &success);
		if(!success){
            wxLogMessage(wxT("Failed to find init function in dll: ") + fileName);
            if (!dl->GetError().IsEmpty())
                wxLogMessage(dl->GetError());
			dl->Detach();
			delete dl;
			continue;
		}

		wxLogMessage(wxT("Loaded debugger: ") + info.name + wxT(", Version: ") + info.version);
		IDebugger *dbg = pfnInitDbg();

		//set the environment
		dbg->SetEnvironment(m_env);

		m_debuggers[info.name] = dbg;

		//keep the dynamic load library
		m_dl.push_back(dl);
	}
	return true;
}

wxArrayString DebuggerMgr::GetAvailableDebuggers()
{
	wxArrayString dbgs;
	std::map<wxString, IDebugger*>::iterator iter = m_debuggers.begin();
	for(; iter != m_debuggers.end(); iter++){
		dbgs.Add(iter->first);
	}
	return dbgs;
}

IDebugger* DebuggerMgr::GetActiveDebugger()
{
	if(m_activeDebuggerName.IsEmpty()){
		//no active debugger is set, use the first one
		std::map<wxString, IDebugger*>::iterator iter = m_debuggers.begin();
		if(iter != m_debuggers.end()){
			SetActiveDebugger( iter->first );
			return iter->second;
		}
		return NULL;
	}

	std::map<wxString, IDebugger*>::iterator iter = m_debuggers.find(m_activeDebuggerName);
	if(iter != m_debuggers.end()){
		return iter->second;
	}
	return NULL;
}

void DebuggerMgr::SetActiveDebugger(const wxString &name)
{
	m_activeDebuggerName = name;
}


void DebuggerMgr::SetDebuggerInformation(const wxString &name, const DebuggerInformation &info)
{
	DebuggerConfigTool::Get()->WriteObject(name, (SerializedObject*)&info);
}

bool DebuggerMgr::GetDebuggerInformation(const wxString &name, DebuggerInformation &info)
{
	return DebuggerConfigTool::Get()->ReadObject(name, &info);
}
