//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : builder.h
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
#ifndef BUILDER_H
#define BUILDER_H

#include "wx/string.h"
#include "smart_ptr.h"
#include "wx/event.h"

/**
 * \ingroup SDK
 * this class defines the interface of a build system
 *
 * \version 1.0
 * first version
 *
 * \date 05-19-2007
 *
 * \author Eran
 */
class Builder
{
protected:
	wxString m_name;
	wxString m_buildTool;
	wxString m_buildToolOptions;
	wxString m_buildToolJobs;
	bool     m_isActive;
	
public:
	Builder(const wxString &name, const wxString &buildTool, const wxString &buildToolOptions) ;
	virtual ~Builder();

	void SetBuildTool(const wxString &buildTool) {
		m_buildTool = buildTool;
	}
	void SetBuildToolOptions(const wxString &buildToolOptions) {
		m_buildToolOptions = buildToolOptions;
	}
	void SetBuildToolJobs(const wxString &buildToolJobs) {
		m_buildToolJobs = buildToolJobs;
	}

	/**
	 * Normalize the configuration name, this is done by removing any trailing and leading
	 * spaces from the string, and replacing any space character with underscore.
	 */
	static wxString NormalizeConfigName(const wxString &confgName);

	/**
	 * \return the builder name
	 */
	const wxString &GetName() const {
		return m_name;
	}

	// ================ API ==========================
	// The below API as default implementation, but can be
	// overrided in the derived class
	// ================ API ==========================
	
	/**
	 * @brief set this builder as the active builder. It also makes sure that all other 
	 * builders are set as non-active
	 */
	virtual void SetActive();
	
	/**
	 * @brief return true if this builder is the active one
	 * @return 
	 */
	virtual bool IsActive() const {
		return m_isActive;
	}
	
	/**
	 * \return the build tool assoicated with this builder
	 */
	virtual wxString GetBuildToolCommand(bool isCommandlineCommand) const {
		return m_buildTool;
	}

	/**
	 * return the build tool name
	 */
	virtual wxString GetBuildToolName() const {
		return m_buildTool;
	}

	/**
	 * return the build tool options
	 */
	virtual wxString GetBuildToolOptions() const {
		return m_buildToolOptions;
	}

	/**
	 * return the build tool jobs
	 */
	virtual wxString GetBuildToolJobs() const {
		return m_buildToolJobs;
	}

	// ================ API ==========================
	// The below API must be implemented by the
	// derived class
	// ================ API ==========================

	/**
	 * Export the build system specific file (e.g. GNU makefile, Ant file etc)
	 * to allow users to invoke them manualy from the command line
	 * \param project project to export.
	 * \param errMsg output
	 * \return true on success, false otherwise.
	 */
	virtual bool Export(const wxString &project, const wxString &confToBuild, bool isProjectOnly, bool force, wxString &errMsg) = 0;

	/**
	 * Return the command that should be executed for performing the clean
	 * task
	 */
	virtual wxString GetCleanCommand(const wxString &project, const wxString &confToBuild) = 0;

	/**
	 * Return the command that should be executed for performing the build
	 * task for a given project
	 */
	virtual wxString GetBuildCommand(const wxString &project, const wxString &confToBuild) = 0;

	//-----------------------------------------------------------------
	// Project Only API
	//-----------------------------------------------------------------
	/**
	 * Return the command that should be executed for performing the clean
	 * task - for the project only (excluding dependencies)
	 */
	virtual wxString GetPOCleanCommand(const wxString &project, const wxString &confToBuild) = 0;

	/**
	 * Return the command that should be executed for performing the build
	 * task for a given project - for the project only (excluding dependencies)
	 */
	virtual wxString GetPOBuildCommand(const wxString &project, const wxString &confToBuild) = 0;

	/**
	 * \brief create a command to execute for compiling single source file
	 * \param project
	 * \param fileName
	 * \param errMsg [output]
	 * \return the command
	 */
	virtual wxString GetSingleFileCmd(const wxString &project, const wxString &confToBuild, const wxString &fileName) = 0;

	/**
	 * \brief create a command to execute for preprocessing single source file
	 * \param project
	 * \param fileName
	 * \param errMsg [output]
	 * \return the command
	 */
	virtual wxString GetPreprocessFileCmd(const wxString &project, const wxString &confToBuild, const wxString &fileName, wxString &errMsg) = 0;

	/**
	 * @brief return the 'rebuild' command
	 * @param project
	 * @param confToBuild
	 * @return
	 */
	virtual wxString GetPORebuildCommand(const wxString &project, const wxString &confToBuild) = 0;
};

typedef SmartPtr<Builder> BuilderPtr;

#endif // BUILDER_H
