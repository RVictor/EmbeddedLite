/**
  \file project.cpp

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#include "project.h"
#include <wx/app.h>
#include <wx/log.h>
#include "fileextmanager.h"
#include "xmlutils.h"
#include <wx/tokenzr.h>
#include "wx/arrstr.h"
#include "dirsaver.h"
#include "globals.h"
#include "macros.h"
#include "wx_xml_compatibility.h"
#include "elconfig.h"

const wxString Project::STATIC_LIBRARY = wxT("Static Library");
const wxString Project::DYNAMIC_LIBRARY = wxT("Dynamic Library");
const wxString Project::EXECUTABLE = wxT("Executable");


Project::Project()
		: m_tranActive(false)
		, m_isModified(false)
{
}

Project::~Project()
{
	m_vdCache.clear();
}

bool Project::Create(const wxString &name, const wxString &description, const wxString &path, const wxString &projType)
{
	m_vdCache.clear();

	m_fileName = path + wxFileName::GetPathSeparator() + name + wxT(".") + EL_PROJECT_EXT;
	m_fileName.MakeAbsolute();

	wxXmlNode *root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("EmbeddedLite_Project"));
	m_doc.SetRoot(root);
	m_doc.GetRoot()->AddProperty(wxT("Name"), name);

	wxXmlNode *descNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Description"));
	XmlUtils::SetNodeContent(descNode, description);
	m_doc.GetRoot()->AddChild(descNode);

	// Create the default virtual directories
	wxXmlNode *srcNode = NULL, *headNode = NULL;

	srcNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("VirtualDirectory"));
	srcNode->AddProperty(wxT("Name"), wxT("src"));
	m_doc.GetRoot()->AddChild(srcNode);

	headNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("VirtualDirectory"));
	headNode->AddProperty(wxT("Name"), wxT("include"));
	m_doc.GetRoot()->AddChild(headNode);

	//creae dependencies node
	wxXmlNode *depNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Dependencies"));
	root->AddChild(depNode);

	SaveXmlFile();
	//create build settings
	SetSettings(new ProjectSettings(NULL));
	ProjectSettingsPtr settings = GetSettings();
	settings->SetProjectType(projType);
	SetSettings(settings);
	SetModified(true);
	return true;
}

bool Project::Load(const wxString &path)
{
	if ( !m_doc.Load(path) ) {
		return false;
	}
	
	ConvertToUnixFormat(m_doc.GetRoot());
	
	// Workaround WX bug: load the plugins data (GetAllPluginsData will strip any trailing whitespaces)
	// and then set them back
	std::map<wxString, wxString> pluginsData;
	GetAllPluginsData(pluginsData);
	SetAllPluginsData(pluginsData, false);

	m_vdCache.clear();

	m_fileName = path;
	m_fileName.MakeAbsolute();
	SetModified(true);
	SetProjectLastModifiedTime(GetFileLastModifiedTime());

	return true;
}

wxXmlNode *Project::GetVirtualDir(const wxString &vdFullPath)
{
	wxStringTokenizer tkz(vdFullPath, wxT(":"));

	// test the cache
	std::map<wxString, wxXmlNode*>::iterator iter = m_vdCache.find(vdFullPath);
	if(iter != m_vdCache.end()){
		return iter->second;
	}

	wxXmlNode *parent = m_doc.GetRoot();
	while ( tkz.HasMoreTokens() ) {
		parent = XmlUtils::FindNodeByName(parent, wxT("VirtualDirectory"), tkz.GetNextToken());
		if ( !parent ) {
			m_vdCache[vdFullPath] = NULL;
			return NULL;
		}
	}
	// cache the result
	m_vdCache[vdFullPath] = parent;
	return parent;
}

wxXmlNode *Project::CreateVD(const wxString &vdFullPath, bool mkpath)
{
	wxXmlNode *oldVd = GetVirtualDir(vdFullPath);
	if ( oldVd ) {
		// VD already exist
		return oldVd;
	}

	wxStringTokenizer tkz(vdFullPath, wxT(":"));

	wxXmlNode *parent = m_doc.GetRoot();
	size_t count = tkz.CountTokens();
	for (size_t i=0; i<count-1; i++) {
		wxString token = tkz.NextToken();
		wxXmlNode *p = XmlUtils::FindNodeByName(parent, wxT("VirtualDirectory"), token);
		if ( !p ) {
			if ( mkpath ) {

				//add the node
				p = new wxXmlNode(parent, wxXML_ELEMENT_NODE, wxT("VirtualDirectory"));
				p->AddProperty(wxT("Name"), token);

			} else {
				return NULL;
			}
		}
		parent = p;
	}

	wxXmlNode *node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("VirtualDirectory"));
	node->AddProperty(wxT("Name"), tkz.GetNextToken());

	parent->AddChild(node);

	//if not in transaction save the changes
	if (!InTransaction()) {
		SaveXmlFile();
	}

	// cache the result
	m_vdCache[vdFullPath] = node;

	return node;
}

bool Project::IsFileExist(const wxString &fileName)
{
	//find the file under this node
	// Convert the file path to be relative to
	// the project path
	DirSaver ds;

	::wxSetWorkingDirectory(m_fileName.GetPath());
	wxFileName tmp(fileName);
	tmp.MakeRelativeTo(m_fileName.GetPath());

	std::vector<wxFileName> files;
	GetFiles(files);

	for (size_t i=0; i<files.size(); i++) {
		if (files.at(i).GetFullPath().CmpNoCase(tmp.GetFullPath(wxPATH_UNIX)) == 0) {
			return true;
		}
	}
	return false;
}

bool Project::AddFile(const wxString &fileName, const wxString &virtualDirPath)
{
	wxXmlNode *vd = GetVirtualDir(virtualDirPath);
	if ( !vd ) {
		return false;
	}

	// Convert the file path to be relative to
	// the project path
	DirSaver ds;

	::wxSetWorkingDirectory(m_fileName.GetPath());
	wxFileName tmp(fileName);
	tmp.MakeRelativeTo(m_fileName.GetPath());

	// if we already have a file with the same name, return false
	if (this->IsFileExist(fileName)) {
		return false;
	}

	wxXmlNode *node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("File"));
	node->AddProperty(wxT("Name"), tmp.GetFullPath(wxPATH_UNIX));
	vd->AddChild(node);
	if (!InTransaction()) {
		SaveXmlFile();
	}
	SetModified(true);
	return true;
}

bool Project::CreateVirtualDir(const wxString &vdFullPath, bool mkpath)
{
	return CreateVD(vdFullPath, mkpath) != NULL;
}

bool Project::DeleteVirtualDir(const wxString &vdFullPath)
{
	wxXmlNode *vd = GetVirtualDir(vdFullPath);
	if ( vd ) {
		wxXmlNode *parent = vd->GetParent();
		if ( parent ) {
			parent->RemoveChild( vd );
		}

		// remove the entry from the cache
		std::map<wxString, wxXmlNode*>::iterator iter = m_vdCache.find(vdFullPath);
		if(iter != m_vdCache.end()){
			m_vdCache.erase(iter);
		}

		delete vd;
		SetModified(true);
		return SaveXmlFile();
	}
	return false;
}

bool Project::RemoveFile(const wxString &fileName, const wxString &virtualDir)
{
	wxXmlNode *vd = GetVirtualDir(virtualDir);
	if ( !vd ) {
		return false;
	}

	// Convert the file path to be relative to
	// the project path
	DirSaver ds;

	::wxSetWorkingDirectory(m_fileName.GetPath());
	wxFileName tmp(fileName);
	tmp.MakeRelativeTo(m_fileName.GetPath());

	wxXmlNode *node = XmlUtils::FindNodeByName(vd, wxT("File"), tmp.GetFullPath(wxPATH_UNIX));
	if ( node ) {
		node->GetParent()->RemoveChild( node );
		delete node;
	} else {
		wxLogMessage(wxT("Failed to remove file %s from project"), tmp.GetFullPath(wxPATH_UNIX).c_str());
	}
	SetModified(true);
	return SaveXmlFile();
}

wxString Project::GetName() const
{
	return m_doc.GetRoot()->GetPropVal(wxT("Name"), wxEmptyString);
}

ProjectTreePtr Project::AsTree()
{
	ProjectItem item(GetName(), GetName(), GetFileName().GetFullPath(), ProjectItem::TypeProject);
	ProjectTreePtr ptp(new ProjectTree(item.Key(), item));

	wxXmlNode *child = m_doc.GetRoot()->GetChildren();
	while ( child ) {
		RecursiveAdd(child, ptp, ptp->GetRoot());
		child = child->GetNext();
	}
	return ptp;
}

void Project::RecursiveAdd(wxXmlNode *xmlNode, ProjectTreePtr &ptp, ProjectTreeNode *nodeParent)
{
	// Build the key for this node
	std::list<wxString> nameList;

	wxXmlNode *parent = xmlNode->GetParent();
	while ( parent ) {
		nameList.push_front(parent->GetPropVal(wxT("Name"), wxEmptyString));
		parent = parent->GetParent();
	}

	wxString key;
	for (size_t i=0; i<nameList.size(); i++) {
		key += nameList.front();
		key += wxT(":");
		nameList.pop_front();
	}
	key += xmlNode->GetPropVal(wxT("Name"), wxEmptyString);

	// Create the tree node data
	ProjectItem item;
	if ( xmlNode->GetName() == wxT("Project") ) {
		item = ProjectItem(key, xmlNode->GetPropVal(wxT("Name"), wxEmptyString), wxEmptyString, ProjectItem::TypeProject);
	} else if ( xmlNode->GetName() == wxT("VirtualDirectory") ) {
		item = ProjectItem(key, xmlNode->GetPropVal(wxT("Name"), wxEmptyString), wxEmptyString, ProjectItem::TypeVirtualDirectory);
	} else if ( xmlNode->GetName() == wxT("File") ) {
		wxFileName filename(xmlNode->GetPropVal(wxT("Name"), wxEmptyString));
		//convert this file name to absolute path
		DirSaver ds;
		::wxSetWorkingDirectory(m_fileName.GetPath());
		filename.MakeAbsolute();
		item = ProjectItem(key, filename.GetFullName(), filename.GetFullPath(), ProjectItem::TypeFile);
	} else {
		// un-recognised or not viewable item in the tree,
		// skip it and its children
		return;
	}

	ProjectTreeNode *newNode = ptp->AddChild(item.Key(), item, nodeParent);
	// This node has children, add them as well
	wxXmlNode *children = xmlNode->GetChildren();

	while ( children ) {
		RecursiveAdd(children, ptp, newNode);
		children = children->GetNext();
	}
	SetModified(true);
}

bool Project::SaveXmlFile()
{
	bool ok = m_doc.Save(m_fileName.GetFullPath());
	SetProjectLastModifiedTime(GetFileLastModifiedTime());

	return ok;
}

void Project::Save()
{
	m_tranActive = false;
	if ( m_doc.IsOk() )
		SaveXmlFile();
}

void Project::GetFilesByVirtualDir(const wxString &vdFullPath, wxArrayString &files)
{
	wxXmlNode *vd = GetVirtualDir(vdFullPath);
	if ( vd ) {
		wxXmlNode *child = vd->GetChildren();
		while ( child ) {
			if ( child->GetName() == wxT("File")) {
				wxFileName fileName(
				    child->GetPropVal(wxT("Name"), wxEmptyString)
				);
				fileName.MakeAbsolute(m_fileName.GetPath());
				files.Add(fileName.GetFullPath());
			}
			child = child->GetNext();
		}
	}
}

wxString Project::GetFiles(bool absPath)
{
	std::vector<wxFileName> files;
	GetFiles(files,absPath);

	wxString temp;
	for (size_t i = 0; i < files.size(); i++)
		temp << wxT("\"") << files.at(i).GetFullPath() << wxT("\" ");
	
	if(temp.IsEmpty() == false)
		temp.RemoveLast();
	
	return temp;
}

void Project::GetFiles(std::vector<wxFileName> &files, bool absPath)
{
	if (absPath) {
		DirSaver ds;
		::wxSetWorkingDirectory(m_fileName.GetPath());

		GetFiles(m_doc.GetRoot(), files, true);
	} else {
		GetFiles(m_doc.GetRoot(), files, false);
	}
}

void Project::GetFiles(wxXmlNode *parent, std::vector<wxFileName> &files, bool absPath)
{
	if ( !parent ) {
		return;
	}

	wxXmlNode *child = parent->GetChildren();
	while (child) {
		if (child->GetName() == wxT("File")) {
			wxString fileName = child->GetPropVal(wxT("Name"), wxEmptyString);
			wxFileName tmp(fileName);
			if (absPath) {
				tmp.MakeAbsolute();
			}
			files.push_back(tmp);
		} else if (child->GetChildren()) {// we could also add a check for VirtualDirectory only
			GetFiles(child, files, absPath);
		}
		child = child->GetNext();
	}
}

wxXmlNode* Project::GetProjectEditorOptions() const
{
	return XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("Options"));
}

void Project::SetProjectEditorOptions(LocalOptionsConfigPtr opts)
{
	wxXmlNode *parent = m_doc.GetRoot();
	wxXmlNode *oldOptions = XmlUtils::FindFirstByTagName(parent, wxT("Options"));
	if (oldOptions) {
		oldOptions->GetParent()->RemoveChild(oldOptions);
		delete oldOptions;
	}
	parent->AddChild(opts->ToXml());
	SaveXmlFile();
}

ProjectSettingsPtr Project::GetSettings() const
{
	wxXmlNode *node = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("Settings"));
	return new ProjectSettings(node);
}

void Project::SetSettings(ProjectSettingsPtr settings)
{
	wxXmlNode *oldSettings = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("Settings"));
	if (oldSettings) {
		oldSettings->GetParent()->RemoveChild(oldSettings);
		delete oldSettings;
	}
	m_doc.GetRoot()->AddChild(settings->ToXml());
	SaveXmlFile();
}

void Project::SetGlobalSettings(BuildConfigCommonPtr globalSettings)
{
	wxXmlNode *settings = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("Settings"));
	wxXmlNode *oldSettings = XmlUtils::FindFirstByTagName(settings, wxT("GlobalSettings"));
	if (oldSettings) {
		oldSettings->GetParent()->RemoveChild(oldSettings);
		delete oldSettings;
	}
	settings->AddChild(globalSettings->ToXml());
	SaveXmlFile();
}

wxArrayString Project::GetDependencies() const
{
	wxArrayString result;
	wxXmlNode *node = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("Dependencies"));
	if (node) {
		wxXmlNode *child = node->GetChildren();
		while (child) {
			if (child->GetName() == wxT("Project")) {
				result.Add(XmlUtils::ReadString(child, wxT("Name")));
			}
			child = child->GetNext();
		}
	}
	return result;
}

void Project::SetModified(bool mod)
{
	m_isModified = mod;
}

bool Project::IsModified()
{
	return m_isModified;
}

wxString Project::GetDescription() const
{
	wxXmlNode *root = m_doc.GetRoot();
	if (root) {
		wxXmlNode *node = XmlUtils::FindFirstByTagName(root, wxT("Description"));
		if (node) {
			return node->GetNodeContent();
		}
	}
	return wxEmptyString;
}

void Project::CopyTo(const wxString& new_path, const wxString& new_name, const wxString& description)
{
	// first save the xml document to the destination folder

	wxString newFile = new_path + new_name + wxT(".") + EL_PROJECT_EXT;
	if ( !m_doc.Save( newFile ) ) {
		return;
	}

	// load the new xml and modify it
	wxXmlDocument doc;
	if ( !doc.Load( newFile ) ) {
		return;
	}

	// update the 'Name' property
	XmlUtils::UpdateProperty(doc.GetRoot(), wxT("Name"), new_name);

	// set description
	wxXmlNode *descNode(NULL);

	// update the description
	descNode = XmlUtils::FindFirstByTagName(doc.GetRoot(), wxT("Description"));
	if (!descNode) {
		descNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Description"));
		doc.GetRoot()->AddChild(descNode);
	}
	XmlUtils::SetNodeContent(descNode, description);

	// Remove the 'Dependencies'
	wxXmlNode *deps = doc.GetRoot()->GetChildren();
	while(deps) {
		if(deps->GetName() == wxT("Dependencies")) {
			doc.GetRoot()->RemoveChild(deps);
			delete deps;

			// restart the search from the begining
			deps = doc.GetRoot()->GetChildren();

		} else {
			// try next child
			deps = deps->GetNext();
		}
	}

	// add an empty deps node
	deps = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Dependencies"));
	doc.GetRoot()->AddChild(deps);

	// Remove virtual folders
	wxXmlNode *vd = XmlUtils::FindFirstByTagName(doc.GetRoot(), wxT("VirtualDirectory"));
	while (vd) {
		doc.GetRoot()->RemoveChild( vd );
		delete vd;
		vd = XmlUtils::FindFirstByTagName(doc.GetRoot(), wxT("VirtualDirectory"));
	}

	// add all files under this path
	std::vector<wxFileName> files;
	GetFiles(files, true);

	wxXmlNode *srcNode(NULL);
	wxXmlNode *headNode(NULL);
	wxXmlNode *rcNode(NULL);

	// copy the files to their new location
	for (size_t i=0; i<files.size(); i++) {
		wxFileName fn = files.at(i);
		wxCopyFile(fn.GetFullPath(), new_path + wxT("/") + fn.GetFullName());

		wxXmlNode *file_node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("File"));
		file_node->AddProperty(wxT("Name"), fn.GetFullName());

		switch ( FileExtManager::GetType( fn.GetFullName() ) ) {
		case FileExtManager::TypeSourceC:
		case FileExtManager::TypeSourceCpp:

			// source file
			if ( !srcNode ) {
				srcNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("VirtualDirectory"));
				srcNode->AddProperty(wxT("Name"), wxT("src"));
				doc.GetRoot()->AddChild(srcNode);
			}
			srcNode->AddChild(file_node);
			break;

		case FileExtManager::TypeHeader:
			// header file
			if (!headNode) {
				headNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("VirtualDirectory"));
				headNode->AddProperty(wxT("Name"), wxT("include"));
				doc.GetRoot()->AddChild(headNode);
			}
			headNode->AddChild(file_node);
			break;

		default:
			// resource file
			if ( !rcNode ) {
				rcNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("VirtualDirectory"));
				rcNode->AddProperty(wxT("Name"), wxT("resources"));
				doc.GetRoot()->AddChild(rcNode);
			}
			rcNode->AddChild(file_node);
			break;
		}
	}

	doc.Save(newFile);
}

void Project::SetFiles(ProjectPtr src)
{
	// first remove all the virtual directories from this project
	// Remove virtual folders
	wxXmlNode *vd = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("VirtualDirectory"));
	while (vd) {
		m_doc.GetRoot()->RemoveChild( vd );
		delete vd;
		vd = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("VirtualDirectory"));
	}

	// copy the virtual directories from the src project
	wxXmlNode *child = src->m_doc.GetRoot()->GetChildren();
	while ( child ) {
		if ( child->GetName() == wxT("VirtualDirectory") ) {
			// create a new VirtualDirectory like this one
			wxXmlNode *newNode = new wxXmlNode(*child);
			m_doc.GetRoot()->AddChild(newNode);
		}
		child = child->GetNext();
	}
	SaveXmlFile();
}

bool Project::RenameFile(const wxString& oldName, const wxString& virtualDir, const wxString& newName)
{
	wxXmlNode *vd = GetVirtualDir(virtualDir);
	if ( !vd ) {
		return false;
	}

	// Convert the file path to be relative to
	// the project path
	DirSaver ds;

	::wxSetWorkingDirectory(m_fileName.GetPath());
	wxFileName tmp(oldName);
	tmp.MakeRelativeTo(m_fileName.GetPath());

	wxXmlNode *node = XmlUtils::FindNodeByName(vd, wxT("File"), tmp.GetFullPath(wxPATH_UNIX));
	if ( node ) {
		// update the new name
		tmp.SetFullName(newName);
		XmlUtils::UpdateProperty(node, wxT("Name"), tmp.GetFullPath(wxPATH_UNIX));
	}

	SetModified(true);
	return SaveXmlFile();
}

wxString Project::GetVDByFileName(const wxString& file)
{
	//find the file under this node
	// Convert the file path to be relative to
	// the project path
	DirSaver ds;

	::wxSetWorkingDirectory(m_fileName.GetPath());
	wxFileName tmp(file);
	tmp.MakeRelativeTo(m_fileName.GetPath());

	wxString path(wxEmptyString);
	wxXmlNode *fileNode = FindFile(m_doc.GetRoot(), tmp.GetFullPath(wxPATH_UNIX));

	if (fileNode) {
		wxXmlNode *parent = fileNode->GetParent();
		while ( parent ) {
			if (parent->GetName() == wxT("VirtualDirectory")) {
				path.Prepend(parent->GetPropVal(wxT("Name"), wxEmptyString));
				path.Prepend(wxT(":"));
			} else {
				break;
			}
			parent = parent->GetParent();
		}
	}
	wxString trunc_path(path);
	path.StartsWith(wxT(":"), &trunc_path);
	return trunc_path;
}

wxXmlNode* Project::FindFile(wxXmlNode* parent, const wxString& file)
{
	wxXmlNode *child = parent->GetChildren();
	while ( child ) {
		wxString name = child->GetName();
		if (name == wxT("File") && child->GetPropVal(wxT("Name"), wxEmptyString) == file) {
			return child;
		}

		if (child->GetName() == wxT("VirtualDirectory")) {
			wxXmlNode *n = FindFile(child, file);
			if (n) {
				return n;
			}
		}
		child = child->GetNext();
	}
	return NULL;
}

bool Project::RenameVirtualDirectory(const wxString& oldVdPath, const wxString& newName)
{
	wxXmlNode *vdNode = GetVirtualDir(oldVdPath);
	if (vdNode) {
		XmlUtils::UpdateProperty(vdNode, wxT("Name"), newName);
		return SaveXmlFile();
	}
	return false;
}

wxArrayString Project::GetDependencies(const wxString& configuration) const
{
	wxArrayString result;

	// dependencies are located directly under the root level
	wxXmlNode *node = m_doc.GetRoot()->GetChildren();
	while (node) {
		if ( node->GetName() == wxT("Dependencies") && node->GetPropVal(wxT("Name"), wxEmptyString) == configuration) {
			// we have our match
			wxXmlNode *child = node->GetChildren();
			while (child) {
				if (child->GetName() == wxT("Project")) {
					result.Add(XmlUtils::ReadString(child, wxT("Name")));
				}
				child = child->GetNext();
			}
			return result;
		}
		node = node->GetNext();
	}

	// if we are here, it means no match for the given configuration
	// return the default dependencies
	return GetDependencies();
}

void Project::SetDependencies(wxArrayString& deps, const wxString& configuration)
{
	// first try to locate the old node
	wxXmlNode *node = m_doc.GetRoot()->GetChildren();
	while (node) {
		if ( node->GetName() == wxT("Dependencies") && node->GetPropVal(wxT("Name"), wxEmptyString) == configuration) {
			// we have our match
			node->GetParent()->RemoveChild(node);
			delete node;
			break;
		}
		node = node->GetNext();
	}

	// create new dependencies node
	node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Dependencies"));
	node->AddProperty(wxT("Name"), configuration);
	m_doc.GetRoot()->AddChild(node);

	//create a node for each dependency in the array
	for (size_t i=0; i<deps.GetCount(); i++) {
		wxXmlNode *child = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Project"));
		child->AddProperty(wxT("Name"), deps.Item(i));
		node->AddChild(child);
	}

	//save changes
	SaveXmlFile();
	SetModified(true);
}

void Project::GetFiles(std::vector<wxFileName>& files, std::vector<wxFileName>& absFiles)
{
	DirSaver ds;
	::wxSetWorkingDirectory(m_fileName.GetPath());
	GetFiles(m_doc.GetRoot(), files, absFiles);
}

void Project::GetFiles(wxXmlNode *parent, std::vector<wxFileName>& files, std::vector<wxFileName>& absFiles)
{
	if ( !parent ) {
		return;
	}

	wxXmlNode *child = parent->GetChildren();
	while (child) {
		if (child->GetName() == wxT("File")) {
			wxString fileName = child->GetPropVal(wxT("Name"), wxEmptyString);
			wxFileName tmp(fileName);

			// append the file as it appears
			files.push_back(tmp);

			// convert to absolute path
			tmp.MakeAbsolute();
			absFiles.push_back(tmp);

		} else if (child->GetChildren()) {// we could also add a check for VirtualDirectory only
			GetFiles(child, files, absFiles);
		}
		child = child->GetNext();
	}
}

bool Project::FastAddFile(const wxString& fileName, const wxString& virtualDir)
{
	wxXmlNode *vd = GetVirtualDir(virtualDir);
	if ( !vd ) {
		return false;
	}

	// Convert the file path to be relative to
	// the project path
	DirSaver ds;

	::wxSetWorkingDirectory(m_fileName.GetPath());
	wxFileName tmp(fileName);
	tmp.MakeRelativeTo(m_fileName.GetPath());

	wxXmlNode *node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("File"));
	node->AddProperty(wxT("Name"), tmp.GetFullPath(wxPATH_UNIX));
	vd->AddChild(node);
	if (!InTransaction()) {
		SaveXmlFile();
	}
	SetModified(true);
	return true;
}

void Project::DoGetVirtualDirectories(wxXmlNode* parent, TreeNode<wxString, VisualWorkspaceNode>* tree)
{
	wxXmlNode *child = parent->GetChildren();
	while(child){
		if(child->GetName() == wxT("VirtualDirectory")){

			VisualWorkspaceNode data;
			data.name = XmlUtils::ReadString(child, wxT("Name"));
			data.type = ProjectItem::TypeVirtualDirectory;

			TreeNode<wxString, VisualWorkspaceNode>* node = new TreeNode<wxString, VisualWorkspaceNode>(data.name, data, tree);
			tree->AddChild(node);

			// test to see if it has children
			if(child->GetChildren()){
				DoGetVirtualDirectories(child, node);
			}
		}
		child = child->GetNext();
	}
}

TreeNode<wxString, VisualWorkspaceNode>* Project::GetVirtualDirectories(TreeNode<wxString, VisualWorkspaceNode> *workspace)
{
	VisualWorkspaceNode data;
	data.name = GetName();
	data.type = ProjectItem::TypeProject;

	TreeNode<wxString, VisualWorkspaceNode> *parent = new TreeNode<wxString, VisualWorkspaceNode>(GetName(), data, workspace);
	DoGetVirtualDirectories(m_doc.GetRoot(), parent);
	workspace->AddChild(parent);
	return parent;
}

bool Project::GetUserData(const wxString& name, SerializedObject* obj)
{
	if(!m_doc.IsOk()){
		return false;
	}

	Archive arch;
	wxXmlNode *userData = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("UserData"));
	if(userData){
		wxXmlNode *dataNode = XmlUtils::FindNodeByName(userData, wxT("Data"), name);
		if(dataNode){
			arch.SetXmlNode(dataNode);
			obj->DeSerialize(arch);
			return true;
		}
	}
	return false;
}

bool Project::SetUserData(const wxString& name, SerializedObject* obj)
{
	if(!m_doc.IsOk()){
		return false;
	}

	Archive arch;

	// locate the 'UserData' node
	wxXmlNode *userData = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("UserData"));
	if( !userData ) {
		userData = new wxXmlNode(m_doc.GetRoot(), wxXML_ELEMENT_NODE, wxT("UserData"));
	}

	// try to find a previous data stored under the same name, if we succeed - remove it
	wxXmlNode *dataNode = XmlUtils::FindNodeByName(userData, wxT("Data"), name);
	if(dataNode){
		// remove old node
		userData->RemoveChild(dataNode);
		delete dataNode;
	}

	// create a new node and set the userData node as the parent
	dataNode = new wxXmlNode(userData, wxXML_ELEMENT_NODE, wxT("Data"));
	dataNode->AddProperty(wxT("Name"), name);

	// serialize the data
	arch.SetXmlNode(dataNode);
	obj->Serialize(arch);
	return SaveXmlFile();
}

void Project::SetProjectInternalType(const wxString& internalType)
{
	XmlUtils::UpdateProperty(m_doc.GetRoot(), wxT("InternalType"), internalType);
}

wxString Project::GetProjectInternalType() const
{
	return m_doc.GetRoot()->GetPropVal(wxT("InternalType"), wxEmptyString);
}

void Project::GetAllPluginsData(std::map<wxString, wxString>& pluginsDataMap)
{
	if(!m_doc.IsOk()){
		return;
	}

	// locate the 'Plugins' node
	wxXmlNode *plugins = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("Plugins"));
	if( !plugins ) {
		return;
	}

	wxXmlNode *child = plugins->GetChildren();
	while( child ) {
		if( child->GetName() == wxT("Plugin") ) {
			// get the content
			wxString content = child->GetNodeContent();
			// overcome bug in WX where CDATA content comes out with extra \n and 4xspaces
			content.Trim().Trim(false);
			pluginsDataMap[child->GetPropVal(wxT("Name"), wxEmptyString)] = content;
		}
		child = child->GetNext();
	}
}

wxString Project::GetPluginData(const wxString& pluginName)
{
	if(!m_doc.IsOk()){
		return wxEmptyString;
	}

	// locate the 'Plugins' node
	wxXmlNode *plugins = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("Plugins"));
	if( !plugins ) {
		return wxEmptyString;
	}

	// find the node and return its content
	wxXmlNode *dataNode = XmlUtils::FindNodeByName(plugins, wxT("Plugin"), pluginName);
	if( dataNode ){
		return dataNode->GetNodeContent().Trim().Trim(false);
	}
	return wxEmptyString;
}

void Project::SetPluginData(const wxString& pluginName, const wxString& data)
{
	if(!m_doc.IsOk()){
		return ;
	}

	// locate the 'Plugins' node
	wxXmlNode *plugins = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("Plugins"));
	if( !plugins ) {
		plugins = new wxXmlNode(m_doc.GetRoot(), wxXML_ELEMENT_NODE, wxT("Plugins"));
	}

	wxXmlNode *plugin = XmlUtils::FindNodeByName(plugins, wxT("Plugin"), pluginName);
	if( !plugin ) {
		plugin = new wxXmlNode(plugins, wxXML_ELEMENT_NODE, wxT("Plugin"));
		plugin->AddProperty(wxT("Name"), pluginName);
	}

	wxString tmpData ( data );
	tmpData.Trim().Trim(false);
	XmlUtils::SetCDATANodeContent(plugin, tmpData);
	SaveXmlFile();
}


void Project::SetAllPluginsData(const std::map<wxString, wxString>& pluginsDataMap, bool saveToFile /* true */)
{
	if(!m_doc.IsOk()){
		return;
	}

	// locate the 'Plugins' node
	wxXmlNode *plugins = XmlUtils::FindFirstByTagName(m_doc.GetRoot(), wxT("Plugins"));
	if( plugins ) {
		m_doc.GetRoot()->RemoveChild( plugins );
		delete plugins;
	}

	std::map<wxString, wxString>::const_iterator iter = pluginsDataMap.begin();
	for(; iter != pluginsDataMap.end(); iter ++) {
		SetPluginData( iter->first, iter->second );
	}

	if ( saveToFile ) {
		SaveXmlFile();
	}
}

time_t Project::GetFileLastModifiedTime() const
{
	return GetFileModificationTime(GetFileName());
}

void Project::ConvertToUnixFormat(wxXmlNode* parent)
{
	if(!parent)
		return;
	
	wxXmlNode *child = parent->GetChildren();
	while(child) {
		
		if(child->GetName() == wxT("VirtualDirectory")) {
			
			ConvertToUnixFormat(child);
			
		} else if(child->GetName() == wxT("File")) {
			
			wxXmlProperty *props = child->GetProperties();
			// Convert the path to unix format
			while ( props ) {
				if(props->GetName() == wxT("Name")) {
					wxString val = props->GetValue();
					val.Replace(wxT("\\"), wxT("/"));
					props->SetValue(val);
					break;
				}
				props = props->GetNext();
			}
		}
		child = child->GetNext();
	}
}
