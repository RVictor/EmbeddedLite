//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : buildtabsettingsdata.h
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
#ifndef __buildtabsettingsdata__
#define __buildtabsettingsdata__

#include "serialized_object.h"

class BuildTabSettingsData : public SerializedObject
{
	bool     m_skipWarnings;
	wxString m_warnColour;
	wxString m_errorColour;
	wxString m_warnColourBg;
	wxString m_errorColourBg;
	bool     m_boldErrFont;
	bool     m_boldWarnFont;
	bool     m_showBuildPane;
	bool     m_autoHide;
	int      m_errorWarningStyle;

private:
	BuildTabSettingsData(const BuildTabSettingsData& rhs);
	BuildTabSettingsData& operator=(const BuildTabSettingsData& rhs);

public:
	enum ErrorsWarningStyle {
		EWS_NoMarkers   = 0x00000000,
		EWS_Annotations = 0x00000001,
		EWS_Bookmarks   = 0x00000002
	};

	enum ShowBuildPane {
		ShowOnStart,
		ShowOnEnd,
		DontShow,
	};

	BuildTabSettingsData();
	~BuildTabSettingsData();

	void Serialize(Archive &arch);
	void DeSerialize(Archive &arch);


	//Setters
	void SetBoldErrFont(const bool& boldErrFont) {
		this->m_boldErrFont = boldErrFont;
	}
	void SetBoldWarnFont(const bool& boldWarnFont) {
		this->m_boldWarnFont = boldWarnFont;
	}
	void SetErrorColour(const wxString& errorColour) {
		this->m_errorColour = errorColour;
	}
	void SetErrorColourBg(const wxString& errorColourBg) {
		this->m_errorColourBg = errorColourBg;
	}
	void SetShowBuildPane(int showBuildPane) {
		this->m_showBuildPane = showBuildPane;
	}
	void SetSkipWarnings(const bool& skipWarnings) {
		this->m_skipWarnings = skipWarnings;
	}
	void SetWarnColour(const wxString& warnColour) {
		this->m_warnColour = warnColour;
	}
	void SetWarnColourBg(const wxString& warnColourBg) {
		this->m_warnColourBg = warnColourBg;
	}
	//Getters
	const bool& GetBoldErrFont() const {
		return m_boldErrFont;
	}
	const bool& GetBoldWarnFont() const {
		return m_boldWarnFont;
	}
	const wxString& GetErrorColour() const {
		return m_errorColour;
	}
	const wxString& GetErrorColourBg() const {
		return m_errorColourBg;
	}
	int GetShowBuildPane() const {
		return m_showBuildPane;
	}
	const bool& GetSkipWarnings() const {
		return m_skipWarnings;
	}
	const wxString& GetWarnColour() const {
		return m_warnColour;
	}
	const wxString& GetWarnColourBg() const {
		return m_warnColourBg;
	}

	//Setters
	void SetAutoHide(const bool& autoHide) {
		this->m_autoHide = autoHide;
	}
	const bool& GetAutoHide() const {
		return m_autoHide;
	}

	void SetErrorWarningStyle(const int& errorWarningStyle) {
		this->m_errorWarningStyle = errorWarningStyle;
	}
	const int& GetErrorWarningStyle() const {
		return m_errorWarningStyle;
	}
};
#endif // __buildtabsettingsdata__
