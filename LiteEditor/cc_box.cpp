//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : cc_box.cpp
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
#include "cc_box.h"
#include "cl_editor_tip_window.h"
#include "cl_editor.h"
#include "globals.h"
#include <wx/imaglist.h>
#include <wx/xrc/xmlres.h>
#include "entry.h"
#include "plugin.h"

#define BOX_HEIGHT 200
#define BOX_WIDTH  300

CCBox::CCBox(LEditor* parent, bool autoHide, bool autoInsertSingleChoice)
		:
		CCBoxBase(parent, wxID_ANY, wxDefaultPosition, wxSize(0, 0))
		, m_showFullDecl(false)
		, m_height(BOX_HEIGHT)
		, m_autoHide(autoHide)
		, m_insertSingleChoice(autoInsertSingleChoice)
		, m_owner(NULL)
{
	Hide();

	// load all the CC images
	wxImageList *il = new wxImageList(16, 16, true);

	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("class")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("struct")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("namespace")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("member_public")));

	wxBitmap m_tpyedefBmp = wxXmlResource::Get()->LoadBitmap(wxT("typedef"));
	m_tpyedefBmp.SetMask(new wxMask(m_tpyedefBmp, wxColor(0, 128, 128)));
	il->Add(m_tpyedefBmp);

	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("member_private")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("member_public")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("member_protected")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("func_private")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("func_public")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("func_protected")));
	wxBitmap m_macroBmp = wxXmlResource::Get()->LoadBitmap(wxT("typedef"));
	m_macroBmp.SetMask(new wxMask(m_macroBmp, wxColor(0, 128, 128)));
	il->Add(m_macroBmp);

	wxBitmap m_enumBmp = wxXmlResource::Get()->LoadBitmap(wxT("enum"));
	m_enumBmp.SetMask(new wxMask(m_enumBmp, wxColor(0, 128, 128)));
	il->Add(m_enumBmp);

	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("enumerator")));

	//Initialise the file bitmaps
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("page_white_cplusplus")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("page_white_h")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("page_white_text")));
	il->Add(wxXmlResource::Get()->LoadBitmap(wxT("cpp_keyword")));

	// assign the image list and let the control take owner ship (i.e. delete it)
	m_listCtrl->AssignImageList(il, wxIMAGE_LIST_SMALL);
	m_listCtrl->InsertColumn(0, wxT("Name"));
	m_listCtrl->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOBK));
	m_listCtrl->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_INFOTEXT));

	m_listCtrl->SetFocus();
	// return the focus to scintilla
	parent->SetActive();
}

void CCBox::OnItemActivated( wxListEvent& event )
{
	m_selectedItem = event.m_itemIndex;
	InsertSelection();
	Hide();

	LEditor *editor = (LEditor*)GetParent();
	if (editor) {
		editor->SetActive();
	}
}

void CCBox::OnItemDeSelected( wxListEvent& event )
{
#ifdef __WXMAC__
	m_listCtrl->Select(event.m_itemIndex, false);
#endif
	m_selectedItem = wxNOT_FOUND;
}

void CCBox::OnItemSelected( wxListEvent& event )
{
	m_selectedItem = event.m_itemIndex;
}

void CCBox::Show(const std::vector<TagEntryPtr> &tags, const wxString &word, bool showFullDecl, wxEvtHandler *owner)
{
	if (tags.empty()) {
		return;
	}

	//m_height = BOX_HEIGHT;
	m_tags = tags;
	m_showFullDecl = showFullDecl;
	m_owner = owner;
	Show(word);
}

void CCBox::Adjust()
{
	LEditor *parent = (LEditor*)GetParent();

	int point = parent->GetCurrentPos();
	wxPoint pt = parent->PointFromPosition(point);

	// add the line height
	int hh = parent->GetCurrLineHeight();
	pt.y += hh;

	wxSize size = parent->GetClientSize();
	int diff = size.y - pt.y;
	m_height = BOX_HEIGHT;
	if (diff < BOX_HEIGHT) {
		pt.y -= BOX_HEIGHT;
		pt.y -= hh;

		if (pt.y < 0) {
			// the completion box is out of screen, resotre original size
			pt.y += BOX_HEIGHT;
			pt.y += hh;
			m_height = diff;
		}
	}

	// adjust the X axis
	if (size.x - pt.x < BOX_WIDTH) {
		// the box is too wide to fit the screen
		if (size.x > BOX_WIDTH) {
			// the screen can contain the completion box
			pt.x = size.x - BOX_WIDTH;
		} else {
			// this will provive the maximum visible area
			pt.x = 0;
		}
	}
	Move(pt);
}

void CCBox::SelectWord(const wxString& word)
{
	long item = m_listCtrl->FindMatch(word);
	if (item != wxNOT_FOUND) {
		// first unselect the current item
		if (m_selectedItem != wxNOT_FOUND && m_selectedItem != item) {
			m_listCtrl->Select(m_selectedItem, false);
		}

		m_selectedItem = item;
		SelectItem(m_selectedItem);
	} else {
		if (GetAutoHide()) {
			Hide();
		}
	}
}

void CCBox::Next()
{
	if (m_selectedItem != wxNOT_FOUND) {
		if (m_selectedItem + 1 < m_listCtrl->GetItemCount()) {
#ifdef __WXMAC__
			// unselect current item
			m_listCtrl->Select(m_selectedItem, false);
#endif
			m_selectedItem++;
			// select next item
			SelectItem(m_selectedItem);
		}
	}
}

void CCBox::Previous()
{
	if (m_selectedItem != wxNOT_FOUND) {
		if (m_selectedItem - 1 >= 0) {
#ifdef __WXMAC__
			// unselect current item
			m_listCtrl->Select(m_selectedItem, false);
#endif
			m_selectedItem--;

			// select previous item
			SelectItem(m_selectedItem);
		}
	}
}

void CCBox::SelectItem(long item)
{
	m_listCtrl->Select(item);
	m_listCtrl->EnsureVisible(item);
}

void CCBox::Show(const wxString& word)
{
	wxString lastName;
	size_t i(0);
	std::vector<CCItemInfo> _tags;

	CCItemInfo item;
	m_listCtrl->DeleteAllItems();

	if (m_tags.empty() == false) {
		for (; i<m_tags.size(); i++) {
			TagEntryPtr tag = m_tags.at(i);
			if (lastName != m_tags.at(i)->GetName()) {

				item.displayName =  tag->GetName();
				item.imgId = GetImageId(*m_tags.at(i));
				_tags.push_back(item);

				lastName = tag->GetName();
			}

			if (m_showFullDecl) {
				//collect only declarations
				if (m_tags.at(i)->GetKind() == wxT("prototype")) {
					item.displayName =  tag->GetName()+tag->GetSignature();
					item.imgId = GetImageId(*m_tags.at(i));
					_tags.push_back(item);

				}
			}
		}
	}

	if (_tags.size() == 1 && m_insertSingleChoice) {
		m_selectedItem = 0;
		DoInsertSelection(_tags.at(0).displayName, false);

		// return without calling to wxWindow::Show()
		// also, make sure we are hidden
		if ( IsShown() ) {
			Hide();
		}
		return;
	}

	m_listCtrl->SetColumnWidth(0, BOX_WIDTH);
	m_listCtrl->SetItems(_tags);
	m_listCtrl->SetItemCount(_tags.size());

	m_selectedItem = 0;

	m_selectedItem = m_listCtrl->FindMatch(word);
	if ( m_selectedItem == wxNOT_FOUND && GetAutoHide() ) {
		// return without calling wxWindow::Show
		return;
	}

	if (m_selectedItem == wxNOT_FOUND) {
		m_selectedItem = 0;
	}

	SetSize(BOX_WIDTH, m_height);
	GetSizer()->Layout();
	wxWindow::Show();

	SelectItem(m_selectedItem);
}

void CCBox::DoInsertSelection(const wxString& word, bool triggerTip)
{
	if (m_owner) {

		// simply send an event and dismiss the dialog
		wxCommandEvent e(wxEVT_CCBOX_SELECTION_MADE);
		e.SetClientData( (void*)&word );
		m_owner->ProcessEvent(e);

	} else {
		LEditor *editor = (LEditor*)GetParent();
		int insertPos = editor->WordStartPosition(editor->GetCurrentPos(), true);

		editor->SetSelection(insertPos, editor->GetCurrentPos());
		editor->ReplaceSelection(word);

		// incase we are adding a function, add '()' at the end of the function name and place the caret in the middle
		int img_id = m_listCtrl->OnGetItemImage(m_selectedItem);
		if (img_id >= 8 && img_id <= 10) {

			// if full declaration was selected, dont do anything,
			// otherwise, append '()' to the inserted string, place the caret
			// in the middle, and trigger the function tooltip

			if (word.Find(wxT("(")) == wxNOT_FOUND && triggerTip) {
				// image id in range of 8-10 is function
				editor->InsertText(editor->GetCurrentPos(), wxT("()"));
				editor->CharRight();
				int pos = editor->GetCurrentPos();
				editor->SetSelectionStart(pos);
				editor->SetSelectionEnd(pos);
				editor->SetIndicatorCurrent(MATCH_INDICATOR);
				editor->IndicatorFillRange(pos, 1);
				// trigger function tip
				editor->CodeComplete();

				wxString tipContent = editor->GetContext()->CallTipContent();
				int where = tipContent.Find(wxT(" : "));
				if (where != wxNOT_FOUND) {
					tipContent = tipContent.Mid(where + 3);
				}

				if (tipContent.Trim().Trim(false) == wxT("()")) {
					// dont place the caret in the middle of the braces,
					// and it is OK to cancel the function calltip
					int new_pos = editor->GetCurrentPos() + 1;
					editor->SetCurrentPos(new_pos);
					editor->SetSelectionStart(new_pos);
					editor->SetSelectionEnd(new_pos);
					
					// remove the current tip that we just activated.
					// if this was the last tip, it will also make it go away
					editor->GetFunctionTip()->Remove();
				}
			}
		}
	}
}

void CCBox::InsertSelection()
{
	if (m_selectedItem == wxNOT_FOUND) {
		return;
	}

	// get the selected word
	wxString word = GetColumnText(m_listCtrl, m_selectedItem, 0);
	DoInsertSelection(word);
}

int CCBox::GetImageId(const TagEntry &entry)
{
	if (entry.GetKind() == wxT("class"))
		return 0;

	if (entry.GetKind() == wxT("struct"))
		return 1;

	if (entry.GetKind() == wxT("namespace"))
		return 2;

	if (entry.GetKind() == wxT("variable"))
		return 3;

	if (entry.GetKind() == wxT("typedef"))
		return 4;

	if (entry.GetKind() == wxT("member") && entry.GetAccess().Contains(wxT("private")))
		return 5;

	if (entry.GetKind() == wxT("member") && entry.GetAccess().Contains(wxT("public")))
		return 6;

	if (entry.GetKind() == wxT("member") && entry.GetAccess().Contains(wxT("protected")))
		return 7;

	//member with no access? (maybe part of namespace??)
	if (entry.GetKind() == wxT("member"))
		return 6;

	if ((entry.GetKind() == wxT("function") || entry.GetKind() == wxT("prototype")) && entry.GetAccess().Contains(wxT("private")))
		return 8;

	if ((entry.GetKind() == wxT("function") || entry.GetKind() == wxT("prototype")) && (entry.GetAccess().Contains(wxT("public")) || entry.GetAccess().IsEmpty()))
		return 9;

	if ((entry.GetKind() == wxT("function") || entry.GetKind() == wxT("prototype")) && entry.GetAccess().Contains(wxT("protected")))
		return 10;

	if (entry.GetKind() == wxT("macro"))
		return 11;

	if (entry.GetKind() == wxT("enum"))
		return 12;

	if (entry.GetKind() == wxT("enumerator"))
		return 13;

	if (entry.GetKind() == wxT("cpp_keyword"))
		return 17;

	// try the user defined images
	std::map<wxString, int>::iterator iter = m_userImages.find(entry.GetKind());
	if (iter != m_userImages.end()) {
		return iter->second;
	}
	return wxNOT_FOUND;
}

void CCBox::RegisterImageForKind(const wxString& kind, const wxBitmap& bmp)
{
	wxImageList *il = m_listCtrl->GetImageList(wxIMAGE_LIST_SMALL);
	if (il && bmp.IsOk()) {
		std::map<wxString, int>::iterator iter = m_userImages.find(kind);

		if (iter == m_userImages.end()) {
			int id = il->Add(bmp);
			m_userImages[kind] = id;
		} else {
			// an entry for this kind already exist, replace the current image with new one
			il->Replace(iter->second, bmp);
		}
	}
}

void CCBox::NextPage()
{
	if (m_selectedItem != wxNOT_FOUND) {
		if (m_selectedItem + 1 < m_listCtrl->GetItemCount()) {
#ifdef __WXMAC__
// unselect current item
			m_listCtrl->Select(m_selectedItem, false);
#endif
			m_selectedItem += 10;
// select next item
			SelectItem(m_selectedItem);
		} else {
#ifdef __WXMAC__
// unselect current item
			m_listCtrl->Select(m_selectedItem, false);
#endif
			m_selectedItem = m_listCtrl->GetItemCount() - 1;
// select next item
			SelectItem(m_selectedItem);
		}
	}
}

void CCBox::PreviousPage()
{
	if (m_selectedItem != wxNOT_FOUND) {
		if (m_selectedItem - 10 >= 0) {
#ifdef __WXMAC__
// unselect current item
			m_listCtrl->Select(m_selectedItem, false);
#endif
			m_selectedItem -= 10;

// select previous item
			SelectItem(m_selectedItem);
		} else {
#ifdef __WXMAC__
// unselect current item
			m_listCtrl->Select(m_selectedItem, false);
#endif
			m_selectedItem = 0;

// select previous item
			SelectItem(m_selectedItem);
		}
	}
}
