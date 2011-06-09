/**
  \file manager.h

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef MANAGER_H
#define MANAGER_H

#include <list>
#include <map>
#include <wx/event.h>

#include "singleton.h"
#include "debuggerobserver.h"
#include "workspace.h"
#include "queuecommand.h"
#include "shell_command.h"
#include "async_executable_cmd.h"
#include "filehistory.h"
#include "breakpointsmgr.h"


// ====================================================================
// The Manager class
// ====================================================================

extern const wxEventType wxEVT_CMD_RESTART_CODELITE;

class DisplayVariableDlg;

class Manager : public wxEvtHandler, public IDebuggerObserver
{
  friend class Singleton<Manager>;

 	wxString                m_strInstallDir;
 	wxString                m_strDataDir;
 	wxString                m_startupDir;
  wxString                m_strConfigDir;
  wxString                m_strLexersDir;
  wxString                m_strShareDir;
  wxString                m_strPluginsDir;
 	FileHistory             m_recentWorkspaces;
 	ShellCommand           *m_shellProcess;
 	AsyncExeCmd            *m_asyncExeCmd;
 	BreakptMgr             *m_breakptsmgr;
 	bool                    m_isShutdown;
 	bool                    m_workspceClosing;
 	bool                    m_dbgCanInteract;
 	bool                    m_useTipWin;
 	long                    m_tipWinPos;
 	int                     m_frameLineno;
	std::list<QueueCommand> m_buildQueue;
	wxArrayString           m_dbgWatchExpressions;
//rvv	wxFileName              m_codeliteLauncher;
	DisplayVariableDlg       *m_displayVariableDlg;

protected:
	Manager(void);
	virtual ~Manager(void);

     //--------------------------- Global State -----------------------------
public:
	DisplayVariableDlg * GetDisplayVariableDialog();

 	const wxString &GetAppInstallDirectory() const { return m_strInstallDir; }
 	void SetAppInstallDirectory(const wxString &path) { m_strInstallDir = path; }

 	const wxString &GetStarupDirectory() const { return m_startupDir; }
 	void SetStarupDirectory(const wxString &path) { m_startupDir = path; }

 	const wxString &GetAppShareDir() const { return m_strShareDir; }
 	void SetAppShareDir(const wxString &dir) { m_strShareDir = dir; }

 	const wxString &GetAppDataDir() const { return m_strDataDir; }
 	void SetAppDataDir(const wxString &dir) { m_strDataDir = dir; }

  const wxString& GetConfigDir() const { return m_strConfigDir; }
  void SetConfigDir(const wxString& strConfigDir) { m_strConfigDir = strConfigDir; }

  const wxString& GetLexersDir() const { return m_strLexersDir; }
  void SetLexersDir(const wxString& strLexersDir) { m_strLexersDir = strLexersDir; }

	/**
	 * @brief return the installation directory of plugins binaries.
	 * @return a full path to plugins binaries installation.
	 */
  const wxString& GetPluginsDir() const { return m_strPluginsDir; }
	/**
	 * @brief Set the installation directory of plugins binaries.
	 * @param strPluginsDir - path to the installation directory of plugins binaries.
	 */
  void SetPluginsDir(const wxString& strPluginsDir) { m_strPluginsDir = strPluginsDir; }

	bool IsShutdownInProgress() const { return m_isShutdown; }
 	void SetShutdownInProgress(bool b) { m_isShutdown = b; }

	void SetEmbeddedLiteLauncherPath(const wxString &path);
	void OnRestart(wxCommandEvent &event);
protected:
	void DoRestartEmbeddedLite();

	 //--------------------------- Solution Loading -----------------------------
public:
 	/*!
 	 * \brief
 	 * check if a workspace is open
 	 *
 	 * \returns
 	 * true if a workspace is open
 	 */
 	bool IsWorkspaceOpen() const;

	const bool& IsWorkspaceClosing() const { return m_workspceClosing; }

	/*!
	 * \brief
	 * Create a workspace with a given name and path
	 *
	 * \param name
	 * workspace name
	 *
	 * \param path
	 * workspace path
	 *
	 */
	void CreateWorkspace(const wxString &name, const wxString &path);

	/**
	 * Open an existing workspace by path
	 */
	void OpenWorkspace(const wxString &path);

	/**
	 * \brief close the currently opened workspace and reload it without saving any modifications made to it, if no workspace is opened, this functiond does anything
	 */
	void ReloadWorkspace();

	/**
	 * Close the current workspace and save all
	 * changes
	 */
	void CloseWorkspace();

	/**
	 * \brief clear the recent workspaces list
	 */
	void ClearWorkspaceHistory();

	/**
	 * Return list of recently opened workspaces from the configuration file
	 * \param files [output]
	 */
	void GetRecentlyOpenedWorkspaces(wxArrayString &files);

	/**
	 * return the FileHistory object that holds the recently opened
	 * workspace data
	 */
	FileHistory &GetRecentlyOpenedWorkspacesClass() { return m_recentWorkspaces; }

	/**
	 * @brief update the C++ parser search / exclude paths with the global paths
	 * and the workspace specifc ones
	 * @return true if the paths were modified, false otherwise
	 */
	bool UpdateParserPaths();

protected:
	void DoSetupWorkspace(const wxString &path);
	void AddToRecentlyOpenedWorkspaces(const wxString &fileName);


    //--------------------------- Solution Projects Mgmt -----------------------------
public:
	/**
	 * \brief create an empty project
	 * \param name project name
	 * \param path project file path
	 * \param type project type, Project::STATIC_LIBRARY, Project::DYNAMIC_LIBRARY or Project::EXECUTABLE
	 */
	void CreateProject(ProjectData &data);

	/**
	 * Add an existing project to the workspace. If no workspace is open,
	 * this function does nothing
	 * \param path project file path name to add
	 */
	void AddProject(const wxString &path);

	/**
	 * Import a MS Solution file and open it in the editor
	 * \param path path to the .sln file
	 */
	void ImportMSVSSolution(const wxString &path, const wxString &defaultCompiler);

	/**
	 * Remove the a project from the workspace
	 * \param name project name to remove
	 * \return true on success false otherwise
	 */
	bool RemoveProject(const wxString &name) ;

	/**
	 * Return all project names under this workspace
	 */
	void GetProjectList(wxArrayString &list);

	/**
	 * find project by name
	 */
	ProjectPtr GetProject(const wxString &name) const;

	/**
	 * \return active project name
	 */
	wxString GetActiveProjectName();

	/**
	 * Set project as active
	 * \param name project name to set as active
	 */
	void SetActiveProject(const wxString &name);

	/**
	 * Return the workspace build matrix
	 */
	BuildMatrixPtr GetWorkspaceBuildMatrix() const;

	/**
	 * Set or update the workspace build matrix
	 */
	void SetWorkspaceBuildMatrix(BuildMatrixPtr matrix);


    //--------------------------- Solution Files Mgmt -----------------------------
public:
	/**
	 * return list of files in absolute path of the whole workspace
	 */
	void GetWorkspaceFiles(wxArrayString &files);

	/**
	 * return list of files that are part of the workspace
	 */
	void GetWorkspaceFiles(std::vector<wxFileName> &files, bool absPath = false);

	/**
	 * check if a file is part of the workspace
	 * \param fileName the file name in absolute path
	 */
	bool IsFileInWorkspace(const wxString &fileName);

    /**
     * Search for (non-absolute) file in the workspace
     */
    wxFileName FindFile(const wxString &fileName, const wxString &project = wxEmptyString);

	/**
	 * retag workspace
	 */
	void RetagWorkspace(bool quickRetag);

	/**
	 * @brief the parser thread has completed to scan for include files to parse
	 * @param event
	 */
	void OnIncludeFilesScanDone(wxCommandEvent &event);

	/**
	 * \brief retag a given file
	 * \param filename
	 */
	void RetagFile(const wxString &filename);

protected:
    wxFileName FindFile ( const wxArrayString& files, const wxFileName &fn );


    //--------------------------- Project Files Mgmt -----------------------------
public:
	/**
	 * Add new virtual directory to the workspace.
	 * \param wxstrGroupFolderFullPath a dot separated string of the new virtual directory full path up to the parent project
	 *        for example: to add a new VD name VD3 under: Project1->VD1->VD2 path should contain: Project1.VD1.VD2.VD3
	 */
	void AddGroupFolder(const wxString &wxstrGroupFolderFullPath);

	/**
	 * Remove virtual directory from the workspace.
	 * \param wxstrGroupFolderFullPath a dot separated string of the virtual directory to be removed
	 */
	void RemoveGroupFolder(const wxString &wxstrGroupFolderFullPath);

	/**
	 * Create new file on the disk and open it in the main editor
	 * \param fileName file full path (including directories)
	 * \param vdFullPath path of the virtual directory
	 */
	bool AddNewFileToProject(const wxString &fileName, const wxString &vdFullPath, bool openIt = true);

	/**
	 * Add an existing file to workspace
	 * \param fileName file full path (including directories)
	 * \param vdFullPath path of the virtual directory
	 */
	bool AddFileToProject(const wxString &fileName, const wxString &vdFullPath, bool openIt = false);

	/**
	 * \brief
	 * \param files
	 * \param vdFullPath
	 * \param actualAdded
	 * \return
	 */
	void AddFilesToProject(const wxArrayString &files, const wxString &vdFullPath, wxArrayString &actualAdded);

	/**
	 * remove file from the workspace
	 * \param fileName the full path of the file to be removed
	 * \param vdFullPath the files' virtual directory path (including project)
	 */
	bool RemoveFile(const wxString &fileName, const wxString &vdFullPath);

	/**
	 * remove file from the workspace
	 * \param origName the full path of the file to be renamed
	 * \param newName the full path the file must be renamed to
	 * \param vdFullPath the files' virtual directory path (including project)
	 */
	bool RenameFile(const wxString &origName, const wxString &newName, const wxString &vdFullPath);

	bool MoveFileToVD(const wxString &fileName, const wxString &srcVD, const wxString &targetVD);

	/**
	 * Rebuild the database by removing all entries from the database
	 * that belongs to a given project, and then re-index all files
	 * \param projectName project to re-tag
	 */
	void RetagProject(const wxString &projectName, bool quickRetag);

	/**
	 * return list of files in absolute path of a given project
	 * \param project project name
	 */
	void GetProjectFiles(const wxString &project, wxArrayString &files);

	/**
	 * return the project name that 'fullPathFileName' belogs to. if 2 matches were found, return
	 * the first one, or empty string if no match was found
	 */
	wxString GetProjectNameByFile(const wxString &fullPathFileName);


    //--------------------------- Project Settings Mgmt -----------------------------
public:
  	/**
 	 * Return a project working directory
 	 * \param project project name
  	 */
 	wxString GetProjectCwd(const wxString &project) const;

  	/**
 	 * Return project settings by name
 	 * \param projectName project name
 	 * \return project settings smart prt
  	 */
 	ProjectSettingsPtr GetProjectSettings(const wxString &projectName) const;

  	/**
 	 * Set project settings
 	 * \param projectName project name
 	 * \param settings settings to update
  	 */
 	void SetProjectSettings(const wxString &projectName, ProjectSettingsPtr settings);

  	/**
 	 * Set project global settings
 	 * \param projectName project name
 	 * \param settings global settings to update
  	 */
	void SetProjectGlobalSettings(const wxString &projectName, BuildConfigCommonPtr settings);

  	/**
 	 * \brief return the project excution command as it appears in the project settings
 	 * \param projectName
 	 * \param wd the working directory that the command should be running from
 	 * \param considerPauseWhenExecuting when set to true (default) EmbeddedLite will take into consideration the value set in the project
 	 * settings 'Pause when execution ends'
 	 * \return project execution command or wxEmptyString if the project does not exist
  	 */
 	wxString GetProjectExecutionCommand(const wxString &projectName, wxString &wd, bool considerPauseWhenExecuting = true);

     //--------------------------- Top Level Pane Management -----------------------------
public:
  	/**
 	 * \brief test if pane_name is resides in the wxAuiManager and is visible
 	 * \param pane_name pane name to search for
 	 * \return true on success (exist in the AUI manager and visible), false otherwise
  	 */
 	bool IsPaneVisible(const wxString &pane_name);

  	/**
 	 * Show output pane and set focus to focusWin
 	 * \param focusWin tab name to set the focus
 	 * \return return true if the output pane was hidden and this function forced it to appear. false if the window was already
 	 * shown and nothing needed to be done
  	 */
 	bool ShowOutputPane(wxString focusWin = wxEmptyString, bool commit = true );

  	/**
 	 * Show the debugger pane
  	 */
 	void ShowDebuggerPane(bool commit = true);

  	/**
 	 * Show the workspace pane and set focus to focusWin
 	 * \param focusWin tab name to set the focus
  	 */
 	void ShowWorkspacePane(wxString focusWin = wxEmptyString, bool commit = true );

  	/**
 	 * Hide pane
  	 */
 	void HidePane(const wxString &paneName, bool commit = true);

  	/**
 	 * Hide/Show all panes. This function saves the current prespective and
 	 * then hides all panes, when called again, all panes are restored
  	 */
 	void TogglePanes();


    //--------------------------- Menu and Accelerator Mmgt -----------------------------
public:
	/**
	 * \brief update the menu bar accelerators
	 */
	void UpdateMenuAccelerators();

	/**
	 * \brief load accelerator table from the configuration section
	 * \param file
	 * \param map
	 */
	void LoadAcceleratorTable(const wxArrayString &files, MenuItemDataMap &map);

	void UpdateMenu(wxMenu *menu, MenuItemDataMap &accelMap, std::vector< wxAcceleratorEntry > &accelVec);

	/**
	 * \brief retrun map of the default accelerator table
	 */
	void GetDefaultAcceleratorMap(MenuItemDataMap& accelMap);

	/**
	 * \brief retrun map of the accelerator table. the StringMap maps between the actions and their accelerators
	 */
	void GetAcceleratorMap(MenuItemDataMap& accelMap);

protected:
	void DoGetAccelFiles(wxArrayString &files);
	void DumpMenu( wxMenu *menu, const wxString &label, wxString &content );


    //--------------------------- Run Program (No Debug) -----------------------------
public:
	/**
	 * return true a child program is running
	 */
	bool IsProgramRunning() const;

	/**
	 * Execute the project with no debugger
	 */
	void ExecuteNoDebug(const wxString &projectName);

	/**
	 * Kill child program which is running
	 */
	void KillProgram();

protected:
	void OnProcessEnd(wxProcessEvent &event);


    //--------------------------- Debugger Support -----------------------------
public:
	BreakptMgr* GetBreakpointsMgr() { return m_breakptsmgr; }

	void UpdateDebuggerPane();

	void SetMemory(const wxString &address, size_t count, const wxString &hex_value);

	//---------------------------------------------------
	// Debugging API
	//---------------------------------------------------

	void DbgStart(long pid = wxNOT_FOUND);
	void DbgStop();
	void DbgMarkDebuggerLine(const wxString &fileName, int lineno);
	void DbgUnMarkDebuggerLine();
	void DbgDoSimpleCommand(int cmd);
	void DbgSetFrame(int frame, int lineno);
	void DbgSetThread(long threadId);
	bool DbgCanInteract() {	return m_dbgCanInteract; }
	void DbgClearWatches();
	void DbgRestoreWatches();

	//---------------------------------------------------
    // Internal implementaion for various debugger events
	//---------------------------------------------------

	void UpdateAddLine              (const wxString &line, const bool OnlyIfLoggingOn = false);
	void UpdateFileLine             (const wxString &file, int lineno);
	void UpdateGotControl           (DebuggerReasons reason);
	void UpdateLostControl          ();
	void UpdateRemoteTargetConnected(const wxString &line);
	void UpdateTypeReolsved         (const wxString &expression, const wxString &type);
	void UpdateAsciiViewer                  (const wxString &expression, const wxString &tip);

	//---------------------------------------------------
	// Handle debugger event
	//---------------------------------------------------

	void DebuggerUpdate        ( const DebuggerEvent &event );
	void DoShowQuickWatchDialog( const DebuggerEvent &event );

    //--------------------------- Build Management -----------------------------
public:
	/**
	 * return true if a compilation is in process (either clean or build)
	 */
	bool IsBuildInProgress() const;

	/**
	 * Stop current build process
	 */
	void StopBuild();

	/**
	 * \brief add build job to the internal queue
	 * \param buildInfo
	 */
	void PushQueueCommand(const QueueCommand &buildInfo);

	/**
	 * \brief process the next build job
	 */
	void ProcessCommandQueue();

	/**
	 * \brief build the entire workspace. This operation is equal to
	 * manually right clicking on each project in the workspace and selecting
	 * 'build'
	 */
	void BuildWorkspace();

	/**
	 * \brief clean the entire workspace. This operation is equal to
	 * manually right clicking on each project in the workspace and selecting
	 * 'clean'
	 */
	void CleanWorkspace();

	/**
	 * \brief clean, followed by buid of the entire workspace. This operation is equal to
	 * manually right clicking on each project in the workspace and selecting
	 * 'clean'
	 */
	void RebuildWorkspace();

	/**
	 * \brief when building using custom build, execute the makefile generation command provided by the user
	 * \param project project to execute it for
	 */
	void RunCustomPreMakeCommand(const wxString &project);

	/**
	 * compile single file from a given
	 * given project
	 */
	void CompileFile(const wxString &project, const wxString &fileName, bool preprocessOnly = false);

	/**
	 * return true if the last buid ended successfully
	 */
	bool IsBuildEndedSuccessfully() const;

protected:
	void DoBuildProject(const QueueCommand &buildInfo);
	void DoCleanProject(const QueueCommand &buildInfo);
	void DoCustomBuild(const QueueCommand &buildInfo);
	void DoCmdWorkspace(int cmd);
	void DoSaveAllFilesBeforeBuild();
};

typedef Singleton<Manager> ManagerST;

#endif // MANAGER_H
