// Scintilla source code edit control
/** @file LexOthers.cxx
 ** Lexers for batch files, diff results, properties files, make files and error lists.
 ** Also lexer for LaTeX documents.
 **/
// Copyright 1998-2001 by Eran Ifrah <eran.ifrah@gmail.com>
// The License.txt file describes the conditions under which this software may be distributed.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>
#include <wx/string.h>

#include "Platform.h"

#include "PropSet.h"
#include "Accessor.h"
#include "KeyWords.h"
#include "Scintilla.h"
#include "SciLexer.h"
#include "StyleContext.h"

#ifdef SCI_NAMESPACE
using namespace Scintilla;
#endif

typedef int (*GCC_COLOUR_FUNC_PTR)(int, const char*, size_t&, size_t&);

static GCC_COLOUR_FUNC_PTR s_gccColourFunc = NULL;

void SetGccColourFunction(GCC_COLOUR_FUNC_PTR func)
{
	s_gccColourFunc = func;
}

static inline bool AtEOL(Accessor &styler, unsigned int i)
{
	return (styler[i] == '\n') ||
	       ((styler[i] == '\r') && (styler.SafeGetCharAt(i + 1) != '\n'));
}

static void ColouriseGccDoc(
    unsigned int pos,
    int length,
    int /*initStyle*/,
    WordList * /*keywordlists*/[],  
    Accessor &styler)
{
    std::string line;
    line.reserve(2048);

	styler.StartAt(pos);
	styler.StartSegment(pos);
	
	for (; length > 0; pos++, length--) {
        line += styler[pos];
		if (AtEOL(styler, pos)) {
			// End of line met, colourise it
            int style = SCLEX_GCC_OUTPUT;
            if (s_gccColourFunc) {
                size_t start(0);
                size_t len(0);
                int startLine = pos-line.size()+1;
                style = s_gccColourFunc(startLine, line.c_str(), start, len);
                if(len != 0) {
                    styler.ColourTo(startLine + start - 1, style);
                    styler.ColourTo(startLine + start + len - 1, SCLEX_GCC_FILE_LINK);
                }
            }
            styler.ColourTo(pos, style);
            line.clear();
		}
	}
}

static void FoldGccDoc(unsigned int pos, int length, int,
                       WordList*[], Accessor &styler)
{
	int curLine = styler.GetLine(pos);
	int prevLevel = curLine > 0 ? styler.LevelAt(curLine-1) : SC_FOLDLEVELBASE;

    unsigned int end = pos+length;
    pos = styler.LineStart(curLine);

	do {
        int nextLevel;
        switch (styler.StyleAt(pos)) {
            case SCLEX_GCC_BUILDING:
                nextLevel = SC_FOLDLEVELBASE | SC_FOLDLEVELHEADERFLAG;
                break;
            default:
                nextLevel = prevLevel & SC_FOLDLEVELHEADERFLAG ? (prevLevel & SC_FOLDLEVELNUMBERMASK) + 1 : prevLevel;
                break;
        }
		if ((nextLevel & SC_FOLDLEVELHEADERFLAG) && nextLevel == prevLevel) {
			styler.SetLevel(curLine-1, prevLevel & ~SC_FOLDLEVELHEADERFLAG);
        }
		styler.SetLevel(curLine, nextLevel);

        curLine++;
		prevLevel = nextLevel;
		pos = styler.LineStart(curLine);
	} while (pos < end);
}

static const char * const gccWordListDesc[] = {
	"Internal Commands",
	"External Commands",
	0
};

static const char * const emptyWordListDesc[] = {
	0
};

LexerModule lmGcc(SCLEX_GCC, ColouriseGccDoc, "gcc", FoldGccDoc, gccWordListDesc);
