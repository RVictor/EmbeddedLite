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
#ifndef __localoptions__
#define __localoptions__

#include <wx/filename.h>
#include "singleton.h"
#include "optionsconfig.h"


// Denotes whether we're dealing with preferences at a global, workspace, project or (maybe one day) file level
enum prefsLevel { pLevel_global, pLevel_workspace, pLevel_project, pLevel_file, pLevel_dunno };

class LocalOptionsConfig;
typedef SmartPtr<LocalOptionsConfig> LocalOptionsConfigPtr;

template <typename T>
class validVar
{
	bool valid;
	T datum;
public:
	validVar() : valid(false) {}
	void Set(const T info) {
		datum = info;
		valid = true;
	}
	void Reset() {
		valid = false;
	}
	T GetDatum() const {
		return datum;
	}
	bool isValid() const {
		return valid;
	}
};

class LocalOptionsConfig
{
	validVar<bool>     m_localdisplayFoldMargin;
	validVar<bool>     m_localdisplayBookmarkMargin;
	validVar<bool>     m_localhighlightCaretLine;
	validVar<bool>     m_localTrimLine;
	validVar<bool>     m_localAppendLF;
	validVar<bool>     m_localdisplayLineNumbers;
	validVar<bool>     m_localshowIndentationGuidelines;
	validVar<bool>     m_localindentUsesTabs;
	validVar<int>      m_localindentWidth;
	validVar<int>      m_localtabWidth;
	validVar<wxFontEncoding> m_localfileFontEncoding;
	validVar<int>      m_localshowWhitspaces;
	validVar<wxString> m_localeolMode;
	validVar<bool>     m_localhideChangeMarkerMargin;

public:
	LocalOptionsConfig(); // Used for setting local values
	LocalOptionsConfig(OptionsConfigPtr opts, wxXmlNode *node); // Used for merging local values into the already-found global ones
	LocalOptionsConfig(LocalOptionsConfigPtr opts, wxXmlNode *node); // Used for storing local values in a previously-empty instance
	virtual ~LocalOptionsConfig(void) {}

	bool HideChangeMarkerMarginIsValid() const {
		return m_localhideChangeMarkerMargin.isValid();
	}
	bool DisplayFoldMarginIsValid() const {
		return m_localdisplayFoldMargin.isValid();
	}
	bool DisplayBookmarkMarginIsValid() const {
		return m_localdisplayBookmarkMargin.isValid();
	}
	bool HighlightCaretLineIsValid() const {
		return m_localhighlightCaretLine.isValid();
	}
	bool TrimLineIsValid() const {
		return m_localTrimLine.isValid();
	}
	bool AppendLFIsValid() const {
		return m_localAppendLF.isValid();
	}
	bool DisplayLineNumbersIsValid() const {
		return m_localdisplayLineNumbers.isValid();
	}
	bool ShowIndentationGuidelinesIsValid() const {
		return m_localshowIndentationGuidelines.isValid();
	}
	bool IndentUsesTabsIsValid() const {
		return m_localindentUsesTabs.isValid();
	}
	bool IndentWidthIsValid() const {
		return m_localindentWidth.isValid();
	}
	bool TabWidthIsValid() const {
		return m_localtabWidth.isValid();
	}
	bool FileFontEncodingIsValid() const {
		return m_localfileFontEncoding.isValid();
	}
	bool ShowWhitespacesIsValid() const {
		return m_localshowWhitspaces.isValid();
	}
	bool EolModeIsValid() const {
		return m_localeolMode.isValid();
	}

	//-------------------------------------
	// Setters/Getters
	//-------------------------------------

	bool GetHideChangeMarkerMargin() const {
		if (m_localhideChangeMarkerMargin.isValid()) {
			return m_localhideChangeMarkerMargin.GetDatum();
		}
		return false;// It's invalid anyway, so false will do as well as anything
	}
	bool GetDisplayFoldMargin() const {
		if (m_localdisplayFoldMargin.isValid()) {
			return m_localdisplayFoldMargin.GetDatum();
		}
		return false;
	}
	bool GetDisplayBookmarkMargin() const {
		if (m_localdisplayBookmarkMargin.isValid()) {
			return m_localdisplayBookmarkMargin.GetDatum();
		}
		return false;
	}
	bool GetHighlightCaretLine() const {
		if (m_localhighlightCaretLine.isValid()) {
			return m_localhighlightCaretLine.GetDatum();
		}
		return false;
	}
	bool GetTrimLine() const {
		if (m_localTrimLine.isValid()) {
			return m_localTrimLine.GetDatum();
		}
		return false;
	}
	bool GetAppendLF() const {
		if (m_localAppendLF.isValid()) {
			return m_localAppendLF.GetDatum();
		}
		return false;
	}
	bool GetDisplayLineNumbers() const {
		if (m_localdisplayLineNumbers.isValid()) {
			return m_localdisplayLineNumbers.GetDatum();
		}
		return false;
	}
	bool GetShowIndentationGuidelines() const {
		if (m_localshowIndentationGuidelines.isValid()) {
			return m_localshowIndentationGuidelines.GetDatum();
		}
		return false;
	}

	void SetHideChangeMarkerMargin(bool hideChangeMarkerMargin) {
		m_localhideChangeMarkerMargin.Set(hideChangeMarkerMargin);
	}
	void SetDisplayFoldMargin(bool b) {
		m_localdisplayFoldMargin.Set(b);
	}
	void SetDisplayBookmarkMargin(bool b) {
		m_localdisplayBookmarkMargin.Set(b);
	}
	void SetHighlightCaretLine(bool b) {
		m_localhighlightCaretLine.Set(b);
	}
	void SetTrimLine(bool b) {
		m_localTrimLine.Set(b);
	}
	void SetAppendLF(bool b) {
		m_localAppendLF.Set(b);
	}
	void SetDisplayLineNumbers(bool b) {
		m_localdisplayLineNumbers.Set(b);
	}
	void SetShowIndentationGuidelines(bool b) {
		m_localshowIndentationGuidelines.Set(b);
	}
	void SetIndentUsesTabs(const bool& indentUsesTabs) {
		m_localindentUsesTabs.Set(indentUsesTabs);
	}
	bool GetIndentUsesTabs() const {
		if (m_localindentUsesTabs.isValid()) {
			return m_localindentUsesTabs.GetDatum();
		}
		return false;
	}
	void SetIndentWidth(const int& indentWidth) {
		m_localindentWidth.Set(indentWidth);
	}
	int GetIndentWidth() const {
		if (m_localindentWidth.isValid()) {
			return m_localindentWidth.GetDatum();
		}
		return wxNOT_FOUND;
	}
	void SetTabWidth(const int& tabWidth) {
		m_localtabWidth.Set(tabWidth);
	}
	int GetTabWidth() const {
		if (m_localtabWidth.isValid()) {
			return m_localtabWidth.GetDatum();
		}
		return wxNOT_FOUND;
	}

	wxFontEncoding GetFileFontEncoding() const {
		if (m_localfileFontEncoding.isValid()) {
			return m_localfileFontEncoding.GetDatum();
		}
		return (wxFontEncoding)(wxFONTENCODING_MAX + 1);
	}
	void SetFileFontEncoding(const wxString& strFileFontEncoding);

	void SetShowWhitespaces(const int& showWhitespaces) {
		m_localshowWhitspaces.Set(showWhitespaces);
	}
	int GetShowWhitespaces() const {
		if (m_localshowWhitspaces.isValid()) {
			return m_localshowWhitspaces.GetDatum();
		}
		return wxNOT_FOUND;
	}

	void SetEolMode(const wxString& eolMode) {
		m_localeolMode.Set(eolMode);
	}
	wxString GetEolMode() const {
		if (m_localeolMode.isValid()) {
			return m_localeolMode.GetDatum();
		}
		return wxT("");
	}

	/**
	 * Return an XML representation of this object
	 * \return XML node
	 */
	wxXmlNode *ToXml(wxXmlNode* parent = NULL, const wxString& nodename = wxT("Options")) const;
};

class LocalWorkspace
{
private:
	friend class Singleton<LocalWorkspace>;
	wxXmlDocument m_doc;
	wxFileName m_fileName;

	/// Constructor
	LocalWorkspace() {}

	/// Destructor
	virtual ~LocalWorkspace() {}

protected:
	/*void SaveWorkspaceOptions(LocalOptionsConfigPtr opts);
	void SaveProjectOptions(LocalOptionsConfigPtr opts, const wxString& projectname);*/
	bool SanityCheck();
	bool Create();
	bool SaveXmlFile();



public:
	/**
	* @brief Get any local editor preferences, merging the values into the global options
	* \param options the global options
	* \param projectname the name of the currently active project
	*/
	void GetOptions(OptionsConfigPtr options, const wxString& projectname);
	/**
	* @brief Sets any local editor preferences for the current workspace
	* \param opts the local options to save
	*/
	bool SetWorkspaceOptions(LocalOptionsConfigPtr opts);
	/**
	* @brief Sets any local editor preferences for the named project
	* \param opts the local options to save
	* \param projectname the name of the project
	*/
	bool SetProjectOptions(LocalOptionsConfigPtr opts, const wxString& projectname);
	/**
	* @brief Returns the node where any current local workspace options are stored
	*/
	wxXmlNode* GetLocalWorkspaceOptionsNode() const;
	/**
	* @brief Returns the node where any current local project options are stored
	* \param projectname the name of the project
	*/
	wxXmlNode* GetLocalProjectOptionsNode(const wxString& projectname) const;

	/**
	 * @brief return the workspace C++ parser specific include + exclude paths
	 * @param inclduePaths [output]
	 * @param excludePaths [output]
	 */
	void GetParserPaths(wxArrayString &inclduePaths, wxArrayString &excludePaths);
	void SetParserPaths(const wxArrayString &inclduePaths, const wxArrayString &excludePaths);

	/**
	 * @brief set and get the active environment variables set name
	 */
	void     SetActiveEnvironmentSet(const wxString &setName);
	wxString GetActiveEnvironmentSet();
};


typedef Singleton<LocalWorkspace> LocalWorkspaceST;

#endif // __localoptions__
