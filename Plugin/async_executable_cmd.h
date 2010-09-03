//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : async_executable_cmd.h              
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
 #ifndef ASYNC_EXECUTABLE_CMD_H
#define ASYNC_EXECUTABLE_CMD_H

#include "wx/event.h"
#include "cl_process.h"
#include "wx/timer.h"

#ifdef WXMAKINGDLL_LE_SDK
#    define WXDLLIMPEXP_LE_SDK WXEXPORT
#elif defined(WXUSINGDLL_LE_SDK)
#    define WXDLLIMPEXP_LE_SDK WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_LE_SDK
#endif // WXMAKINGDLL_LE_SDK

DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_LE_SDK, wxEVT_ASYNC_PROC_ADDLINE, wxID_ANY)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_LE_SDK, wxEVT_ASYNC_PROC_ADDERRLINE, wxID_ANY)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_LE_SDK, wxEVT_ASYNC_PROC_STARTED, wxID_ANY)
DECLARE_EXPORTED_EVENT_TYPE(WXDLLIMPEXP_LE_SDK, wxEVT_ASYNC_PROC_ENDED, wxID_ANY)



class WXDLLIMPEXP_LE_SDK AsyncExeCmd : public wxEvtHandler {
protected:
	clProcess *m_proc;
	wxEvtHandler *m_owner;
	wxTimer *m_timer;
	bool m_busy;
	bool m_stop;
	wxString m_cmdLine;

protected:
	virtual void OnTimer(wxTimerEvent &event);
	virtual void PrintOutput();
	virtual void DoPrintOutput(const wxString &out, const wxString &err);
	
public:
	bool IsBusy() const { return m_busy; }
	void SetBusy(bool busy) { m_busy = busy; }
	void Stop();
	void ProcessEnd(wxProcessEvent& event);
	clProcess *GetProcess() { return m_proc; }

public:
	//construct a compiler action 
	// \param owner the window owner for this action
	AsyncExeCmd(wxEvtHandler *owner) ;
	
	virtual ~AsyncExeCmd();
	virtual void Execute(const wxString &cmdLine, bool hide = true, bool redirect = true);

	wxOutputStream *GetOutputStream() { 
		if(m_proc){
			return m_proc->GetOutputStream(); 
		}
		return NULL;
	}

	void AppendLine(const wxString &line, bool isErr = false);
	void SendStartMsg();
	void SendEndMsg(int exitCode);
	void Terminate();
	DECLARE_EVENT_TABLE()
};

#endif // ASYNC_EXECUTABLE_CMD_H

