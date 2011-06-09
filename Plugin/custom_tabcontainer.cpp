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
#include "custom_tabcontainer.h"
#include "notebookcustomdlg.h"
#include <wx/app.h>
#include <wx/xrc/xmlres.h>
#include "drawingutils.h"
#include "wx/settings.h"
#include "wx/sizer.h"
#include "custom_tab.h"
#include "custom_notebook.h"
#include "wx/dcbuffer.h"
#include "wx/menu.h"
#include "wx/log.h"
#include <wx/wupdlock.h>

#include <wx/arrimpl.cpp> // this is a magic incantation which must be done!

#define PNAEL_BG_COLOUR DrawingUtils::GetPanelBgColour()

BEGIN_EVENT_TABLE(wxTabContainer, wxPanel)
	EVT_PAINT(wxTabContainer::OnPaint)
	EVT_ERASE_BACKGROUND(wxTabContainer::OnEraseBg)
	EVT_SIZE(wxTabContainer::OnSizeEvent)
	EVT_LEFT_DCLICK(wxTabContainer::OnDoubleClick)

END_EVENT_TABLE()

//-------------------------------------------------------
// Helper methods
//-------------------------------------------------------

void wxTabContainer::DoDrawBackground(wxDC &dc, bool gradient, int orientation, const wxRect &rr)
{

	// set the gradient colours, by default we use the same colours
	wxColour col1 = PNAEL_BG_COLOUR;
	wxColour col2 = PNAEL_BG_COLOUR;

	if (gradient) {
		col1 = PNAEL_BG_COLOUR;
		col2 = DrawingUtils::LightColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW), 3.0);

		//paint gradient background
		switch (orientation) {
		case wxLEFT:
			DrawingUtils::PaintStraightGradientBox(dc, rr, col1, col2, false);
			break;
		case wxRIGHT:
			DrawingUtils::PaintStraightGradientBox(dc, rr, col2, col1, false);
			break;
		case wxBOTTOM:
			DrawingUtils::PaintStraightGradientBox(dc, rr, col2, col1, true);
			break;
		case wxTOP:
		default:
			DrawingUtils::PaintStraightGradientBox(dc, rr, col1, col2, true);
			break;
		}
	} else {
		dc.SetPen(col1);
		dc.SetBrush(col1);
		dc.DrawRectangle(rr);
	}
}

void wxTabContainer::DoDrawMargin(wxDC &dc, int orientation, const wxRect &rr)
{
	wxPen _3dFace(PNAEL_BG_COLOUR);
	wxPen borderPen(DrawingUtils::LightColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW), DrawingUtils::GetDdkShadowLightFactor2()));

	for (int i=0; i<3; i++) {

		dc.SetPen(_3dFace);
		switch (orientation) {

		case wxRIGHT:
			dc.DrawLine(rr.x+i, rr.y, rr.x+i, rr.y+rr.height);
			break;

		case wxTOP:
			dc.DrawLine(rr.x, rr.height-i-1, rr.x+rr.width, rr.height-i-1);
			break;

		case wxLEFT:
			dc.DrawLine(rr.x+rr.width-i-1, rr.y, rr.x+rr.width-i-2, rr.y+rr.height);
			break;

		default:
			dc.DrawLine(rr.x, rr.y+i, rr.x + rr.width, rr.y+i);
			break;
		}
	}

	dc.SetPen(borderPen);
	switch (orientation) {
	case wxRIGHT:
		dc.DrawLine(rr.x+3, rr.y, rr.x+3, rr.y+rr.height);
		break;

	case wxTOP:
		dc.DrawLine(rr.x, rr.height-4, rr.x+rr.width, rr.height-4);
		break;

	case wxLEFT:
		dc.DrawLine(rr.x+rr.width-4, rr.y, rr.x+rr.width-4, rr.y+rr.height);
		break;

	default:
		dc.DrawLine(rr.x, rr.y+3, rr.x + rr.width, rr.y+3);
		break;
	}
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
wxTabContainer::wxTabContainer(wxWindow *win, wxWindowID id, int orientation, long style)
		: wxPanel         (win, id)
		, m_orientation   (orientation)
		, m_draggedTab    (NULL)
		, m_rightClickMenu(NULL)
		, m_bmpHeight     (14)
		, m_fixedTabWidth (120)
{
	if ( style & wxVB_NO_TABS ) {
		Hide();
	}
	Initialize();
	Connect(wxEVT_CMD_ENSURE_SEL_TAB_VISIBILE, wxCommandEventHandler(wxTabContainer::OnEnsureVisible), NULL, this);
}

wxTabContainer::~wxTabContainer()
{
	if (m_rightClickMenu) {
		delete m_rightClickMenu;
		m_rightClickMenu = NULL;
	}
}

void wxTabContainer::Initialize()
{
	wxOrientation sizerOri( wxHORIZONTAL );
	if (m_orientation == wxLEFT || m_orientation == wxRIGHT) {
		sizerOri = wxVERTICAL;
	}

	wxBoxSizer *sz = new wxBoxSizer(sizerOri);
	SetSizer(sz);

	m_tabsSizer = new wxBoxSizer(sizerOri);

	Notebook *book = (Notebook*) GetParent();
	if (!(book->GetBookStyle() & wxVB_NO_DROPBUTTON)) {
		DropButton *btn = new DropButton(this, this);
		int flag(wxALIGN_CENTER_VERTICAL);
		if (sizerOri == wxVERTICAL) {
			flag = wxALIGN_CENTER_HORIZONTAL;
		}
		sz->Add(btn, 0, flag|wxEXPAND|wxALL, 0);
	}

	sz->Add(m_tabsSizer, 1, wxEXPAND);
	sz->Layout();
}

void wxTabContainer::InsertTab(CustomTab* tab, size_t index)
{
	size_t oldSel(0);

	if (tab->GetSelected() == false && GetTabsCount() == 0) {
		tab->SetSelected(true);
		PushPageHistory(tab);
	}

	if (index >= GetTabsCount()) {
		if (m_orientation == wxLEFT || m_orientation == wxRIGHT) {
			m_tabsSizer->Add(tab, 0, wxLEFT | wxRIGHT, 3);
		} else {
			m_tabsSizer->Add(tab, 0, wxTOP | wxBOTTOM, 3);
		}
	} else {
		if (m_orientation == wxLEFT || m_orientation == wxRIGHT) {
			m_tabsSizer->Insert(index, tab, 0, wxLEFT | wxRIGHT, 3);
		} else {
			m_tabsSizer->Insert(index, tab, 0, wxTOP | wxBOTTOM, 3);
		}
	}

	if (tab->GetSelected()) {
		//find the old selection and unselect it
		CustomTab *selectedTab = GetSelection();

		if (selectedTab && selectedTab != tab) {
			selectedTab->SetSelected( false );
			oldSel = TabToIndex( selectedTab );
		}
	}

	m_tabsSizer->Layout();
	if (tab->GetSelected()) {

		EnsureVisible(tab);
		PushPageHistory(tab);

		//fire page changed event
		NotebookEvent event(wxEVT_COMMAND_BOOK_PAGE_CHANGED, GetId());
		event.SetSelection( TabToIndex(tab) );
		event.SetOldSelection( oldSel );
		event.SetEventObject( GetParent() );
		GetParent()->GetEventHandler()->ProcessEvent(event);
	}
}

void wxTabContainer::AddTab(CustomTab *tab)
{
	InsertTab(tab, GetTabsCount());
}

CustomTab* wxTabContainer::GetSelection()
{
	//iteratre over the items in the sizer, and search for the selected one
	wxSizer *sz = m_tabsSizer;
	wxSizerItemList items = sz->GetChildren();

	wxSizerItemList::iterator iter = items.begin();
	for (; iter != items.end(); iter++) {
		wxSizerItem *item = *iter;
		wxWindow *win = item->GetWindow();
		if (win) {
			CustomTab *tab = (CustomTab*)win;
			if (tab && tab->GetSelected()) {
				return tab;
			}
		}
	}
	return NULL;
}

void wxTabContainer::SetSelection(CustomTab *tab, bool notify)
{
	//iteratre over the items in the sizer, and search for the selected one
	if (!tab) {
		return;
	}

	tab->GetWindow()->SetFocus();

	size_t oldSel((size_t)-1);
	if (notify) {
		//send event to noitfy that the page is changing
		oldSel = TabToIndex( GetSelection() );

		NotebookEvent event(wxEVT_COMMAND_BOOK_PAGE_CHANGING, GetId());
		event.SetSelection( TabToIndex( tab ) );
		event.SetOldSelection( oldSel );
		event.SetEventObject( GetParent() );
		GetParent()->GetEventHandler()->ProcessEvent(event);

		if ( !event.IsAllowed() ) {
			return;
		}
	}

	//let the notebook process this first
	Notebook *book = (Notebook *)GetParent();
	if (book) {
		book->SetSelection(tab);
	}

	//find the old selection
	CustomTab *oldSelection = GetSelection();
	if (oldSelection) {
		oldSelection->SetSelected(false);
		oldSelection->Refresh();
	}

	tab->SetSelected(true);
	EnsureVisible(tab);

	tab->Refresh();
	tab->GetWindow()->SetFocus();

	//add this page to the history
	PushPageHistory(tab);

	if (notify) {
		//send event to noitfy that the page has changed
		NotebookEvent event(wxEVT_COMMAND_BOOK_PAGE_CHANGED, GetId());
		event.SetSelection( TabToIndex( tab ) );
		event.SetOldSelection( oldSel );
		event.SetEventObject( GetParent() );
		GetParent()->GetEventHandler()->ProcessEvent(event);
	}
}

CustomTab* wxTabContainer::IndexToTab(size_t page)
{
	//return the tab of given index
	wxSizer *sz = m_tabsSizer;

	//check limit
	if (page >= sz->GetChildren().GetCount()) {
		return NULL;
	}

	wxSizerItem *szItem = sz->GetItem(page);
	if (szItem) {
		return (CustomTab*)szItem->GetWindow();
	}
	return NULL;
}

size_t wxTabContainer::TabToIndex(CustomTab *tab)
{
	//iteratre over the items in the sizer, and search for the selected one
	if (!tab) {
		return static_cast<size_t>(-1);
	}

	wxSizer *sz = m_tabsSizer;
	wxSizerItemList items = sz->GetChildren();

	wxSizerItemList::iterator iter = items.begin();
	for (size_t i=0; iter != items.end(); iter++, i++) {
		wxSizerItem *item = *iter;
		wxWindow *win = item->GetWindow();
		if (win == tab) {
			return i;
		}
	}
	return static_cast<size_t>(-1);
}

size_t wxTabContainer::GetTabsCount()
{
	wxSizer *sz = m_tabsSizer;
	wxSizerItemList items = sz->GetChildren();
	return items.GetCount();
}

void wxTabContainer::OnPaint(wxPaintEvent &e)
{
	wxBufferedPaintDC dc(this);
	Notebook *book = (Notebook *)GetParent();

	wxRect rr = GetClientSize();

	if (GetTabsCount() == 0) {
		dc.SetPen(PNAEL_BG_COLOUR);
		dc.SetBrush(PNAEL_BG_COLOUR);
		dc.DrawRectangle(rr);
		return;
	}

	DoDrawBackground(dc, book->GetBookStyle() & wxVB_BG_GRADIENT, m_orientation, rr);

	//draw border around the tab area
	if (book->m_style & wxVB_BORDER) {
		wxColour borderColour = DrawingUtils::LightColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW), DrawingUtils::GetDdkShadowLightFactor2());
		dc.SetPen(borderColour);
		dc.SetBrush(*wxTRANSPARENT_BRUSH);
		dc.DrawRectangle(rr);
	}

	DoDrawMargin(dc, m_orientation, rr);
}

void wxTabContainer::OnEraseBg(wxEraseEvent &)
{
	//do nothing
}

void wxTabContainer::OnSizeEvent(wxSizeEvent &e)
{
	wxCommandEvent evt(wxEVT_CMD_ENSURE_SEL_TAB_VISIBILE);
	AddPendingEvent(evt);

	Refresh();
	e.Skip();
}

void wxTabContainer::SetOrientation(const int& orientation)
{
	this->m_orientation = orientation;

	wxBoxSizer *sz = (wxBoxSizer*)m_tabsSizer;
	wxSizerItemList items = sz->GetChildren();

	std::vector<CustomTab*> tabs;
	wxSizerItemList::iterator iter = items.begin();
	for (; iter != items.end(); iter++) {
		wxSizerItem *item = *iter;
		wxWindow *win = item->GetWindow();
		if (win) {
			CustomTab *curtab = (CustomTab*)win;
			curtab->SetOrientation(m_orientation);
			tabs.push_back(curtab);
		}
	}
	sz->Clear();

	switch (orientation) {
	case wxTOP:
	case wxBOTTOM:
		((wxBoxSizer*)m_tabsSizer)->SetOrientation(wxHORIZONTAL);
		((wxBoxSizer*)GetSizer())->SetOrientation(wxHORIZONTAL);
		break;
	default:
		((wxBoxSizer*)m_tabsSizer)->SetOrientation(wxVERTICAL);
		((wxBoxSizer*)GetSizer())->SetOrientation(wxVERTICAL);
		break;
	}

	// add the tabs
	for (size_t i=0; i<tabs.size(); i++) {
		AddTab(tabs.at(i));
	}

	Layout();
	GetSizer()->Layout();
}

void wxTabContainer::SetDraggedTab(CustomTab *tab)
{
	m_draggedTab = tab;
}

void wxTabContainer::SwapTabs(CustomTab *tab)
{
	if (m_draggedTab == tab) {
		return;
	}
	if (m_draggedTab == NULL) {
		return;
	}

	int orientation(wxBOTTOM);

	size_t index = TabToIndex(tab);
	if (index == static_cast<size_t>(-1)) {
		return;
	}

	size_t index2 = TabToIndex(m_draggedTab);
	if (index2 == static_cast<size_t>(-1)) {
		return;
	}

	orientation = index2 > index ? wxTOP : wxBOTTOM;

	Freeze();
	//detach the dragged tab from the sizer
	m_tabsSizer->Detach(m_draggedTab);

	int flags(0);
	if (m_orientation == wxLEFT || m_orientation == wxRIGHT) {
		flags = wxLEFT | wxRIGHT;
	} else {
		flags = wxTOP | wxBOTTOM;
	}

	index = TabToIndex(tab);
	if (orientation == wxBOTTOM) {
		//tab is being dragged bottom
		if (index == GetTabsCount()-1) {
			//last tab
			m_tabsSizer->Add(m_draggedTab, 0, flags, 3);
		} else {
			m_tabsSizer->Insert(index+1, m_draggedTab, 0, flags, 3);
		}
	} else {
		//dragg is going up
		m_tabsSizer->Insert(index, m_draggedTab, 0, flags, 3);
	}
	Thaw();
	m_tabsSizer->Layout();

}

void wxTabContainer::OnLeaveWindow(wxMouseEvent &e)
{
	wxUnusedVar(e);
	m_draggedTab = NULL;
}

void wxTabContainer::OnLeftUp(wxMouseEvent &e)
{
	wxUnusedVar(e);
	m_draggedTab = NULL;
}

void wxTabContainer::PushPageHistory(CustomTab *page)
{
	if (page == NULL)
		return;

	int where = m_history.Index(page);
	//remove old entry of this page and re-insert it as first
	if (where != wxNOT_FOUND) {
		m_history.Remove(page);
	}
	m_history.Insert(page, 0);
}

void wxTabContainer::PopPageHistory(CustomTab *page)
{
	int where = m_history.Index(page);
	while (where != wxNOT_FOUND) {
		CustomTab *tab = static_cast<CustomTab *>(m_history.Item(where));
		m_history.Remove(tab);

		//remove all appearances of this page
		where = m_history.Index(page);
	}
}

CustomTab* wxTabContainer::GetPreviousSelection()
{
	if (m_history.empty()) {
		return NULL;
	}
	//return the top of the heap
	return static_cast<CustomTab*>( m_history.Item(0));
}

bool wxTabContainer::DeletePage(CustomTab *deleteTab, bool notify)
{
	return DoRemoveTab(deleteTab, true, notify);
}

bool wxTabContainer::RemovePage(CustomTab *removePage, bool notify)
{
	return DoRemoveTab(removePage, false, notify);
}

bool wxTabContainer::DoRemoveTab(CustomTab *deleteTab, bool deleteIt, bool notify)
{
	if (deleteTab == NULL) {
		return false;
	}

	size_t pageIndex = TabToIndex( deleteTab );
	if (notify) {
		//send event to noitfy that the page has changed
		NotebookEvent event(wxEVT_COMMAND_BOOK_PAGE_CLOSING, GetId());
		event.SetSelection( pageIndex );
		event.SetEventObject( GetParent() );
		GetParent()->GetEventHandler()->ProcessEvent(event);

		if (!event.IsAllowed()) {
			return false;
		}
	}

	//detach the tab from the tabs container
	if (m_tabsSizer->Detach(deleteTab)) {
		//the  tab was removed successfully
		//locate next item to be selected
		PopPageHistory(deleteTab);

		CustomTab *nextSelection = GetPreviousSelection();
		if ( !nextSelection && this->GetTabsCount() > 0) {
			nextSelection = IndexToTab(0);
		}

		Notebook *book = (Notebook*)GetParent();
		if (book && nextSelection) {
			SetSelection(nextSelection, true);
		}

		//remove the window from the parents' sizer
		GetParent()->GetSizer()->Detach(deleteTab->GetWindow());

		//destroy the window as well?
		if (deleteIt) {
			deleteTab->GetWindow()->Destroy();
		}

		//you can now safely destroy the visual tab button
		deleteTab->Destroy();
	}
	if ( GetTabsCount() == 0 ) {
		Hide();
	}

	m_tabsSizer->Layout();
	GetParent()->GetSizer()->Layout();

	if (notify) {
		//send event to noitfy that the page has been closed
		NotebookEvent event(wxEVT_COMMAND_BOOK_PAGE_CLOSED, GetId());
		event.SetSelection( pageIndex );
		event.SetEventObject( GetParent() );
		GetParent()->GetEventHandler()->ProcessEvent(event);
	}

	return true;
}

void wxTabContainer::EnsureVisible(CustomTab *tab)
{
	if ( !tab )
		return;

	wxWindowUpdateLocker locker(this);

	//make sure all tabs are hidden
	for ( size_t i=0; i< GetTabsCount(); i++ ) {
		if (m_tabsSizer->IsShown(i)) {
			m_tabsSizer->Show(i, false);
		}
	}
	
	// The area of the tabs depends on the orientation
	size_t areaLen = GetSize().x;
	if(m_orientation == wxLEFT || m_orientation == wxRIGHT)
		areaLen = GetSize().y;

	size_t tabIdx = TabToIndex(tab);
	size_t lengthUpUntilTheTab(0);

	for (size_t i=0; i<=tabIdx; i++) {
		// start hiding from this tab
		lengthUpUntilTheTab += IndexToTab(i)->GetTabWidth();
	}
	
	// The is visible, check if we can add some more tabs to the right
	if ( lengthUpUntilTheTab <= areaLen ) {

		// Make sure all tabs to the left including our tab are visible
		for ( size_t i=0; i<= tabIdx; i++ ) {
			if (m_tabsSizer->IsShown(i) == false) {
				m_tabsSizer->Show(i);
			}
		}
		
		// Try to add some more tabs to the right
		size_t i=tabIdx+1;
		for(; i<GetTabsCount(); i++) {
			CustomTab *t = IndexToTab(i);
			size_t diff = (t->GetTabWidth() + lengthUpUntilTheTab) - areaLen;
			if((t->GetTabWidth() + lengthUpUntilTheTab <= areaLen) || (diff < (size_t)t->GetTabWidth() && diff > 30 ))
			{
				lengthUpUntilTheTab += t->GetTabWidth();
				if(m_tabsSizer->IsShown(t) == false)
					m_tabsSizer->Show(t);
			} else 
				break;
		}
		
		// Hide the rest of the tabs
		for(; i<GetTabsCount(); i++) {
			if(m_tabsSizer->IsShown(i))
				m_tabsSizer->Show(i, false);
		}
	} else {
		// Our tab is not visible. Hide all the tabs from the right side of our tab
		// Try to add some more tabs to the right
		
		// Hide the rest of the tabs
		for(size_t i=tabIdx+1; i<GetTabsCount(); i++) {
			if(m_tabsSizer->IsShown(i))
				m_tabsSizer->Show(i, false);
		}
		
		// Now start hiding tabs from the left starting from 0
		size_t i=0;
		for(; i<tabIdx; i++) {
			CustomTab *t = IndexToTab(i);
			if(m_tabsSizer->IsShown(i))
				m_tabsSizer->Show(i, false);
				
			lengthUpUntilTheTab -= t->GetTabWidth();
			if( lengthUpUntilTheTab + 30 <= areaLen) {
				// our tab is now visible, no need to hide more
				break;
			}
		}
		
		// Show all tabs from i up until our tab
		for(; i<=tabIdx; i++) {
			m_tabsSizer->Show(i);
		}
	}
	
	m_tabsSizer->Layout();
}

bool wxTabContainer::IsVisible(CustomTab *tab, bool fullShown)
{
	wxPoint pos = tab->GetPosition();
	wxSize tabSize = tab->GetSize();
	wxRect rr = GetParent()->GetSize();

	// get the accumulated size
	// up until this tab

	int lengthTaken(16); // 16 pixels for the drop down button
	for (size_t i=0; i<GetTabsCount(); i++)
	{
		CustomTab* t = IndexToTab(i);
		if (t == tab)
			break;
		else
			lengthTaken += t->GetTabWidth();
	}

	if (lengthTaken >= rr.width)
	{
		return false;
	}
	return true;
}

void wxTabContainer::OnDeleteTab(wxCommandEvent &e)
{
	CustomTab *tab = (CustomTab*)e.GetEventObject();
	if (tab)
	{
		GetParent()->Freeze();
		DeletePage(tab, true);
		GetParent()->Thaw();
	}
}

void wxTabContainer::Resize()
{
	//refersh all tabs
	wxSizer *sz = m_tabsSizer;
	wxSizerItemList items = sz->GetChildren();

	wxSizerItemList::iterator iter = items.begin();
	for (; iter != items.end(); iter++)
	{
		wxSizerItem *item = *iter;
		wxWindow *win = item->GetWindow();
		if (win) {
			CustomTab *curtab = (CustomTab*)win;

			//refresh only visible tabs
			if (IsVisible(curtab, false)) {
				curtab->Refresh();
			}

			// force re-initialization even for non-visible items
			curtab->Initialize();
		}
	}

	GetSizer()->Layout();
	m_tabsSizer->Layout();
	Refresh();
}

void wxTabContainer::ShowPopupMenu()
{
	PopupMenu( m_rightClickMenu );
}

void wxTabContainer::OnDoubleClick(wxMouseEvent& e)
{
	e.Skip();
}

size_t wxTabContainer::GetFirstVisibleTab()
{
	for ( size_t i=0; i< GetTabsCount(); i++ )
	{
		CustomTab *t = IndexToTab(i);
		if (m_tabsSizer->IsShown(t)) {
			return i;
		}
	}
	return Notebook::npos;
}

size_t wxTabContainer::GetLastVisibleTab()
{
	size_t last( Notebook::npos );
	for ( size_t i=0; i< GetTabsCount(); i++ )
	{
		CustomTab *t = IndexToTab(i);
		if (m_tabsSizer->IsShown(t)) {
			last = i;
		}
	}
	return last;
}

void wxTabContainer::OnEnsureVisible(wxCommandEvent& e)
{
	wxUnusedVar(e);
	CustomTab *tab = GetSelection();
	if (tab)
	{
		EnsureVisible( tab );
	}
//	DoShowMaxTabs();
}

//--------------------------------------------------------------------
// Dropbutton class used for the notebook
//--------------------------------------------------------------------

DropButton::DropButton(wxWindow* parent, wxTabContainer* tabContainer)
		: DropButtonBase(parent)
		, m_tabContainer(tabContainer)
{
}

DropButton::~DropButton()
{
}

size_t DropButton::GetItemCount()
{
	return m_tabContainer ? m_tabContainer->GetTabsCount() : 0;
}

wxString DropButton::GetItem(size_t n)
{
	return m_tabContainer->IndexToTab(n)->GetText();
}

bool DropButton::IsItemSelected(size_t n)
{
	return m_tabContainer->GetSelection() == m_tabContainer->IndexToTab(n);
}

void DropButton::OnMenuSelection(wxCommandEvent &e)
{
	if (e.GetId() == XRCID("customize"))
	{
		Notebook *bk = (Notebook*)m_tabContainer->GetParent();
		NotebookCustomDlg dlg(wxTheApp->GetTopWindow(), bk, bk->GetFixedTabWidth());
		dlg.ShowModal();

	} else
	{
		size_t item = (size_t)e.GetId();
		CustomTab *tab = m_tabContainer->IndexToTab(item);
		m_tabContainer->SetSelection(tab, true);
	}
}

void DropButton::OnPaint(wxPaintEvent& e)
{
	wxRect rr = GetSize();
	wxBufferedPaintDC dc(this);

	if (GetItemCount() == 0)
	{
		dc.SetPen(PNAEL_BG_COLOUR);
		dc.SetBrush(PNAEL_BG_COLOUR);
		dc.DrawRectangle(rr);
		return;
	}

	Notebook *book = (Notebook *)m_tabContainer->GetParent();
	wxTabContainer::DoDrawBackground(dc, false, m_tabContainer->GetOrientation(), rr);

	if (IsEnabled() && GetItemCount() > 0)
	{
		// drow the drop down arrow
		int bmpWidth = m_arrowDownBmp.GetWidth();
		int bmpHeight = m_arrowDownBmp.GetHeight();

		int bmpX = (rr.width - bmpWidth) / 2;
		int bmpY = (rr.height - bmpHeight) / 2;
		dc.DrawBitmap(m_arrowDownBmp, bmpX, bmpY, true);
	}

	if (book->GetBookStyle() & wxVB_BORDER)
	{

		wxColour borderColour = DrawingUtils::LightColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DDKSHADOW), DrawingUtils::GetDdkShadowLightFactor2());
		dc.SetPen(borderColour);

		// top
		dc.DrawLine(rr.x, rr.y, rr.x+rr.width, rr.y);
		// bottom
		dc.DrawLine(rr.x, rr.y+rr.height, rr.x+rr.width, rr.y+rr.height);
		// left
		dc.DrawLine(rr.x, rr.y, rr.x, rr.y+rr.height);
	}

	wxTabContainer::DoDrawMargin(dc, m_tabContainer->GetOrientation(), rr);
}

int wxTabContainer::GetBookStyle()
{
	Notebook *book = (Notebook *)GetParent();
	return book->GetBookStyle();
}

int wxTabContainer::GetBmpHeight() const
{
	return m_bmpHeight;
}

void wxTabContainer::SetBmpHeight(int height)
{
	if (height < 0)
	{
		return;
	}

	m_bmpHeight = height;
	Resize();
}

void wxTabContainer::SetFixedTabWidth(const size_t& fixedTabWidth)
{
	this->m_fixedTabWidth = fixedTabWidth;
	Resize();
	Layout();

}

void DropButton::OnLeftDown(wxMouseEvent& e)
{
	size_t count = GetItemCount();
	if (count == 0)
	{
		return;
	}

	wxRect rr = GetSize();
	wxMenu popupMenu;

#ifdef __WXMSW__
	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
#endif

	for (size_t i=0; i<count; i++)
	{
		wxString text = GetItem(i);
		bool selected = IsItemSelected(i);

		wxMenuItem *item = new wxMenuItem(&popupMenu, static_cast<int>(i), text, text, wxITEM_CHECK);

		//set the font
#ifdef __WXMSW__
		if (selected) {
			font.SetWeight(wxBOLD);
		}
		item->SetFont(font);
#endif
		popupMenu.Append(item);

		//mark the selected item
		item->Check(selected);

		//restore font
#ifdef __WXMSW__
		font.SetWeight(wxNORMAL);
#endif
	}

	wxMenuItem *item = new wxMenuItem(&popupMenu, XRCID("customize"), wxT("Customize..."), wxT("Customize..."), wxITEM_NORMAL);
#ifdef __WXMSW__
	font.SetWeight(wxNORMAL);
	item->SetFont(font);
#endif
	popupMenu.AppendSeparator();
	popupMenu.Append(item);

	// connect an event handler to our menu
	popupMenu.Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(DropButtonBase::OnMenuSelection), NULL, this);

	m_state = BTN_PUSHED;
	Refresh();
	PopupMenu( &popupMenu, 0, rr.y + rr.height );

	m_state = BTN_NONE;
	Refresh();
}
