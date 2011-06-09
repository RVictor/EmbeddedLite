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
#include "project_settings.h"
#include "xmlutils.h"
#include "project.h"
#include "macros.h"
#include "wx_xml_compatibility.h"

ProjectSettings::ProjectSettings(wxXmlNode *node)
{
	if (node) {
		// load configurations
		m_projectType = XmlUtils::ReadString(node, wxT("Type"));
		wxXmlNode *child = node->GetChildren();
		while (child) {
			if (child->GetName() == wxT("Configuration")) {
				wxString configName = XmlUtils::ReadString(child, wxT("Name"));
				m_configs.insert(std::pair<wxString, BuildConfigPtr>(configName, new BuildConfig(child)));
			}
			else if (child->GetName() == wxT("GlobalSettings")) {
				m_globalSettings = new BuildConfigCommon(child, wxT("GlobalSettings"));
			}
			child = child->GetNext();
		}
	} else {
		//create new settings with default values
		m_projectType = Project::STATIC_LIBRARY;
		m_configs.insert(std::pair<wxString, BuildConfigPtr>(wxT("Debug"), new BuildConfig(NULL)));
	}

	// Create global settings if it's not been loaded or by default
	if (!m_globalSettings)
	{
//		wxLogMessage(wxT("ProjectSettings : Create global settings because it doesn't exists"));
		m_globalSettings = new BuildConfigCommon(NULL, wxT("GlobalSettings"));
	}
}

ProjectSettings::~ProjectSettings()
{
}

ProjectSettings *ProjectSettings::Clone() const
{
	wxXmlNode *node = ToXml();
	ProjectSettings *cloned = new ProjectSettings(node);
	delete node;
	return cloned;
}

wxXmlNode *ProjectSettings::ToXml() const
{
	wxXmlNode *node = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("Settings"));
	node->AddProperty(wxT("Type"), m_projectType);
	node->AddChild(m_globalSettings->ToXml());
	std::map<wxString, BuildConfigPtr>::const_iterator iter = m_configs.begin();
	for (; iter != m_configs.end(); iter++) {
		node->AddChild(iter->second->ToXml());
	}
	return node;
}

BuildConfigPtr ProjectSettings::GetBuildConfiguration(const wxString &configName, bool merge) const
{
	wxString confName = configName;
	if (confName.IsEmpty()) {
		confName = wxT("Debug");
	}

	std::map<wxString, BuildConfigPtr>::const_iterator iter = m_configs.find(confName);
	if (iter == m_configs.end()) {
		return NULL;
	}
	BuildConfigPtr buildConf = iter->second;
	if (!merge) {
		return buildConf;
	}

	// Need to merge configuration and global settings
	BuildConfigPtr buildConfMerged(buildConf->Clone());
	if (buildConfMerged->GetBuildCmpWithGlobalSettings() == BuildConfig::PREPEND_GLOBAL_SETTINGS) 	{
		buildConfMerged->SetCompileOptions(buildConf->GetCompileOptions() + wxT(";") + m_globalSettings->GetCompileOptions());
		buildConfMerged->SetAssemblerOptions(buildConf->GetAssemblerOptions() + wxT(";") + m_globalSettings->GetAssemblerOptions());
		buildConfMerged->SetPreprocessor(buildConf->GetPreprocessor() + wxT(";") + m_globalSettings->GetPreprocessor());
		buildConfMerged->SetIncludePath(buildConf->GetIncludePath() + wxT(";") + m_globalSettings->GetIncludePath());
	}
	else if (buildConfMerged->GetBuildCmpWithGlobalSettings() == BuildConfig::APPEND_TO_GLOBAL_SETTINGS) {
		buildConfMerged->SetCompileOptions(m_globalSettings->GetCompileOptions() + wxT(";") + buildConf->GetCompileOptions());
		buildConfMerged->SetAssemblerOptions(m_globalSettings->GetAssemblerOptions() + wxT(";") + buildConf->GetAssemblerOptions());
		buildConfMerged->SetPreprocessor(m_globalSettings->GetPreprocessor() + wxT(";") + buildConf->GetPreprocessor());
		buildConfMerged->SetIncludePath(m_globalSettings->GetIncludePath() + wxT(";") + buildConf->GetIncludePath());
	}
	if (buildConfMerged->GetBuildLnkWithGlobalSettings() == BuildConfig::PREPEND_GLOBAL_SETTINGS) 	{
		buildConfMerged->SetLinkOptions(buildConf->GetLinkOptions() + wxT(";") + m_globalSettings->GetLinkOptions());
		buildConfMerged->SetLibraries(buildConf->GetLibraries() + wxT(";") + m_globalSettings->GetLibraries());
		buildConfMerged->SetLibPath(buildConf->GetLibPath() + wxT(";") + m_globalSettings->GetLibPath());
	}
	else if (buildConfMerged->GetBuildLnkWithGlobalSettings() == BuildConfig::APPEND_TO_GLOBAL_SETTINGS) {
		buildConfMerged->SetLinkOptions(m_globalSettings->GetLinkOptions() + wxT(";") + buildConf->GetLinkOptions());
		buildConfMerged->SetLibraries(m_globalSettings->GetLibraries() + wxT(";") + buildConf->GetLibraries());
		buildConfMerged->SetLibPath(m_globalSettings->GetLibPath() + wxT(";") + buildConf->GetLibPath());
	}
	if (buildConfMerged->GetBuildResWithGlobalSettings() == BuildConfig::PREPEND_GLOBAL_SETTINGS) 	{
		buildConfMerged->SetResCmpOptions(buildConf->GetResCompileOptions() + wxT(";") + m_globalSettings->GetResCompileOptions());
		buildConfMerged->SetResCmpIncludePath(buildConf->GetResCmpIncludePath() + wxT(";") + m_globalSettings->GetResCmpIncludePath());
	}
	else if (buildConfMerged->GetBuildResWithGlobalSettings() == BuildConfig::APPEND_TO_GLOBAL_SETTINGS) {
		buildConfMerged->SetResCmpOptions(m_globalSettings->GetResCompileOptions() + wxT(";") + buildConf->GetResCompileOptions());
		buildConfMerged->SetResCmpIncludePath(m_globalSettings->GetResCmpIncludePath() + wxT(";") + buildConf->GetResCmpIncludePath());
	}
	return buildConfMerged;
}

BuildConfigPtr ProjectSettings::GetFirstBuildConfiguration(ProjectSettingsCookie &cookie) const
{
	cookie.iter = m_configs.begin();
	if (cookie.iter != m_configs.end()) {
		BuildConfigPtr conf = cookie.iter->second;
		cookie.iter++;
		return conf;
	}
	return NULL;
}

BuildConfigPtr ProjectSettings::GetNextBuildConfiguration(ProjectSettingsCookie &cookie) const
{
	if (cookie.iter != m_configs.end()) {
		BuildConfigPtr conf = cookie.iter->second;
		cookie.iter++;
		return conf;
	}
	return NULL;
}

void ProjectSettings::SetBuildConfiguration(const BuildConfigPtr bc)
{
	m_configs[bc->GetName()] = bc;
}

void ProjectSettings::RemoveConfiguration(const wxString  &configName)
{
	std::map<wxString, BuildConfigPtr>::iterator iter = m_configs.find(configName);
	if (iter != m_configs.end()) {
		m_configs.erase(iter);
	}
}

wxString ProjectSettings::GetProjectType(const wxString &confName)
{
	// try to return the project type per configuration name. If no property name 'configurationType' exists,
	// return the parent configuration type
	if (confName.IsEmpty() == false) {
		std::map<wxString, BuildConfigPtr>::iterator iter = m_configs.find(confName);
		if (iter != m_configs.end()) {
			BuildConfigPtr conf = iter->second;
			wxString type = conf->GetProjectType();
			if (type.IsEmpty()) {
				type = m_projectType;
			}
			return type;
		}
	}
	return m_projectType;
}
