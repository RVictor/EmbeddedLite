//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : cc_box.h
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
#ifndef __cc_box__
#define __cc_box__

/**
@file
Subclass of CCBoxBase, which is generated by wxFormBuilder.
*/

#include "cc_boxbase.h"
#include "entry.h"
#include <vector>
#include <map>

class LEditor;

/** Implementing CCBoxBase */
class CCBox : public CCBoxBase
{
	int m_selectedItem;
	std::vector<TagEntryPtr> m_tags;
	bool m_showFullDecl;
	int m_height;
	bool m_autoHide;
	bool m_insertSingleChoice;
	std::map<wxString, int> m_userImages;
	wxEvtHandler *m_owner;
	
protected:
	// Handlers for CCBoxBase events.
	void OnItemActivated( wxListEvent& event );
	void OnItemDeSelected( wxListEvent& event );
	void OnItemSelected( wxListEvent& event );

protected:
	// helper methods
	int GetImageId(const TagEntry &entry);
	void SelectItem(long item);
	void Show(const wxString &word);
	void DoInsertSelection(const wxString &word, bool triggerTip = true);

public:

	/** Constructor */
	CCBox(LEditor* parent, bool autoHide = true, bool autoInsertSingleChoice = true);

	void Show(const std::vector<TagEntryPtr> &tags, const wxString &word, bool showFullDecl, wxEvtHandler *owner = NULL);
	void InsertSelection();

	void Adjust();

	// can this window have focus? (no)
	bool AcceptsFocus() const {
		return false;
	}

	void SelectWord(const wxString &word);
	void Next();
	void Previous();
	
	//Setters
	void SetAutoHide(const bool& autoHide) {
		this->m_autoHide = autoHide;
	}
	void SetInsertSingleChoice(const bool& insertSingleChoice) {
		this->m_insertSingleChoice = insertSingleChoice;
	}
	//Getters
	const bool& GetAutoHide() const {
		return m_autoHide;
	}
	const bool& GetInsertSingleChoice() const {
		return m_insertSingleChoice;
	}
	
	/**
	 * @brief register new user image fot TagEntry kind
	 * @param kind the kind string that will be associated with the bitmap (TagEntry::GetKind())
	 * @param bmp 16x16 bitmap
	 */
	void RegisterImageForKind(const wxString &kind, const wxBitmap &bmp);
	void NextPage();
	void PreviousPage();
};

#endif // __cc_box__
