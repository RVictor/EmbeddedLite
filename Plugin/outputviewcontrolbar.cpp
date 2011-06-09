#include "outputviewcontrolbar.h"
#include "globals.h"
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/frame.h>
#include "editor_config.h"
#include <wx/app.h>
#include "plugin.h"
#include <wx/image.h>
#include <wx/menu.h>
#include <wx/tokenzr.h>
#include "custom_tab.h"
#include "custom_notebook.h"
#include "custom_tabcontainer.h"
#include <wx/sizer.h>
#include "drawingutils.h"
#include <wx/dcbuffer.h>
#include <wx/settings.h>
#include <wx/xrc/xmlres.h>
#include <wx/log.h>
#include <wx/aui/framemanager.h>

static unsigned char list_bits[] = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x0f, 0xf8, 0xff, 0xff, 0x0f, 0xf8, 0x1f, 0xfc, 0x3f, 0xfe, 0x7f, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

#ifdef __WXMSW__
#    define BUTTON_SPACER_X 5
#    define BUTTON_SPACER_Y 4
#    define BAR_SPACER      6
#    define DARK_FACTOR     2
#elif defined(__WXGTK__)
#    define BUTTON_SPACER_X 4
#    define BUTTON_SPACER_Y 3
#    define BAR_SPACER      5
#    define DARK_FACTOR     2
#else // __WXMAC__
#    define BUTTON_SPACER_X 6
#    define BUTTON_SPACER_Y 4
#    define BAR_SPACER      6
#    define DARK_FACTOR     1.2
#endif

wxString ST_CLASS          = wxT("Class, struct or union");
wxString ST_WORKSPACE_FILE = wxT("Solution file");
wxString ST_MACRO          = wxT("Macro");
wxString ST_TYPEDEF        = wxT("Typedef");
wxString ST_FUNCTION       = wxT("Function");
wxString TYPE_HERE_TEXT    = wxT("<Type String to Search>");


BEGIN_EVENT_TABLE(OutputViewControlBar, wxPanel)
	#if defined(__WXMSW__)||defined(__WXMAC__)
	EVT_PAINT           (OutputViewControlBar::OnPaint)
	EVT_ERASE_BACKGROUND(OutputViewControlBar::OnEraseBackground)
	#endif
	EVT_COMMAND         (wxID_ANY, EVENT_BUTTON_PRESSED, OutputViewControlBar::OnButtonClicked)
	EVT_SIZE            (OutputViewControlBar::OnSize)
END_EVENT_TABLE()

OutputViewControlBar::OutputViewControlBar(wxWindow* win, Notebook *book, wxAuiManager *aui, wxWindowID id)
		: wxPanel(win, id)
		, m_aui  (aui)
		, m_book (book)
		, m_moreButton(NULL)
{
	wxBoxSizer *mainSizer = new wxBoxSizer(wxHORIZONTAL);
	SetSizer( mainSizer );

	if ( m_book ) {
		m_book->Connect(wxEVT_COMMAND_BOOK_PAGE_CHANGED, NotebookEventHandler(OutputViewControlBar::OnPageChanged), NULL, this);
	}

	if ( m_aui ) {
		m_aui->Connect(wxEVT_AUI_RENDER, wxAuiManagerEventHandler(OutputViewControlBar::OnRender), NULL, this);
	}
	wxTheApp->Connect ( wxEVT_EDITOR_CLICKED         , wxCommandEventHandler ( OutputViewControlBar::OnEditorFocus          ), NULL, this );
	wxTheApp->Connect ( wxEVT_EDITOR_SETTINGS_CHANGED, wxCommandEventHandler ( OutputViewControlBar::OnEditorSettingsChanged), NULL, this );
}

OutputViewControlBar::~OutputViewControlBar()
{
}

void OutputViewControlBar::OnEraseBackground(wxEraseEvent& event)
{
	// do nothing
}

void OutputViewControlBar::OnPaint(wxPaintEvent& event)
{
	wxUnusedVar(event);
	wxBufferedPaintDC dc(this);

	wxRect rect = GetClientSize();

	dc.SetPen  (wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
	dc.SetBrush(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
	dc.DrawRectangle( rect );
}

void OutputViewControlBar::AddButton(const wxString& text, const wxBitmap& bmp, bool selected, long style)
{
#ifndef __WXGTK__
	OutputViewControlBarButton *button = new OutputViewControlBarButton(this, text, bmp, style);
	button->SetState( selected ? OutputViewControlBarButton::Button_Pressed : OutputViewControlBarButton::Button_Normal );
	m_buttons.push_back( button );
#else
	OutputViewControlBarToggleButton *button = new OutputViewControlBarToggleButton(this, text);
	button->SetValue(selected);
	m_buttons.push_back( button );
#endif

	GetSizer()->Add(button, 0, wxLEFT|wxTOP|wxBOTTOM | wxEXPAND, 3);
	GetSizer()->Layout();
	button->Refresh();
}

void OutputViewControlBar::OnButtonClicked(wxCommandEvent& event)
{
#ifndef __WXGTK__
	OutputViewControlBarButton *button = (OutputViewControlBarButton *)event.GetEventObject();
#else
	OutputViewControlBarToggleButton *button = (OutputViewControlBarToggleButton *)event.GetEventObject();
#endif
	DoToggleButton( button, false );
}

void OutputViewControlBar::DoTogglePane(bool hide)
{
	static wxString saved_dock_info;
	if ( m_book && m_aui ) {
		wxAuiPaneInfo &pane_info = m_aui->GetPane(wxT("Output View"));
		wxString dock_info ( wxString::Format(wxT("dock_size(%d,%d,%d)"), pane_info.dock_direction, pane_info.dock_layer, pane_info.dock_row) );
		if ( hide ) {
			if ( pane_info.IsShown() ) {
				wxTheApp->GetTopWindow()->Freeze();

				DoFindDockInfo(m_aui->SavePerspective(), dock_info, saved_dock_info);
				pane_info.Hide();

				m_aui->Update();

				wxTheApp->GetTopWindow()->Thaw();
			}


		} else {
			// Show it
			if ( pane_info.IsShown() == false ) {
				wxTheApp->GetTopWindow()->Freeze();
				if ( saved_dock_info.IsEmpty() ) {
					pane_info.Show();
					m_aui->Update();
				} else {
					wxString auiPerspective = m_aui->SavePerspective();
					if ( auiPerspective.Find(dock_info) == wxNOT_FOUND ) {
						// the dock_info does not exist
						auiPerspective << saved_dock_info << wxT("|");
						m_aui->LoadPerspective(auiPerspective, false);
						pane_info.Show();
						m_aui->Update();
					} else {
						pane_info.Show();
						m_aui->Update();
					}
				}
				wxTheApp->GetTopWindow()->Thaw();
			}
		}

	}
}

void OutputViewControlBar::OnRender(wxAuiManagerEvent& event)
{
	if ( m_aui && m_aui->GetPane(wxT("Output View")).IsShown() == false ) {
		DoMarkActive( wxEmptyString );
	} else if ( m_aui ) {
		DoMarkActive ( m_book->GetPageText( m_book->GetSelection() ) );
	}
	event.Skip();
}

void OutputViewControlBar::DoMarkActive(const wxString& name)
{
#ifndef __WXGTK__
	for (size_t i=0; i<m_buttons.size(); i++) {
		OutputViewControlBarButton *button = m_buttons.at(i);
		if ( button->GetText() == name ) {
			button->SetState(OutputViewControlBarButton::Button_Pressed);
		} else {
			button->SetState(OutputViewControlBarButton::Button_Normal);
		}
		button->Refresh();
	}
#else
	for (size_t i=0; i<m_buttons.size(); i++) {
		OutputViewControlBarToggleButton *button = m_buttons.at(i);
		if ( button->GetText() == name ) {
			button->SetValue(true);
		} else {
			button->SetValue(false);
		}
	}
#endif

	if ( m_book && name.IsEmpty() == false ) {
		for (size_t i=0; i<m_book->GetPageCount(); i++) {
			if ( m_book->GetPageText(i) == name ) {
				m_book->SetSelection(i, false);
				break;
			}
		}
	}
}

void OutputViewControlBar::AddAllButtons()
{
	// add the 'more' button
	wxColour color(*wxBLACK);
	wxImage img = wxBitmap((const char*)list_bits, 16, 16).ConvertToImage();
	img.Replace(0, 0, 0, 123, 123, 123);
	img.Replace(255,255,255,color.Red(),color.Green(),color.Blue());
	img.SetMaskColour(123, 123, 123);

	// Add the 'More' button
	m_moreButton = new OutputViewControlBarButton(this, wxT("More"), wxBitmap(img), 0);
	m_moreButton->SetState( OutputViewControlBarButton::Button_Normal );

	//m_buttons.push_back( m_searchBar );
	GetSizer()->Add(m_moreButton, 0, wxALL | wxEXPAND, 1);
	GetSizer()->Layout();

	if ( m_book ) {
		for (size_t i=0; i<m_book->GetPageCount(); i++) {
			wxString text = m_book->GetPageText(i);
			wxBitmap bmp  = m_book->GetTabContainer()->IndexToTab(i)->GetBmp();

			AddButton(text, bmp, m_book->GetSelection() == i, OutputViewControlBarButton::Button_Default);
		}
	}
}

void OutputViewControlBar::OnPageChanged(NotebookEvent& event)
{
	size_t cursel = m_book->GetSelection();
	if ( cursel != Notebook::npos ) {
		wxString selectedPageText = m_book->GetPageText(cursel);
		DoMarkActive( selectedPageText );
	}
	event.Skip();
}

bool OutputViewControlBar::DoFindDockInfo(const wxString &saved_perspective, const wxString &dock_name, wxString &dock_info)
{
	// search for the 'Output View' perspective
	wxArrayString panes = wxStringTokenize(saved_perspective, wxT("|"), wxTOKEN_STRTOK);
	for (size_t i=0; i<panes.GetCount(); i++) {
		if ( panes.Item(i).StartsWith(dock_name) ) {
			dock_info = panes.Item(i);
			return true;
		}
	}
	return false;
}


void OutputViewControlBar::OnSize(wxSizeEvent& event)
{
	event.Skip();
}

void OutputViewControlBar::DoToggleButton(wxWindow* button, bool fromMenu)
{
#ifndef __WXGTK__
	OutputViewControlBarButton *bt = (OutputViewControlBarButton*)button;
	if ( bt && bt->GetState() == OutputViewControlBarButton::Button_Pressed ) {
		// second click on an already pressed button, hide the AUI pane
		bt->SetState(OutputViewControlBarButton::Button_Normal);
		bt->Refresh();

		// hide the pane
		DoTogglePane(true);

	}
#else
	OutputViewControlBarToggleButton *bt = (OutputViewControlBarToggleButton*)button;
	if ( (!fromMenu && (bt && !bt->GetValue())) ||
	        (fromMenu && (bt && bt->GetValue()))) {
		// second click on an already pressed button, hide the AUI pane
		bt->SetValue(false);

		// hide the pane
		DoTogglePane(true);
	}
#endif
	else if ( bt ) {
		DoMarkActive( bt->GetText() );
		DoTogglePane(false);
	}
}

wxWindow* OutputViewControlBar::DoFindButton(const wxString& name)
{
	for (size_t i=0; i<m_buttons.size(); i++) {
#ifndef __WXGTK__
		OutputViewControlBarButton *      button = m_buttons.at(i);
#else
		OutputViewControlBarToggleButton *button = m_buttons.at(i);
#endif
		if ( button->GetText() == name ) {
			return button;
		}
	}
	return NULL;
}


void OutputViewControlBar::OnMenuSelection(wxCommandEvent& event)
{
	for (size_t i=0; i<m_buttons.size(); i++) {
#ifndef __WXGTK__
		OutputViewControlBarButton *      button = m_buttons.at(i);
#else
		OutputViewControlBarToggleButton *button = m_buttons.at(i);
#endif
		if ( wxXmlResource::GetXRCID(button->GetText().c_str()) == event.GetId() ) {
			DoToggleButton(button, true);
			break;
		}
	}
}

void OutputViewControlBar::OnEditorSettingsChanged(wxCommandEvent& event)
{
	event.Skip();
}

void OutputViewControlBar::OnEditorFocus(wxCommandEvent& event)
{
	event.Skip();

	if (EditorConfigST::Get()->GetOptions()->GetHideOutpuPaneOnUserClick()) {

		// Optionally don't hide the Debug pane: it's irritating during a debug session, you click to set a breakpoint...
		if (EditorConfigST::Get()->GetOptions()->GetHideOutputPaneNotIfDebug()) {
			// Find if it's 'Debug' that's visible
			size_t cursel = m_book->GetSelection();
			if ( cursel != Notebook::npos && m_book->GetPageText(cursel) == wxT("Debug") ) {
				return;
			}
		}

		// re-draw all the buttons
		for (size_t i=0; i<m_buttons.size(); i++) {
#ifndef __WXGTK__
			OutputViewControlBarButton *button = m_buttons.at(i);
			button->SetState( OutputViewControlBarButton::Button_Normal);
			button->Refresh();
#else
			OutputViewControlBarToggleButton *button = m_buttons.at(i);
			button->SetValue(false);
			button->Refresh();
#endif
		}
		// and hide the output view
		DoTogglePane(true);
	}
}

//-----------------------------------------------------------------------------------

const wxEventType EVENT_BUTTON_PRESSED = XRCID("button_pressed");

BEGIN_EVENT_TABLE(OutputViewControlBarButton, wxPanel)
	EVT_PAINT           (OutputViewControlBarButton::OnPaint)
	EVT_ERASE_BACKGROUND(OutputViewControlBarButton::OnEraseBackground)
	EVT_LEFT_DOWN       (OutputViewControlBarButton::OnMouseLDown)
	EVT_LEFT_DCLICK     (OutputViewControlBarButton::OnMouseLDown)
END_EVENT_TABLE()

OutputViewControlBarButton::OutputViewControlBarButton(wxWindow* win, const wxString& title, const wxBitmap& bmp, long style)
		: wxPanel(win)
		, m_state(Button_Normal)
		, m_text (title)
		, m_bmp  (bmp)
		, m_style (style)
{
	if ( title.IsEmpty() && bmp.IsOk() == false ) return;

	SetSizeHints(DoCalcButtonWidth( this,
	                                m_style & Button_UseText  ? m_text : wxT(""),
	                                m_bmp,
	                                m_style & Button_UseXSpacer ? BUTTON_SPACER_X : 1),

	             DoCalcButtonHeight(this, wxEmptyString, m_bmp, BUTTON_SPACER_Y));
}

OutputViewControlBarButton::~OutputViewControlBarButton()
{
}

void OutputViewControlBarButton::OnEraseBackground(wxEraseEvent& event)
{
	wxUnusedVar(event);
}

void OutputViewControlBarButton::OnMouseLDown(wxMouseEvent& event)
{
	// notify our parent

	if ( GetText() == wxT("More") ) {
		DoShowPopupMenu();
	} else {

		wxCommandEvent e(EVENT_BUTTON_PRESSED);
		e.SetEventObject(this);
		GetParent()->GetEventHandler()->AddPendingEvent(e);

	}
}

void OutputViewControlBarButton::OnPaint(wxPaintEvent& event)
{
	int xx, yy;
	wxBufferedPaintDC dc(this);

	// Fill the background
	wxRect rect = GetClientSize();
	dc.SetBrush( DrawingUtils::GetPanelBgColour() );
	dc.SetPen  ( DrawingUtils::GetPanelBgColour() );

	// draw the background
	dc.DrawRectangle( rect );

	// draw the filling
	wxRect tmpRect (rect);
	tmpRect.Deflate(2, 2);
	if ( m_style != 0 ) {
		if ( GetState() == Button_Normal ) {
			DrawingUtils::PaintStraightGradientBox( dc,
			                                        tmpRect,
			                                        wxT("WHITE"),
			                                        DrawingUtils::DarkColour(wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE), DARK_FACTOR),
			                                        true);
		} else {
			DrawingUtils::PaintStraightGradientBox(dc, tmpRect, wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW), wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE), true);
		}
	}

	// Draw the text
	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	dc.SetTextForeground(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNTEXT));
	dc.GetTextExtent(GetText(), &xx, &yy, NULL, NULL, &font);

	int  spacer_x  = m_style & Button_UseXSpacer ? BUTTON_SPACER_X : 1;
	bool draw_text = m_style & Button_UseText    ? true : false       ;

	// draw the bitmap
	int bmp_x(0);
	int bmp_y(0);
	if ( GetBmp().IsOk() ) {
		bmp_x = spacer_x;
		bmp_y = (rect.height - GetBmp().GetHeight())/2;

		dc.DrawBitmap(GetBmp(), bmp_x, bmp_y, true);
	}

	if ( draw_text ) {
		int text_x = bmp_x + GetBmp().GetWidth() + spacer_x;
		int text_y = (rect.height - yy) / 2;
		dc.SetFont( font );
		dc.DrawText(GetText(), text_x, text_y);
	}

	// draw the border
	if ( m_style != 0 ) {
		dc.SetPen(wxSystemSettings::GetColour(wxSYS_COLOUR_3DSHADOW));
		dc.SetBrush( *wxTRANSPARENT_BRUSH );
		dc.DrawRoundedRectangle(rect, 1);

		// draw an inner white rectangle as well
		wxColour innerBorderCol (wxSystemSettings::GetColour(wxSYS_COLOUR_3DFACE));
		innerBorderCol = DrawingUtils::LightColour(innerBorderCol, 3);

		dc.SetPen  ( wxPen(innerBorderCol) );
		dc.SetBrush( *wxTRANSPARENT_BRUSH );
		rect.Deflate(1);
		dc.DrawRoundedRectangle(rect, 0);
	}
}

int OutputViewControlBarButton::DoCalcButtonWidth(wxWindow *win, const wxString &text, const wxBitmap &bmp, int spacer)
{
	int text_width(0);
	int width     (0);

	// Calculate the text width
	if (text.IsEmpty() == false) {
		int yy;
		wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
		win->GetTextExtent(text, &text_width, &yy, NULL, NULL, &font);
	}

	// spacer
	width += spacer;

	// bitmap
	if ( bmp.IsOk() ) {
		width += bmp.GetWidth();
		width += spacer;
	}

	// text
	if ( text_width ) {
		width += text_width;
		width += spacer;
	}

	return width;
}

int OutputViewControlBarButton::DoCalcButtonHeight(wxWindow *win, const wxString &text, const wxBitmap &bmp, int spacer)
{
	int text_height(0);
	int bmp_height (0);
	int height     (0);

	wxUnusedVar(text);

	// Calculate the text height
	int xx;
	wxString stam(wxT("Tp"));
	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	win->GetTextExtent(stam, &xx, &text_height, NULL, NULL, &font);

	// bitmap
	if ( bmp.IsOk() ) {
		bmp_height += bmp.GetHeight();
	}

	height += spacer;
	text_height > bmp_height ? height += text_height : height += bmp_height;
	height += spacer;

	return height;
}
void OutputViewControlBarButton::DoShowPopupMenu()
{
	wxRect rr = GetSize();
	wxMenu popupMenu;

#ifndef __WXGTK__
	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
#endif

	OutputViewControlBar *bar = (OutputViewControlBar *)GetParent();
	for (size_t i=0; i<bar->m_buttons.size(); i++) {
#ifndef __WXGTK__
		OutputViewControlBarButton *button = bar->m_buttons.at(i);
#else
		OutputViewControlBarToggleButton *button = bar->m_buttons.at(i);
#endif
		// Skip the More button and empty text buttons
		if ( button->GetText() == wxT("More") || button->GetText().IsEmpty() ) {
			continue;
		}

		wxString text = button->GetText();
#ifndef __WXGTK__
		bool selected = button->GetState() == OutputViewControlBarButton::Button_Pressed;
#else
		bool selected = button->GetValue();
#endif
		wxMenuItem *item = new wxMenuItem(&popupMenu, wxXmlResource::GetXRCID(button->GetText().c_str()), text, text, wxITEM_CHECK);

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
	popupMenu.Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(OutputViewControlBar::OnMenuSelection), NULL, bar);
	PopupMenu( &popupMenu, rr.x, rr.y );
}

BEGIN_EVENT_TABLE(OutputViewControlBarToggleButton, wxToggleButton)
	EVT_TOGGLEBUTTON(wxID_ANY, OutputViewControlBarToggleButton::OnButtonToggled)
END_EVENT_TABLE()
OutputViewControlBarToggleButton::OutputViewControlBarToggleButton(wxWindow* parent, const wxString& label)
		: wxToggleButton(parent, wxID_ANY, label, wxDefaultPosition, wxDefaultSize, wxBU_EXACTFIT)
{
}

OutputViewControlBarToggleButton::~OutputViewControlBarToggleButton()
{
}

void OutputViewControlBarToggleButton::OnButtonToggled(wxCommandEvent& e)
{
	if ( GetText() == wxT("More") ) {
		DoShowPopupMenu();

	} else {

		wxCommandEvent e(EVENT_BUTTON_PRESSED);
		e.SetEventObject(this);
		GetParent()->GetEventHandler()->AddPendingEvent(e);
	}
}

void OutputViewControlBarToggleButton::DoShowPopupMenu()
{
	wxRect rr = GetSize();
	wxMenu popupMenu;

#ifdef __WXMSW__
	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
#endif

	OutputViewControlBar *bar = (OutputViewControlBar *)GetParent();
	for (size_t i=0; i<bar->m_buttons.size(); i++) {
#ifndef __WXGTK__
		OutputViewControlBarButton *button = bar->m_buttons.at(i);
#else
		OutputViewControlBarToggleButton *button = bar->m_buttons.at(i);
#endif
		wxString text = button->GetText();
#ifndef __WXGTK__
		bool selected = button->GetState() == OutputViewControlBarButton::Button_Pressed;
#else
		bool selected = button->GetValue();
#endif
		wxMenuItem *item = new wxMenuItem(&popupMenu, wxXmlResource::GetXRCID(button->GetText().c_str()), text, text, wxITEM_CHECK);

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
	popupMenu.Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(OutputViewControlBar::OnMenuSelection), NULL, bar);
	PopupMenu( &popupMenu, rr.x, rr.y );
}
