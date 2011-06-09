#ifndef __auicontrolbar__
#define __auicontrolbar__

#include <wx/panel.h>
#include <wx/tglbtn.h>
#include <wx/window.h>
#include <wx/bitmap.h>
#include <vector>
#include "custom_notebook.h"
#include <wx/textctrl.h>

//--------------------------------------------------------

extern wxString ST_CLASS;
extern wxString ST_WORKSPACE_FILE;
extern wxString ST_MACRO;
extern wxString ST_TYPEDEF;
extern wxString ST_FUNCTION;
extern wxString TYPE_HERE_TEXT;

//--------------------------------------------------------

class wxAuiManager;
class OutputViewControlBarButton;
class Notebook;
class OutputViewControlBarToggleButton;

//--------------------------------------------------------

class OutputViewControlBar : public wxPanel
{
	wxAuiManager *                           m_aui;
	Notebook*                                m_book;
	OutputViewControlBarButton*              m_moreButton;

public:
#ifndef __WXGTK__
	std::vector<OutputViewControlBarButton*>       m_buttons;
#else
	std::vector<OutputViewControlBarToggleButton*> m_buttons;
#endif
	OutputViewControlBar(wxWindow *win, Notebook *book, wxAuiManager *aui, wxWindowID id);

	virtual ~OutputViewControlBar();

	void AddButton        (const wxString &text, const wxBitmap & bmp, bool selected, long style);
	void AddAllButtons    ();

	void OnPaint                (wxPaintEvent      &event);
	void OnEraseBackground      (wxEraseEvent      &event);
	void OnButtonClicked        (wxCommandEvent    &event);
	void OnPageChanged          (NotebookEvent     &event);
	void OnRender               (wxAuiManagerEvent &event);
	void OnSize                 (wxSizeEvent       &event);
	void OnMenuSelection        (wxCommandEvent    &event);
	void OnEditorFocus          (wxCommandEvent    &event);
	void OnEditorSettingsChanged(wxCommandEvent    &event);
	DECLARE_EVENT_TABLE()

protected:
	void        DoTogglePane     (bool hide = true);
	void        DoToggleButton   (wxWindow *button, bool fromMenu);
	wxWindow *  DoFindButton     (const wxString &name);
	void        DoMarkActive     (const wxString &name);
	bool        DoFindDockInfo   (const wxString &saved_perspective, const wxString &dock_name, wxString &dock_info);
};

//--------------------------------------------------------

extern const wxEventType EVENT_BUTTON_PRESSED;
extern const wxEventType EVENT_SHOW_MENU;

class OutputViewControlBarButton : public wxPanel
{
	int           m_state;
	wxString      m_text;
	wxBitmap      m_bmp;
	long          m_style;
public:
	/**
	 * button states
	 */
	enum {
		Button_Pressed = 0,
		Button_Normal     ,
		Button_Hover
	};

	enum {
		Button_UseXSpacer = 0x00000001,
		Button_UseText    = 0x00000002,
		Button_Default    = Button_UseXSpacer | Button_UseText
	};

	static int DoCalcButtonWidth (wxWindow *win, const wxString &text, const wxBitmap &bmp, int spacer);
	static int DoCalcButtonHeight(wxWindow *win, const wxString &text, const wxBitmap &bmp, int spacer);
	void       DoShowPopupMenu ();

public:
	OutputViewControlBarButton(wxWindow *win, const wxString &title, const wxBitmap &bmp, long style = Button_Default);
	virtual ~OutputViewControlBarButton();

	void SetBmp(const wxBitmap& bmp) {
		this->m_bmp = bmp;
	}

	void SetState(const int& state) {
		this->m_state = state;
	}

	void SetText(const wxString& text) {
		this->m_text = text;
	}

	const wxBitmap& GetBmp() const {
		return m_bmp;
	}

	const int& GetState() const {
		return m_state;
	}

	const wxString& GetText() const {
		return m_text;
	}

	DECLARE_EVENT_TABLE();
	virtual void OnPaint          (wxPaintEvent &event);
	virtual void OnEraseBackground(wxEraseEvent &event);
	virtual void OnMouseLDown     (wxMouseEvent &event);
};

class OutputViewControlBarToggleButton : public wxToggleButton
{
	void DoShowPopupMenu();
public:
	OutputViewControlBarToggleButton(wxWindow *parent, const wxString &label);
	virtual ~OutputViewControlBarToggleButton();

	void SetText(const wxString& text) {
		SetLabel(text);
	}

	wxString GetText() const {
		return GetLabel();
	}

	DECLARE_EVENT_TABLE()
	void OnButtonToggled(wxCommandEvent &e);
};

#endif // __auicontrolbar__
