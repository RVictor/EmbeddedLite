//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : search_thread.cpp
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
#include <set>
#include "editor_config.h"
#include "search_thread.h"
#include "wx/event.h"
#include <wx/txtstrm.h>
#include <wx/wfstream.h>
#include <iostream>
#include <wx/tokenzr.h>
#include <wx/dir.h>
#include <wx/log.h>
#include "dirtraverser.h"
#include "macros.h"
#include "workspace.h"

const wxEventType wxEVT_SEARCH_THREAD_MATCHFOUND = wxNewEventType();
const wxEventType wxEVT_SEARCH_THREAD_SEARCHEND = wxNewEventType();
const wxEventType wxEVT_SEARCH_THREAD_SEARCHCANCELED = wxNewEventType();
const wxEventType wxEVT_SEARCH_THREAD_SEARCHSTARTED = wxNewEventType();

extern unsigned int UTF8Length(const wchar_t *uptr, unsigned int tlen);

//----------------------------------------------------------------
// SearchData
//----------------------------------------------------------------

const wxString& SearchData::GetExtensions() const
{
	return m_validExt;
}

//----------------------------------------------------------------
// SearchThread
//----------------------------------------------------------------

SearchThread::SearchThread()
		: WorkerThread()
		, m_wordChars(wxT("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_"))
		, m_reExpr(wxT(""))
{
	IndexWordChars();
}

SearchThread::~SearchThread()
{
}

void SearchThread::IndexWordChars()
{
	m_wordCharsMap.clear();
	for (size_t i=0; i<m_wordChars.Length(); i++) {
		m_wordCharsMap[m_wordChars.GetChar(i)] = true;
	}
}

void SearchThread::SetWordChars(const wxString &chars)
{
	m_wordChars = chars;
	IndexWordChars();
}

wxRegEx& SearchThread::GetRegex(const wxString &expr, bool matchCase)
{
	if (m_reExpr == expr && matchCase == m_matchCase) {
		return m_regex;
	} else {
		m_reExpr = expr;
		m_matchCase = matchCase;
#ifndef __WXMAC__
		int flags = wxRE_ADVANCED;
#else
		int flags = wxRE_DEFAULT;
#endif

		if ( !matchCase ) flags |= wxRE_ICASE;
		m_regex.Compile(m_reExpr, flags);
	}
	return m_regex;
}

void SearchThread::PerformSearch(const SearchData &data)
{
	Add( new SearchData(data) );
}

void SearchThread::ProcessRequest(ThreadRequest *req)
{
	wxStopWatch sw;
	m_summary = SearchSummary();
	DoSearchFiles(req);
	m_summary.SetElapsedTime(sw.Time());

	SearchData *sd = (SearchData*)req;
	// Send search end event
	SendEvent(wxEVT_SEARCH_THREAD_SEARCHEND, sd->GetOwner());
}

void SearchThread::GetFiles(const SearchData *data, wxArrayString &files)
{
	const wxArrayString& rootDirs = data->GetRootDirs();
	for (size_t i = 0; i < rootDirs.Count(); ++i) {
		wxArrayString someFiles;
		const wxString& rootDir = rootDirs.Item(i);
		if (rootDir == SEARCH_IN_WORKSPACE || rootDir == SEARCH_IN_CURR_FILE_PROJECT || rootDir == SEARCH_IN_PROJECT) {
			someFiles = data->GetFiles();
			// filter files which does not match the criteria
			FilterFiles(someFiles, data);
		} else if (wxFile::Exists(rootDir)) {
			// search root is actually a file...
			someFiles.push_back(rootDir);
		} else if (wxDir::Exists(rootDir)) {
			// make sure it's really a dir (not a fifo, etc.)
			DirTraverser traverser(data->GetExtensions());
			wxDir dir(rootDir);
			dir.Traverse(traverser);
			someFiles = traverser.GetFiles();
		}

		for (size_t j = 0; j < someFiles.Count(); ++j) {
			if(files.Index(someFiles.Item(j)) == wxNOT_FOUND) {
				// add only unique file names
				files.push_back(someFiles.Item(j));
			}
		}
	}
}

void SearchThread::DoSearchFiles(ThreadRequest *req)
{
	SearchData *data = static_cast<SearchData*>(req);

	// Get all files
	if ( data->GetRootDirs().IsEmpty() )
		return;

	if ( data->GetFindString().IsEmpty() )
		return;

	StopSearch(false);
	wxArrayString fileList;
	GetFiles(data, fileList);

	wxStopWatch sw;

	// Send startup message to main thread
	if ( m_notifiedWindow || data->GetOwner() ) {
		wxCommandEvent event(wxEVT_SEARCH_THREAD_SEARCHSTARTED, GetId());
		event.SetClientData(new SearchData(*data));
		//set the rquested output tab
		event.SetInt(data->UseNewTab() ? 1 : 0);
		if (data->GetOwner()) {
			::wxPostEvent(data->GetOwner(), event);
		} else {
			// since we are in if ( m_notifiedWindow || data->GetOwner() ) block...
			::wxPostEvent(m_notifiedWindow, event);
		}
	}

	for (size_t i=0; i<fileList.Count(); i++) {
		m_summary.SetNumFileScanned((int)i+1);

		// give user chance to cancel the search ...
		if ( TestStopSearch() ) {
			// Send cancel event
			SendEvent(wxEVT_SEARCH_THREAD_SEARCHCANCELED, data->GetOwner());
			StopSearch(false);
			break;
		}
		DoSearchFile(fileList.Item(i), data);
	}
}

bool SearchThread::TestStopSearch()
{
	bool stop = false;
	{
		wxCriticalSectionLocker locker(m_cs);
		stop = m_stopSearch;
	}
	return stop;
}

void SearchThread::StopSearch(bool stop)
{
	wxCriticalSectionLocker locker(m_cs);
	m_stopSearch = stop;
}

void SearchThread::DoSearchFile(const wxString &fileName, const SearchData *data)
{
	// Process single lines
	int lineNumber = 1;
	if (!wxFileName::FileExists(fileName)) {
		return;
	}

	wxFFile thefile(fileName, wxT("rb"));
	wxFileOffset size = thefile.Length();
	wxString fileData;
	fileData.Alloc(size);

	// support for other encoding
	if ( data->UseEditorFontConfig() ) {
		wxCSConv fontEncConv(EditorConfigST::Get()->GetOptions()->GetFileFontEncoding());
		thefile.ReadAll(&fileData, fontEncConv);
	} else {
//		thefile.ReadAll(&fileData, wxConvLocal);
		thefile.ReadAll(&fileData, wxCSConv(wxT("ISO-8859-1")));
	}

  if ( (0 == fileData.Length()) && (0 != size) ) //rvv,debug
  {
    wxLogMessage(wxString::Format(wxT("Searing file encoding problem: %s"), fileName.c_str()));
  }
  
  wxStringTokenizer tkz(fileData, wxT("\n"), wxTOKEN_RET_EMPTY_ALL);

	if ( data->IsRegularExpression() ) {
		// regular expression search
		while (tkz.HasMoreTokens()) {
			// Read the next line
			wxString line = tkz.NextToken();
			DoSearchLineRE(line, lineNumber, fileName, data);
			lineNumber++;
		}
	} else {
		// simple search
		while (tkz.HasMoreTokens()) {

			// Read the next line
			wxString line = tkz.NextToken();
			DoSearchLine(line, lineNumber, fileName, data);
			lineNumber++;
		}
	}

	if ( m_results.empty() == false )
		SendEvent(wxEVT_SEARCH_THREAD_MATCHFOUND, data->GetOwner());
}
void SearchThread::DoSearchLineRE(const wxString &line, const int lineNum, const wxString &fileName, const SearchData *data)
{
	wxRegEx &re = GetRegex(data->GetFindString(), data->IsMatchCase());
	size_t col = 0;
	int iCorrectedCol = 0;
	int iCorrectedLen = 0;
	wxString modLine = line;
	if ( re.IsValid() ) {
		while ( re.Matches(modLine)) {
			size_t start, len;
			re.GetMatch(&start, &len);
			col += start;

			m_summary.SetNumMatchesFound(m_summary.GetNumMatchesFound() + 1);

			// Notify our match
			// correct search Pos and Length owing to non plain ASCII multibyte characters
			iCorrectedCol = UTF8Length(line.c_str(), col);
			iCorrectedLen = UTF8Length(line.c_str(), col+len) - iCorrectedCol;
			SearchResult result;
			result.SetColumnInChars((int)col);
			result.SetColumn(iCorrectedCol);
			result.SetLineNumber(lineNum);
			result.SetPattern(line);
			result.SetFileName(fileName);
			result.SetLenInChars((int)len);
			result.SetLen(iCorrectedLen);
			result.SetFlags(data->m_flags);
			result.SetFindWhat(data->GetFindString());
			m_results.push_back(result);
			col += len;

			// adjust the line
			if (line.Length() - col <= 0)
				break;
			modLine = modLine.Right(line.Length() - col);
		}
	}
}

void SearchThread::DoSearchLine(const wxString &line, const int lineNum, const wxString &fileName, const SearchData *data)
{
	wxString findString = data->GetFindString();
	wxString modLine = line;

	if ( !data->IsMatchCase() ) {
		modLine = modLine.MakeLower();
		findString = findString.MakeLower();
	}

	int pos = 0;
	int col = 0;
	int iCorrectedCol = 0;
	int iCorrectedLen = 0;
	while ( pos != wxNOT_FOUND ) {
		pos = modLine.Find(findString);
		if (pos != wxNOT_FOUND) {
			col += pos;

			// we have a match
			if ( data->IsMatchWholeWord() ) {

				// make sure that the word before is not in the wordChars map
				if ((pos > 0) && (m_wordCharsMap.find(modLine.GetChar(pos-1)) != m_wordCharsMap.end()) ) {
					if( !AdjustLine(modLine, pos, findString) ){

						break;
					} else {
						col += (int) findString.Length();
                    	continue;
					}
				}
				// if we have more characters to the right, make sure that the first char does not match any
				// in the wordCharsMap
				if (pos + findString.Length() <= modLine.Length()) {
					wxChar nextCh = modLine.GetChar(pos+findString.Length());
					if (m_wordCharsMap.find(nextCh) != m_wordCharsMap.end()){
						if( !AdjustLine(modLine, pos, findString) ){

							break;
						} else {
							col += (int) findString.Length();
							continue;
						}
					}
				}
			}

			m_summary.SetNumMatchesFound(m_summary.GetNumMatchesFound() + 1);

			// Notify our match
			// correct search Pos and Length owing to non plain ASCII multibyte characters
			iCorrectedCol = UTF8Length(line.c_str(), col);
			iCorrectedLen = UTF8Length(findString.c_str(), findString.Length());
			SearchResult result;
			result.SetColumnInChars(col);
			result.SetColumn(iCorrectedCol);
			result.SetLineNumber(lineNum);
			result.SetPattern(line);
			result.SetFileName(fileName);
			result.SetLenInChars((int)findString.Length());
			result.SetLen(iCorrectedLen);
			result.SetFindWhat(data->GetFindString());
			result.SetFlags(data->m_flags);

			m_results.push_back(result);
			if ( !AdjustLine(modLine, pos, findString) ) {
				break;
			}
			col += (int)findString.Length();
		}
	}
}

bool SearchThread::AdjustLine(wxString &line, int &pos, wxString &findString)
{
	// adjust the current line
	if ( line.Length() - (pos + findString.Length()) >= findString.Length()) {
		line = line.Right(line.Length() - (pos + findString.Length()));
		pos += (int)findString.Length();
		return true;
	} else {
		return false;
	}
}


void SearchThread::SendEvent(wxEventType type, wxEvtHandler *owner)
{
	if ( !m_notifiedWindow && !owner)
		return;

	wxCommandEvent event(type, GetId());

	if (type == wxEVT_SEARCH_THREAD_MATCHFOUND) {
		event.SetClientData( new SearchResultList(m_results) );
		m_results.clear();
	} else if (type == wxEVT_SEARCH_THREAD_SEARCHEND) {
		// Nothing to do
		event.SetClientData( new SearchSummary(m_summary) );
	} else if (type == wxEVT_SEARCH_THREAD_SEARCHCANCELED) {
		event.SetClientData(new wxString(wxT("Search cancelled by user")));
	}

	if (owner) {
		wxPostEvent(owner, event);
	} else if (m_notifiedWindow ) {
		wxPostEvent(m_notifiedWindow, event);
	}
	wxThread::Sleep(5);
}

void SearchThread::FilterFiles(wxArrayString& files, const SearchData* data)
{
	std::map<wxString, bool> spec;
	wxString exts = data->GetExtensions();
	if (exts.Trim().Trim(false) == wxT("*.*") || exts.Trim().Trim(false) == wxT("*")) {
		spec.clear();
	} else {
		wxStringTokenizer tok(exts, wxT(";"));
		while ( tok.HasMoreTokens() ) {
			std::pair<wxString, bool> val;
			val.first = tok.GetNextToken().AfterLast(wxT('*')).c_str();
			val.first = val.first.AfterLast(wxT('.')).MakeLower().c_str();
			val.second = true;
			spec.insert( val );
		}
	}

	std::set<wxString> uniqueFileList;
	for (size_t i=0; i<files.GetCount(); i++) {
		uniqueFileList.insert(files.Item(i));
	}

	files.Clear();
	// remove duplicate files from the file array
	std::set<wxString>::iterator iter = uniqueFileList.begin();
	for (; iter != uniqueFileList.end(); iter++) {
		files.Add(*iter);
	}

	// if there is no spec, we are done here
	if (spec.empty()) {
		return;
	}

	// loop over the files and compare against the list of spec
	wxArrayString f = files;
	files.Clear();

	// filter files by extension
	for (size_t i=0; i<f.GetCount(); i++) {
		wxString ext = f.Item(i).AfterLast(wxT('.'));
		if (ext.empty()) {
			// add extensionless files (first checking for duplicates)
			files.Add(f.Item(i));
		} else if (spec.find(ext.MakeLower()) != spec.end()) {
			files.Add(f.Item(i));
		}
	}
}
