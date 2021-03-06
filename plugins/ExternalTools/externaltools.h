//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : externaltools.h
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

#ifndef __ExternalTools__
#define __ExternalTools__

#include "plugin.h"
#include "externaltoolsdata.h"

class wxToolBar;
class AsyncExeCmd;

class ExternalToolsPlugin : public IPlugin
{
	clToolBar *   m_tb;
	wxEvtHandler* topWin;
	AsyncExeCmd*  m_pipedProcess;
	wxMenu*       m_parentMenu;

protected:
	void    OnSettings(wxCommandEvent &e);
	void    OnLaunchExternalTool(wxCommandEvent &e);
	void    OnLaunchExternalToolUI(wxUpdateUIEvent &e);
	void    OnStopExternalTool(wxCommandEvent &e);
	void    OnStopExternalToolUI(wxUpdateUIEvent &e);
	void    DoLaunchTool(const ToolInfo &ti);
	void    DoRecreateToolbar();
	bool    IsRedirectedToolRunning();
	void    OnProcessEnd(wxProcessEvent &event);
	void    DoCreatePluginMenu();

public:
	ExternalToolsPlugin(IManager *manager);
	~ExternalToolsPlugin();

	//--------------------------------------------
	//Abstract methods
	//--------------------------------------------
	virtual clToolBar *CreateToolBar(wxWindow *parent);
	virtual void CreatePluginMenu(wxMenu *pluginsMenu);
	virtual void HookPopupMenu(wxMenu *menu, MenuType type);
	virtual void UnHookPopupMenu(wxMenu *menu, MenuType type);
	virtual void UnPlug();
};

#endif //ExternalTools

