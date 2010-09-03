//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : project.h
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
#ifndef PROJECT_H
#define PROJECT_H

#include "wx/treectrl.h"
#include "wx/string.h"
#include <wx/xml/xml.h>
#include "wx/filename.h"
#include <tree.h>
#include "smart_ptr.h"
#include <list>
#include "serialized_object.h"
#include "project_settings.h"
#include "optionsconfig.h"
#include "localworkspace.h"

//incase we are using DLL build of wxWdigets, we need to make this class to export its
//classes
#ifndef WXDLLIMPEXP_LE_SDK
#ifdef WXMAKINGDLL_LE_SDK
#    define WXDLLIMPEXP_LE_SDK WXEXPORT
#elif defined(WXUSINGDLL_LE_SDK)
#    define WXDLLIMPEXP_LE_SDK WXIMPORT
#else /* not making nor using FNB as DLL */
#    define WXDLLIMPEXP_LE_SDK
#endif // WXMAKINGDLL_LE_SDK
#endif

struct VisualWorkspaceNode {
	wxString name;
	int type;
	wxTreeItemId itemId;
};

/**
 * \class ProjectItem
 * a node item that represents a displayable project item
 * 'Displayable' items are items that will be shown in the
 * FileView control
 *
 * \author Eran
 */
class WXDLLIMPEXP_LE_SDK ProjectItem
{
public:
	// The visible items
	enum {
		TypeVirtualDirectory,
		TypeProject,
		TypeFile,
		TypeWorkspace
	};

public:
	wxString m_key;
	wxString m_displayName;
	wxString m_file;
	int m_kind;

public:
	//---------------------------------------------------------------
	// Constructors, destructor and assignment operator
	//---------------------------------------------------------------
	ProjectItem(const wxString &key, const wxString &displayName, const wxString &file, int kind)
			: m_key(key)
			, m_displayName(displayName)
			, m_file(file)
			, m_kind(kind) {
	}

	ProjectItem() : m_key(wxEmptyString), m_displayName(wxEmptyString), m_file(wxEmptyString), m_kind(TypeProject) {}

	virtual ~ProjectItem() {}

	ProjectItem(const ProjectItem& item) {
		*this = item;
	}

	ProjectItem &operator=(const ProjectItem &item) {
		if (this == &item) {
			return *this;
		}

		m_key = item.m_key;
		m_displayName = item.m_displayName;
		m_file = item.m_file;
		m_kind = item.m_kind;
		return *this;
	}

	//-----------------------------------------
	// Setters / Getters
	//-----------------------------------------
	const wxString &GetDisplayName() const {
		return m_displayName;
	}
	const wxString &GetFile() const {
		return m_file;
	}
	int GetKind() const {
		return m_kind;
	}

	void SetDisplayName(const wxString &displayName) {
		m_displayName = displayName;
	}
	void SetFile(const wxString &file) {
		m_file = file;
	}
	void SetKind(int kind) {
		m_kind = kind;
	}

	//------------------------------------------
	// operations
	const wxString& Key() const {
		return m_key;
	}
};

// useful typedefs
typedef Tree<wxString, ProjectItem> ProjectTree;
typedef SmartPtr<ProjectTree> ProjectTreePtr;
typedef TreeNode<wxString, ProjectItem> ProjectTreeNode;

class Project;
typedef SmartPtr<Project> ProjectPtr;

/**
 * \ingroup LiteEditor
 *
 *
 * \date 04-15-2007
 *
 * \author Eran
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 *
 * \todo
 *
 * \bug
 *
 */
class WXDLLIMPEXP_LE_SDK Project
{
public:
	static const wxString STATIC_LIBRARY;
	static const wxString DYNAMIC_LIBRARY;
	static const wxString EXECUTABLE;

private:
	wxXmlDocument m_doc;
	wxFileName m_fileName;
	bool m_tranActive;
	bool m_isModified;
	std::map<wxString, wxXmlNode*> m_vdCache;
	time_t m_modifyTime;

public:
	const wxFileName &GetFileName() const {
		return m_fileName;
	}

	/**
	 * \brief copy this project and all the files under to new_path
	 * \param file_name the new path of the project
	 * \param new_name the new project name
	 * \param description the new project description
	 */
	void CopyTo(const wxString &new_path, const wxString &new_name, const wxString &description);

	/**
	 * \brief copy files (and virtual directories) from src project to this project
	 * note that this call replaces the files that exists under this project
	 * \param src
	 */
	void SetFiles(ProjectPtr src);

	//--------------------------------------------------
	// Ctor - Dtor
	//--------------------------------------------------

	// default constructor
	Project();
	virtual ~Project();

	/**
	 * \return project name
	 */
	wxString GetName() const;


	/**
	 * \brief return the project description as appears in the XML file
	 * \return project description
	 */
	wxString GetDescription() const;

	//-----------------------------------
	// Project operations
	//-----------------------------------
	/**
	 * Load project from file
	 * \param path
	 * \return
	 */
	bool Load(const wxString &path);
	/**
	 * \brief Create new project
	 * \param name project name
	 * \param description project description
	 * \param path path of the file excluding  the file name (e.g. C:\)
	 * \param projType project type: Project::STATIC_LIBRARY, Project::DYNAMIC_LIBRARY, Project::EXECUTABLE
	 * \return true on success, false otherwise
	 */
	bool Create(const wxString &name, const wxString &description, const wxString &path, const wxString &projType);

	/**
	 * Add file to the project
	 * \param fileName file full name and path
	 * \param virtualDir owner virtual directory, if the virtual directory does not exist, a new one will be created
	 *        and the file will be placed under it
	 * \return
	 */
	bool AddFile(const wxString &fileName, const wxString &virtualDir = wxEmptyString);

	/**
	 * Add file to the project - dont check for file duplication, this
	 * \param fileName file full name and path
	 * \param virtualDir owner virtual directory, if the virtual directory does not exist, a new one will be created
	 *        and the file will be placed under it
	 * \return true on success, false otherwise
	 */
	bool FastAddFile(const wxString &fileName, const wxString &virtualDir = wxEmptyString);

	/**
	 * Remove file from the project
	 * \param fileName file full path
	 * \param virtualDir owner virtual directory
	 * \return
	 */
	bool RemoveFile(const wxString &fileName, const wxString &virtualDir = wxEmptyString);

	/**
	 * Rename file from the project
	 * \param fileName file full path
	 * \param virtualDir owner virtual directory
	 * \return true on success, false otherwise
	 */
	bool RenameFile(const wxString &oldName, const wxString &virtualDir, const wxString &newName);

	/**
	 * \brief change the name of a virtual folder
	 * \param oldVdPath full path of the virtual folder
	 * \param newName the new name *only* of the virtual folder (without the path)
	 * \return true on success, false otherwise
	 */
	bool RenameVirtualDirectory(const wxString &oldVdPath, const wxString &newName);

	/**
	 * Create new virtual directory
	 * \param vdFullPath VD path to add
	 * \return
	 */
	bool CreateVirtualDir(const wxString &vdFullPath, bool mkpath = false);

	/**
	 * remove a virtual directory
	 * \param vdFullPath VD path to remove
	 * \return
	 */
	bool DeleteVirtualDir(const wxString &vdFullPath);

	/**
	 * Return list of files by a virtual directory
	 * \param vdFullPath virtual directory
	 * \param files [output] list of files under this vdFullPath. The files format are in absolute path!
	 */
	void GetFilesByVirtualDir(const wxString &vdFullPath, wxArrayString &files);

	/**
	 * Save project settings
	 */
	void Save();


	/**
	 * Return list of files in this project
	 * \param files
	 */
	void GetFiles(std::vector<wxFileName> &files, bool absPath = false);

	/**
	 * Return list of files in this project as a wxString in blank separated format.
	 * \param files
	 */
	wxString GetFiles(bool absPath = false);	

	/**
	 * Return list of files in this project - in both absolute and relative path
	 * \param files relative paths
	 * \param absFiles absolute paths
	 */
	void GetFiles(std::vector<wxFileName> &files, std::vector<wxFileName> &absFiles);

	/**
	 * Return a node pointing to any project-wide editor preferences
	 */
	wxXmlNode* GetProjectEditorOptions() const;

	/**
	 * Add or update local project options
	 */
	void SetProjectEditorOptions(LocalOptionsConfigPtr opts);

	/**
	 * Return the project build settings object by name
	 */
	ProjectSettingsPtr GetSettings() const;

	/**
	 * Add or update settings to the project
	 */
	void SetSettings(ProjectSettingsPtr settings);

	/**
	 * Update global settings to the project
	 */
	void SetGlobalSettings(BuildConfigCommonPtr settings);

	//-----------------------------------
	// visual operations
	//-----------------------------------
	ProjectTreePtr AsTree();

	/**
	 * \brief return the build order for a given configuration
	 * \param configuration
	 */
	wxArrayString GetDependencies(const wxString &configuration) const;

	/**
	 * \brief set the dependencies for this project for a given configuration
	 * \param deps
	 * \param configuration
	 */
	void SetDependencies(wxArrayString &deps, const wxString &configuration);

	/**
	 * Return true if a file already exist under the project
	 */
	bool IsFileExist(const wxString &fileName);

	/**
	 * \brief return true of the project was modified (in terms of files removed/added)
	 */
	bool IsModified();

	/**
	 * \brief
	 */
	void SetModified(bool mod);

	// Transaction support to reduce overhead of disk writing
	void BeginTranscation() {
		m_tranActive = true;
	}
	void CommitTranscation() {
		Save();
	}
	bool InTransaction() const {
		return m_tranActive;
	}

	wxString GetVDByFileName(const wxString &file);

	/**
	 * \brief return Tree representation of all virtual folders of this project
	 * \return tree node. return NULL if no virtual folders exist
	 */
	TreeNode<wxString, VisualWorkspaceNode>* GetVirtualDirectories(TreeNode<wxString, VisualWorkspaceNode>* workspace);

	/**
	 * @brief return the user saved information for custom data
	 * @param name the object key
	 * @param obj [output] container for the output
	 * @return true on success.
	 */
	bool GetUserData(const wxString &name, SerializedObject *obj);

	/**
	 * @brief save user data in the project settings
	 * @param name the name under which the data is to be saved
	 * @param obj the data
	 * @return true on success.
	 */
	bool SetUserData(const wxString &name, SerializedObject *obj);

	/**
	 * @brief set the project internal type (usually used to indicate internal types for the project
	 * like 'GUI' or 'UnitTest++' etc.
	 * @param internalType
	 */
	void SetProjectInternalType(const wxString &internalType);
	/**
	 * @brief return the project internal type
	 * @return
	 */
	wxString GetProjectInternalType() const;

	/**
	 * @brief return the plugins' data. This data is copied when using 'save project as template' functionality
	 * @param plugin plugin name
	 * @return plugins data or wxEmptyString
	 */
	wxString GetPluginData(const wxString &pluginName);

	/**
	 * @brief set the plugin data. This data is copied when using 'save project as template' functionality
	 * @param plugin the plugins' name
	 * @param data the data
	 */
	void SetPluginData(const wxString &pluginName, const wxString &data);

	/**
	 * @brief get all plugins data as map of plugin=value pair
	 * @param pluginsDataMap [output]
	 */
	void GetAllPluginsData(std::map<wxString, wxString> &pluginsDataMap);

	/**
	 * @brief set all plugins data as map of plugin=value pair
	 * @param pluginsDataMap
	 */
	void SetAllPluginsData(const std::map<wxString, wxString> &pluginsDataMap, bool saveToFile = true);

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
	time_t GetProjectLastModifiedTime() const {
		return m_modifyTime;
	}
	void SetProjectLastModifiedTime(time_t modificationTime) {
		m_modifyTime = modificationTime;
	}

private:

	void DoGetVirtualDirectories(wxXmlNode* parent, TreeNode<wxString, VisualWorkspaceNode>* tree);
	wxXmlNode *FindFile(wxXmlNode* parent, const wxString &file);

	// Recursive helper function
	void RecursiveAdd(wxXmlNode *xmlNode, ProjectTreePtr &ptp, ProjectTreeNode *nodeParent);

	// Return the node representing a virtual dir by name
	// if no such virtual dir exist, create it.
	wxXmlNode *GetVirtualDir(const wxString &vdFullPath);

	// Create virtual dir and return its xml node
	wxXmlNode *CreateVD(const wxString &vdFullPath, bool mkpath = false);

	void GetFiles(wxXmlNode *parent, std::vector<wxFileName> &files, bool absPath = false);
	void GetFiles(wxXmlNode *parent, std::vector<wxFileName>& files, std::vector<wxFileName>& absFiles);
	/**
	 * Return list of projects that this projects depends on
	 */
	wxArrayString GetDependencies() const;
	
	/**
	 * @brief convert all the files paths to Unix format
	 * @param parent
	 */
	void ConvertToUnixFormat(wxXmlNode *parent);
	
	bool SaveXmlFile();
};

class ProjectData
{
public:
	wxString m_name;	//< project name
	wxString m_path;	//< project directoy
	ProjectPtr m_srcProject;
	wxString m_cmpType; //< Project compiler type
};

//-----------------------------------------------------------------
// This class is related to the visual representation of the class
// projects in the tree view
//-----------------------------------------------------------------
/**
 * Class FilewViewTreeItemData, a user defined class which stores a node private information
 *
 * \date 12-04-2007
 * \author Eran
 *
 */
class FilewViewTreeItemData : public wxTreeItemData
{
	ProjectItem m_item;
public:
	FilewViewTreeItemData(const ProjectItem &item) : m_item(item) { }
	const ProjectItem &GetData() const {
		return m_item;
	}

	void SetDisplayName(const wxString &displayName) {
		m_item.SetDisplayName(displayName);
	}

	void SetFile(const wxString &file) {
		m_item.SetFile(file);
	}
};

#endif // PROJECT_H
