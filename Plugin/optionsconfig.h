//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : optionsconfig.h
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

#ifndef OPTIONS_CONFIG_H
#define OPTIONS_CONFIG_H

#include "wx/string.h"
#include "wx/xml/xml.h"
#include "wx/colour.h"
#include "wx/font.h"
#include "configuration_object.h"

class OptionsConfig : public ConfObject
{
protected:
	bool           m_displayFoldMargin;
	bool           m_underlineFoldLine;
	wxString       m_foldStyle;
	bool           m_displayBookmarkMargin;
	wxString       m_bookmarkShape;
	wxColour       m_bookmarkBgColour;
	wxColour       m_bookmarkFgColour;
	bool           m_highlightCaretLine;
	bool           m_displayLineNumbers;
	bool           m_showIndentationGuidelines;
	wxColour       m_caretLineColour;
	bool           m_indentUsesTabs;
	int            m_indentWidth;
	int            m_tabWidth;
	int            m_iconsSize;
	wxFontEncoding m_fileFontEncoding;
	int            m_showWhitspaces;
	bool           m_foldCompact;
	bool           m_foldAtElse;
	bool           m_foldPreprocessor;
	int            m_edgeMode;
	int            m_edgeColumn;
	wxColour       m_edgeColour;
	bool           m_highlightMatchedBraces;
	bool           m_autoAddMatchedBraces;
	wxColour       m_foldBgColour;
	bool           m_autoAdjustHScrollBarWidth;
	int            m_caretWidth;
	int            m_caretBlinkPeriod;
	wxString       m_programConsoleCommand;
	wxString       m_eolMode;
	bool           m_hideChangeMarkerMargin;
	bool           m_hideOutpuPaneOnUserClick;
	bool           m_hideOutputPaneNotIfDebug;
	bool           m_showQuickFinder;
	bool           m_TrimLine;
	bool           m_AppendLF;
	bool           m_disableSmartIndent;
	bool           m_disableSemicolonShift;
	int            m_caretLineAlpha;

public:
	OptionsConfig() {}
	OptionsConfig(wxXmlNode *node);
	virtual ~OptionsConfig(void);

	//-------------------------------------
	// Setters/Getters
	//-------------------------------------

	bool GetDisableSemicolonShift() const {
		return m_disableSemicolonShift;
	}
	void SetDisableSemicolonShift(bool disableSemicolonShift) {
		this->m_disableSemicolonShift = disableSemicolonShift;
	}
	void SetDisableSmartIndent(bool disableSmartIndent) {
		this->m_disableSmartIndent = disableSmartIndent;
	}
	bool GetDisableSmartIndent() const {
		return m_disableSmartIndent;
	}
	void SetTrimLine(const bool& trimLine) {
		this->m_TrimLine = trimLine;
	}
	const bool& GetTrimLine() const {
		return m_TrimLine;
	}
	void SetAppendLF(const bool& appendLF) {
		this->m_AppendLF = appendLF;
	}
	const bool& GetAppendLF() const {
		return m_AppendLF;
	}
	void SetShowQuickFinder(const bool& showQuickFinder) {
		this->m_showQuickFinder = showQuickFinder;
	}
	const bool& GetShowQuickFinder() const {
		return m_showQuickFinder;
	}
	void SetHideOutpuPaneOnUserClick(const bool& hideOutpuPaneOnUserClick) {
		this->m_hideOutpuPaneOnUserClick = hideOutpuPaneOnUserClick;
	}
	const bool& GetHideOutpuPaneOnUserClick() const {
		return m_hideOutpuPaneOnUserClick;
	}
	void SetHideOutputPaneNotIfDebug(const bool& HideOutpuPaneNotIfDebug) {
		this->m_hideOutputPaneNotIfDebug = HideOutpuPaneNotIfDebug;
	}
	const bool& GetHideOutputPaneNotIfDebug() const {
		return m_hideOutputPaneNotIfDebug;
	}
	void SetHideChangeMarkerMargin(bool hideChangeMarkerMargin) {
		this->m_hideChangeMarkerMargin = hideChangeMarkerMargin;
	}

	bool GetHideChangeMarkerMargin() const {
		return m_hideChangeMarkerMargin;
	}

	bool GetDisplayFoldMargin() const {
		return m_displayFoldMargin;
	}
	bool GetUnderlineFoldLine() const {
		return m_underlineFoldLine;
	}
	wxString GetFoldStyle() const {
		return m_foldStyle;
	}
	bool GetDisplayBookmarkMargin() const {
		return m_displayBookmarkMargin;
	}
	wxString GetBookmarkShape() const {
		return m_bookmarkShape;
	}
	wxColour GetBookmarkFgColour() const {
		return m_bookmarkFgColour;
	}
	wxColour GetBookmarkBgColour() const {
		return m_bookmarkBgColour;
	}
	bool GetHighlightCaretLine() const {
		return m_highlightCaretLine;
	}
	bool GetDisplayLineNumbers() const {
		return m_displayLineNumbers;
	}
	bool GetShowIndentationGuidelines() const {
		return m_showIndentationGuidelines;
	}
	wxColour GetCaretLineColour() const {
		return m_caretLineColour;
	}


	void SetDisplayFoldMargin(bool b) {
		m_displayFoldMargin = b;
	}
	void SetUnderlineFoldLine(bool b) {
		m_underlineFoldLine = b;
	}
	void SetFoldStyle(wxString s) {
		m_foldStyle = s;
	}
	void SetDisplayBookmarkMargin(bool b) {
		m_displayBookmarkMargin = b;
	}
	void SetBookmarkShape(wxString s) {
		m_bookmarkShape = s;
	}
	void SetBookmarkFgColour(wxColour c) {
		m_bookmarkFgColour = c;
	}
	void SetBookmarkBgColour(wxColour c) {
		m_bookmarkBgColour = c;
	}
	void SetHighlightCaretLine(bool b) {
		m_highlightCaretLine = b;
	}
	void SetDisplayLineNumbers(bool b) {
		m_displayLineNumbers = b;
	}
	void SetShowIndentationGuidelines(bool b) {
		m_showIndentationGuidelines = b;
	}
	void SetCaretLineColour(wxColour c) {
		m_caretLineColour = c;
	}

	void SetIndentUsesTabs(const bool& indentUsesTabs) {
		this->m_indentUsesTabs = indentUsesTabs;
	}
	const bool& GetIndentUsesTabs() const {
		return m_indentUsesTabs;
	}
	void SetIndentWidth(const int& indentWidth) {
		this->m_indentWidth = indentWidth;
	}
	const int& GetIndentWidth() const {
		return m_indentWidth;
	}
	void SetTabWidth(const int& tabWidth) {
		this->m_tabWidth = tabWidth;
	}
	const int& GetTabWidth() const {
		return m_tabWidth;
	}

	void SetIconsSize(const int& iconsSize) {
		this->m_iconsSize = iconsSize;
	}
	const int& GetIconsSize() const {
		return m_iconsSize;
	}

	void SetFileFontEncoding(const wxString& strFileFontEncoding);
	wxFontEncoding GetFileFontEncoding() const {
		return m_fileFontEncoding;
	}

	void SetShowWhitspaces(const int& showWhitspaces) {
		this->m_showWhitspaces = showWhitspaces;
	}
	const int& GetShowWhitspaces() const {
		return m_showWhitspaces;
	}

	void SetFoldAtElse(const bool& foldAtElse) {
		this->m_foldAtElse = foldAtElse;
	}
	void SetFoldCompact(const bool& foldCompact) {
		this->m_foldCompact = foldCompact;
	}
	const bool& GetFoldAtElse() const {
		return m_foldAtElse;
	}
	const bool& GetFoldCompact() const {
		return m_foldCompact;
	}
	void SetFoldPreprocessor(const bool& foldPreprocessor) {
		this->m_foldPreprocessor = foldPreprocessor;
	}
	const bool& GetFoldPreprocessor() const {
		return m_foldPreprocessor;
	}

	void SetEdgeColour(const wxColour& edgeColour) {
		this->m_edgeColour = edgeColour;
	}
	void SetEdgeColumn(int edgeColumn) {
		this->m_edgeColumn = edgeColumn;
	}
	void SetEdgeMode(int edgeMode) {
		this->m_edgeMode = edgeMode;
	}

	const wxColour& GetEdgeColour() const {
		return m_edgeColour;
	}
	int GetEdgeColumn() const {
		return m_edgeColumn;
	}
	int GetEdgeMode() const {
		return m_edgeMode;
	}

	void SetHighlightMatchedBraces(const bool& highlightMatchedBraces) {
		this->m_highlightMatchedBraces = highlightMatchedBraces;
	}
	const bool& GetHighlightMatchedBraces() const {
		return m_highlightMatchedBraces;
	}
	void SetAutoAddMatchedBraces(const bool& autoAddMatchedBraces) {
		this->m_autoAddMatchedBraces = autoAddMatchedBraces;
	}
	const bool& GetAutoAddMatchedBraces() const {
		return m_autoAddMatchedBraces;
	}

	void SetFoldBgColour(const wxColour& foldBgColour) {
		this->m_foldBgColour = foldBgColour;
	}
	const wxColour& GetFoldBgColour() const {
		return m_foldBgColour;
	}
	void SetAutoAdjustHScrollBarWidth(const bool& autoAdjustHScrollBarWidth) {
		this->m_autoAdjustHScrollBarWidth = autoAdjustHScrollBarWidth;
	}
	const bool& GetAutoAdjustHScrollBarWidth() const {
		return m_autoAdjustHScrollBarWidth;
	}
	void SetCaretBlinkPeriod(const int& caretBlinkPeriod) {
		this->m_caretBlinkPeriod = caretBlinkPeriod;
	}
	void SetCaretWidth(const int& caretWidth) {
		this->m_caretWidth = caretWidth;
	}
	const int& GetCaretBlinkPeriod() const {
		return m_caretBlinkPeriod;
	}
	const int& GetCaretWidth() const {
		return m_caretWidth;
	}

	void SetProgramConsoleCommand(const wxString& programConsoleCommand) {
		this->m_programConsoleCommand = programConsoleCommand;
	}

	const wxString& GetProgramConsoleCommand() const {
		return m_programConsoleCommand;
	}

	void SetEolMode(const wxString& eolMode) {
		this->m_eolMode = eolMode;
	}
	const wxString& GetEolMode() const {
		return m_eolMode;
	}
	void SetCaretLineAlpha(int caretLineAlpha) {
		this->m_caretLineAlpha = caretLineAlpha;
	}
	int GetCaretLineAlpha() const {
		return m_caretLineAlpha;
	}
	/**
	 * Return an XML representation of this object
	 * \return XML node
	 */
	wxXmlNode *ToXml() const;
};

typedef SmartPtr<OptionsConfig> OptionsConfigPtr;


#endif // OPTIONS_CONFIG_H
