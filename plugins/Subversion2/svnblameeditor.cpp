#include "svnblameeditor.h"
#include <wx/xrc/xmlres.h>
#include "drawingutils.h"
#include <map>
#include <wx/tokenzr.h>
#include <wx/font.h>
#include <wx/settings.h>

BEGIN_EVENT_TABLE(SvnBlameEditor, wxScintilla)
	EVT_CONTEXT_MENU(SvnBlameEditor::OnContextMenu)
END_EVENT_TABLE()

SvnBlameEditor::SvnBlameEditor(wxWindow *win)
		: wxScintilla(win)
{
	Initialize();
}

SvnBlameEditor::~SvnBlameEditor()
{
}

void SvnBlameEditor::Initialize()
{
	// Initialize some styles
	StyleClearAll();
	SetLexer(wxSCI_LEX_NULL);

	wxFont defFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxFont font(defFont.GetPointSize(), wxFONTFAMILY_TELETYPE, wxNORMAL, wxNORMAL);

	for (int i=0; i<=wxSCI_STYLE_DEFAULT; i++) {
		StyleSetBackground(i, wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
		StyleSetForeground(i, *wxBLACK);
		StyleSetFont(i, font);
	}

	SetMarginType (0, wxSCI_MARGIN_TEXT  );
	SetMarginType (1, wxSCI_MARGIN_NUMBER);
	SetMarginWidth(1, 4 + 5*TextWidth(wxSCI_STYLE_LINENUMBER, wxT("9")));
	SetMarginWidth(2, 0);
	SetMarginWidth(3, 0);
	SetMarginWidth(4, 0);
	SetTabWidth(4);

	// Define some colors to use
	StyleSetBackground(1,  DrawingUtils::LightColour(wxT("GREEN"),      7.0));
	StyleSetBackground(2,  DrawingUtils::LightColour(wxT("BLUE"),       7.0));
	StyleSetBackground(3,  DrawingUtils::LightColour(wxT("ORANGE"),     7.0));
	StyleSetBackground(4,  DrawingUtils::LightColour(wxT("YELLOW"),     7.0));
	StyleSetBackground(5,  DrawingUtils::LightColour(wxT("PURPLE"),     7.0));
	StyleSetBackground(6,  DrawingUtils::LightColour(wxT("RED"),        7.0));
	StyleSetBackground(7,  DrawingUtils::LightColour(wxT("BROWN"),      7.0));
	StyleSetBackground(8,  DrawingUtils::LightColour(wxT("LIGHT GREY"), 7.0));
	StyleSetBackground(9,  DrawingUtils::LightColour(wxT("SIENNA"),     7.0));

	StyleSetBackground(10, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
	StyleSetForeground(10, wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHTTEXT));
}

void SvnBlameEditor::SetText(const wxString& text)
{
	// Define some colors
	int xx, yy;
	int marginWidth(0);

	int s_style(1);
	std::map<wxString, int> authorsColorsMap;

	wxFont defFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxFont font(defFont.GetPointSize(), wxFONTFAMILY_TELETYPE, wxNORMAL, wxNORMAL);

	wxArrayString lines = wxStringTokenize(text, wxT("\n"), wxTOKEN_RET_DELIMS);
	for (size_t i=0; i<lines.GetCount(); i++) {
		wxString revision;
		wxString author;
		wxString text;

		wxString line = lines.Item(i);
		line.Trim(false);

		revision = line.BeforeFirst(wxT(' '));
		revision.Trim().Trim(false);
		line = line.AfterFirst(wxT(' '));
		line.Trim(false);

		author = line.BeforeFirst(wxT(' '));
		author.Trim().Trim(false);

		int style;
		std::map<wxString, int>::iterator iter = authorsColorsMap.find(author);
		if (iter != authorsColorsMap.end()) {
			// Create new random color and use it
			style = (*iter).second;
		} else {
			style = s_style;
			s_style ++;
			if (s_style > 9)
				s_style = 1;
			authorsColorsMap[author] = style;
		}

		line = line.AfterFirst(wxT(' '));

		wxString marginText = wxString::Format(wxT("% 8s: %s"), revision.c_str(), author.c_str());
		wxWindow::GetTextExtent(marginText, &xx, &yy, NULL, NULL, &font);

		marginWidth = wxMax(marginWidth, xx);
		AppendText(line);
		MarginSetText((int)i, marginText);
		MarginSetStyle((int)i, style);

		// Keep the revision on in array
		BlameLineInfo info;
		info.revision = revision;
		info.style    = style;
		m_lineInfo.push_back(info);
	}

	SetMarginWidth(0, marginWidth);
	SetReadOnly(true);
}

void SvnBlameEditor::OnContextMenu(wxContextMenuEvent& event)
{
	wxPoint pt       = event.GetPosition();
	wxPoint clientPt = ScreenToClient(pt);

	int margin = GetMarginWidth(0); // get the margin width
	if ( clientPt.x < margin ) {

		GotoPos( PositionFromPoint(clientPt) );
		// Margin context menu
		wxMenu menu;
		menu.Append( XRCID("svn_highlight_revision"), wxT("Hightlight this revision"), wxT("Hightlight this revision"), false);
		menu.Connect(XRCID("svn_highlight_revision"), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(SvnBlameEditor::OnHighlightRevision), NULL, this);

		PopupMenu(&menu);

	} else {
		wxScintilla::OnContextMenu(event);

	}
}

void SvnBlameEditor::OnHighlightRevision(wxCommandEvent& event)
{
	int line = GetCurrentLine();
	if(line < (int)m_lineInfo.size() && line >= 0){

		BlameLineInfo info  = m_lineInfo.at(line);
		wxString  revision  = info.revision;

		// Loop over the lines and adjust the styles
		for(size_t i=0; i<m_lineInfo.size(); i++){
			BlameLineInfo lineInfo = m_lineInfo.at(i);
			if(lineInfo.revision == revision) {
				// Highlight this line
				MarginSetStyle((int)i, 10);

			} else {
				// restore old settings
				MarginSetStyle((int)i, lineInfo.style);
			}
		}
		Colourise(0, -1);
	}
}
