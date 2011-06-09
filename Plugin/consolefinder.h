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
#ifndef CONSOLEFINDER_H
#define CONSOLEFINDER_H

#include "precompiled_header.h"

/**
 * \ingroup SDK
 * a misc class that allows user to locate a free pty terminal
 * this is used by the debugger
 * \version 1.0
 * first version
 *
 * \date 10-14-2007
 *
 * \author Eran
 *
 */
class ConsoleFinder
{
	wxString  m_ConsoleTty;
	int       m_nConsolePid;
	wxString  m_consoleCommand;

private:
	int RunConsole(const wxString &title);
	wxString GetConsoleTty(int ConsolePid);

public:
	ConsoleFinder();
	virtual ~ConsoleFinder();

	bool FindConsole(const wxString &title, wxString &consoleName);
	void FreeConsole();
	wxString GetConsoleName();

	void SetConsoleCommand(const wxString& consoleCommand) {
		this->m_consoleCommand = consoleCommand;
	}
	const wxString& GetConsoleCommand() const {
		return m_consoleCommand;
	}
};

#endif
