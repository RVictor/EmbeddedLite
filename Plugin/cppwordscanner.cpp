//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : cppwordscanner.cpp
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
#include <wx/ffile.h>
#include "globals.h"
#include <wx/tokenzr.h>
#include "cppwordscanner.h"
#include "stringaccessor.h"
//#include "tokendb.h"

enum {
	STATE_NORMAL = 0,
	STATE_C_COMMENT,
	STATE_CPP_COMMENT,
	STATE_DQ_STRING,
	STATE_SINGLE_STRING,
	STATE_PRE_PROCESSING
};

CppWordScanner::CppWordScanner(const wxString &file_name)
		: m_filename(file_name)
{
	wxString key_words =
	    wxT("auto break case char const continue default define defined do double elif else endif enum error extern float \
for  goto if ifdef ifndef include int line long pragma register return short signed sizeof static struct switch \
typedef undef union unsigned void volatile while class namespace delete friend inline new operator overload \
protected private public this virtual template typename dynamic_cast static_cast const_cast reinterpret_cast \
using throw catch size_t");

	//add this items into map
	m_arr = wxStringTokenize(key_words, wxT(" "));
	m_arr.Sort();
#ifdef __WXGTK__
	wxFFile thefile(file_name, wxT("rb"));
	wxFileOffset size = thefile.Length();
	wxString fileData;
	fileData.Alloc(size);
	thefile.ReadAll( &m_text );
#else
	ReadFileWithConversion(file_name, m_text);
#endif
}

CppWordScanner::~CppWordScanner()
{
//	m_db = NULL;
}

void CppWordScanner::FindAll(CppTokensMap &l)
{
	doFind(wxEmptyString, l);

//	if (m_db) {
//		m_db->BeginTransaction();
//		m_db->DeleteByFile(m_filename);
//		CppTokenList::iterator iter = l.begin();
//		for (; iter != l.end(); iter++) {
//			m_db->Store( (*iter) );
//		}
//		m_db->Commit();
//	}
}

void CppWordScanner::Match(const wxString& word, CppTokensMap& l)
{
	doFind(word, l);
//	if (m_db) {
//		m_db->BeginTransaction();
//		m_db->DeleteByFile(m_filename);
//		CppTokenList::iterator iter = l.begin();
//		for (; iter != l.end(); iter++) {
//			m_db->Store( (*iter) );
//		}
//		m_db->Commit();
//	}
}

void CppWordScanner::doFind(const wxString& filter, CppTokensMap& l)
{
	int state(STATE_NORMAL);
//	bool sol(true);

	StringAccessor accessor(m_text);
	CppToken token;

	for (size_t i=0; i<m_text.size(); i++) {
		char ch = accessor.safeAt(i);

		switch (state) {

		case STATE_NORMAL:
			if (accessor.match("#", i)) {

				if (	i == 0 || 						// satrt of document
				        accessor.match("\n", i-1)) { 	// we are at start of line
					state = STATE_PRE_PROCESSING;
				}
			} else if (accessor.match("//", i)) {

				// C++ comment, advance i
				state = STATE_CPP_COMMENT;
				i++;

			} else if (accessor.match("/*", i)) {

				// C comment
				state = STATE_C_COMMENT;
				i++;

			} else if (accessor.match("'", i)) {

				// single quoted string
				state = STATE_SINGLE_STRING;

			} else if (accessor.match("\"", i)) {

				// dbouble quoted string
				state = STATE_DQ_STRING;

			} else if ( accessor.isWordChar(ch) ) {

				// is valid C++ word?
				token.append( ch );
				if (token.getOffset() == wxString::npos) {
					token.setOffset( i );
				}
			} else {

				if (token.getName().empty() == false) {

					if ((int)token.getName().GetChar(0) >= 48 && (int)token.getName().GetChar(0) <= 57) {
						token.reset();
					} else {
						//dont add C++ key words
						if (m_arr.Index(token.getName()) == wxNOT_FOUND) {

							// Ok, we are not a number!
							// filter out non matching words
							if (filter.empty() || filter == token.getName()) {
								token.setFilename(m_filename);
								l.addToken(token);
							}
						}
						token.reset();
					}
				}
			}

			break;
		case STATE_PRE_PROCESSING:
			//skip pre processor lines
			if ( accessor.match("\n", i) && !accessor.match("\\", i-1) ) {
				// no wrap
				state = STATE_NORMAL;
			}
			break;
		case STATE_C_COMMENT:
			if ( accessor.match("*/", i)) {
				state = STATE_NORMAL;
				i++;
			}
			break;
		case STATE_CPP_COMMENT:
			if ( accessor.match("\n", i)) {
				state = STATE_NORMAL;
			}
			break;
		case STATE_DQ_STRING:
			if (accessor.match("\\\"", i)) {
				//escaped string
				i++;
			} else if(accessor.match("\\", i)){
				i++;
			} else if (accessor.match("\"", i)) {
				state = STATE_NORMAL;
			}
			break;
		case STATE_SINGLE_STRING:
			if (accessor.match("\\'", i)) {
				//escaped single string
				i++;
			} else if (accessor.match("'", i)) {
				state = STATE_NORMAL;
			}
			break;
		}
	}
}

//void CppWordScanner::SetDatabase(TokenDb* db)
//{
//	m_db = db;
//}
//
