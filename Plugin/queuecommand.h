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
#ifndef __queuecommand__
#define __queuecommand__

#include <wx/string.h>

class QueueCommand
{
	wxString m_project;
	wxString m_configuration;
	bool m_projectOnly;
	int m_kind;
	bool m_cleanLog;
	bool m_checkBuildSuccess;
	wxString m_customBuildTarget;
    mutable wxString m_synopsis;

public:
	// Command's kind
	enum {
		Build,
		Clean,
		CustomBuild,
		Debug,
		Cmd,
		BatchCmds,
		ReBuild
	};

public:
	QueueCommand(const wxString &project, const wxString &configuration, bool projectOnly, int kind);
	QueueCommand(int kind);
	~QueueCommand();

	//----------------------------------------
	// Setters/Getters
	//----------------------------------------

	void SetConfiguration(const wxString& configuration) ;
	void SetProject(const wxString& project) ;
	const wxString& GetConfiguration() const ;
	const wxString& GetProject() const ;
	void SetProjectOnly(const bool& projectOnly) ;
	const bool& GetProjectOnly() const ;
	void SetKind(const int& kind) ;
	const int& GetKind() const ;
	void SetCleanLog(const bool& cleanLog) ;
	const bool& GetCleanLog() const ;
	void SetCheckBuildSuccess(const bool& checkBuildSuccess) {
		this->m_checkBuildSuccess = checkBuildSuccess;
	}
	const bool& GetCheckBuildSuccess() const {
		return m_checkBuildSuccess;
	}
	void SetCustomBuildTarget(const wxString& customBuildTarget) {
		this->m_customBuildTarget = customBuildTarget;
	}
	const wxString& GetCustomBuildTarget() const {
		return m_customBuildTarget;
	}
    const wxString& GetSynopsis() const {
        if (m_synopsis.IsEmpty())
            m_synopsis = DeriveSynopsis();
        return m_synopsis;
    }
    void SetSynopsis(const wxString& synopsis) {
        m_synopsis = synopsis;
    }
    wxString DeriveSynopsis() const;
};

#endif // __queuecommand__
