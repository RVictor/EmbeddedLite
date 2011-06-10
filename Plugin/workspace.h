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
#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "singleton.h"
#include "wx/string.h"
#include <wx/xml/xml.h>
#include "wx/filename.h"
#include "project.h"
#include <map>
//#include "ctags_manager.h"
#include "configuration_mapping.h"
#include "optionsconfig.h"
#include "localworkspace.h"


#ifdef WXMAKINGDLL_LE_SDK
#    define WXDLLIMPEXP_LE_SDK WXEXPORT
#elif defined(WXUSINGDLL_LE_SDK)
#    define WXDLLIMPEXP_LE_SDK WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_LE_SDK
#endif // WXMAKINGDLL_LE_SDK

/*!
 * \brief
 * CSolution manager class
 *
 */
class WXDLLIMPEXP_LE_SDK CSolution
{
	friend class Singleton<CSolution>;
	
	wxXmlDocument m_doc;
	wxFileName m_fileName;
	std::map<wxString, ProjectPtr> m_projects;
	wxString m_startupDir;
	time_t m_modifyTime;

private:

	/// Constructor
	CSolution();

	/// Destructor
	virtual ~CSolution();

public:

	void SetStartupDir(const wxString& startupDir) {this->m_startupDir = startupDir;}
	const wxString& GetStartupDir() const {return m_startupDir;}

	/**
	 * Returns the workspace file name
	 */
	const wxFileName& GetSolutionFileName() const { return m_fileName; }

	/**
	 * \brief
	 * Create a new workspace
	 *
	 * \param name
	 * workspace name
	 *
	 * \param path
	 * workspace path
	 *
	 * \returns
	 * true on success false otherwise
	 */
	bool CreateSolution(const wxString &name, const wxString &path, wxString &errMsg);

	/**
	 * \brief
	 * Open an existing workspace
	 *
	 * \param fileName
	 * CSolution file name (including extesion)
	 *
	 * \returns
	 * true on success false otherwise
	 */
	bool OpenSolution(const wxString &fileName, wxString &errMsg);

	/**
	 * Close the currently opened workspace
	 */
	void CloseSolution();

	/**
	 * \brief close the currently open workspace without saving any changes made to it,
	 * and reload it again
	 */
	void ReloadSolution();

	/**
	 * \brief
	 * Create a new project with given name, path and type
	 *
	 * \param name
	 * project name
	 *
	 * \param path
	 * project path
	 *
	 * \param type
	 * Can be Project::EXECUTABLE, Project::STATIC_LIBRARY or Project::DYNAMIC_LIBRARY
	 * \returns
	 * true on success false otherwise
	 */
	bool CreateProject(const wxString &name, const wxString &path, const wxString &type, bool addToBuildMatrix, wxString &errMsg);

	/**
	 * \brief get a string property from the worksapce file
	 * \returns property value or wxEmptyString
	 */
	wxString GetStringProperty(const wxString &propName, wxString &errMsg);

	/**
	 * Find a project by name
	 * \param projName project name
	 * \param errMsg [output] incase an error, report the error to the caller
	 * \returns ProjectPtr or NULL if no match was found
	 */
	ProjectPtr FindProjectByName(const wxString &projName, wxString &errMsg) const;

	/**
	 * Return all project names under this workspace
	 */
	void GetProjectList(wxArrayString &list);

	/**
	 * Add an existing project to the workspace. If no workspace is open,
	 * this function does nothing
	 * \param path project file path name to add
	 * \param errMsg [output] incase an error, report the error to the caller
	 */
	bool AddProject(const wxString & path, wxString &errMsg);

	/**
	 * Remove project from the workspace. This function does not delete
	 * any file related to the project but simply removes it from the workspace
	 * \param name project name to remove
	 * \param errMsg [output] incase an error, report the error to the caller
	 * \return true on success false otherwise
	 */
	bool RemoveProject(const wxString &name, wxString &errMsg);

	/**
	 * \return The active project name or wxEmptyString
	 */
	wxString GetActiveProjectName();

	/**
	 * Set project as active
	 * \param name  project name
	 * \param active state
	 */
	void SetActiveProject(const wxString &name, bool active);

	/**
	 * Add new virtual directoy to workspace
	 * \param vdFullPath virtual directory full path
	 * \param errMsg [output] incase an error, report the error to the caller
	 * \return true on success false otherwise
	 */
	bool CreateGroupFolder(const wxString &vdFullPath, wxString &errMsg);

	/**
	 * Remove virtual directoy to workspace
	 * \param vdFullPath virtual directory full path
	 * \param errMsg [output] incase an error, report the error to the caller
	 * \return true on success false otherwise
	 */
	bool RemoveGroupFolder(const wxString &vdFullPath, wxString &errMsg);

	/**
	 * Add new file to project. The project name is taken from the virtual directory full path
	 * \param vdFullPath vritual directory full path including project
	 * \param fileName file name to add
	 * \param errMsg output
	 * \return true on success, false otherwise
	 */
	bool AddNewFile(const wxString &vdFullPath, const wxString &fileName, wxString &errMsg);

	/**
	 * Remove file from a project. The project name is taken from the virtual directory full path
	 * \param vdFullPath vritual directory full path including project
	 * \param fileName file name to remove
	 * \param errMsg output
	 * \return true on success, false otherwise
	 */
	bool RemoveFile(const wxString &vdFullPath, const wxString &fileName, wxString &errMsg);

	/**
	 * Save workspace & projects settings
	 */
	void Save();

	/**
	 * Return a node pointing to any workspace-wide editor preferences
	 */
	wxXmlNode* GetSolutionEditorOptions() const;

	/**
	 * Add or update local workspace options
	 */
	void SetSolutionEditorOptions(LocalOptionsConfigPtr opts);

	/**
	 * Return the configuration mapping for the workspace. 'Configuration Mapping' is
	 * the build matrix that should be used for this workspace, it contains all possible
	 * configurations and which project specific configuration should be built in that
	 * configuration
	 */
	BuildMatrixPtr GetBuildMatrix() const;

	/**
	 * Store configuration mapping for the workspace, overriding the current one
	 * \param mapping new mapping to store
	 */
	void SetBuildMatrix(BuildMatrixPtr mapping);

	/**
	 * Return the workspace name
	 */
	wxString GetName() const;

	/**
	 * return the project build configuration that matches the
	 * workspace selected configuration from the build matrix
	 */
	BuildConfigPtr GetProjBuildConf(const wxString &projectName, const wxString &confName) const;

	wxString ExpandVariables(const wxString &expression) const;

	/**
	 * \brief add project to the solution build matrix. By default EmbeddedLite will try to match the best project configuration
	 * to the solution configuration (i.e. Debug -> Debug, if no suitable match found, it will use the first one)
	 * \param prj
	 */
	void AddProjectToBuildMatrix(ProjectPtr prj);

	//----------------------------------
	//File modifications
	//----------------------------------

	/**
	 * return the last modification time (on disk) of editor's underlying file
	 */
	time_t GetFileLastModifiedTime() const;

	/**
	 * return/set the last modification time that was made by the editor
	 */
	time_t GetSolutionLastModifiedTime() const {
		return m_modifyTime;
	}
	void SetSolutionLastModifiedTime(time_t modificationTime) {
		m_modifyTime = modificationTime;
	}

private:
	/**
	 * Do the actual add project
	 * \param path project file path
	 * \param errMsg [output] incase an error, report the error to the caller
	 */
	bool DoAddProject(const wxString &path, wxString &errMsg);

	void RemoveProjectFromBuildMatrix(ProjectPtr prj);

	bool SaveXmlFile();
};

typedef Singleton<CSolution> SolutionST;

#endif // WORKSPACE_H
