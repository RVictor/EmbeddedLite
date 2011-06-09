//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah                            
// file name            : webupdatethread.cpp              
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

#include <wx/url.h>
#include "precompiled_header.h"
#include <wx/tokenzr.h>
#include "webupdatethread.h"
#include "elconfig.h"

extern const wxChar *SvnRevision;

const wxEventType wxEVT_CMD_NEW_VERSION_AVAILABLE = wxNewEventType();
const wxEventType wxEVT_CMD_VERSION_UPTODATE = wxNewEventType();

static const size_t DLBUFSIZE = 4096;

WebUpdateJob::WebUpdateJob(wxEvtHandler *parent, bool userRequest)
		: Job(parent)
		, m_userRequest(userRequest)
{
}

WebUpdateJob::~WebUpdateJob()
{
}

void WebUpdateJob::Process(wxThread* thread)
{
//	CURL *curl(NULL);
//	CURLcode res;
//	curl_global_init(CURL_GLOBAL_DEFAULT);
//	curl = curl_easy_init();
//	if (curl) {
//		// set the URL to the packages list
//		curl_easy_setopt(curl, CURLOPT_URL, "http://codelite.org/packages.txt");
//		// set callback for the write operation
//		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteData);
//		// pass this object to the callback
//		curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
//		// switch off verbose mode
//		curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
//		// fetch!
//		res = curl_easy_perform(curl);
//
//		if (res == CURLE_OK) {
//			// compare the version
//			ParseFile();
//		}
//	}
//	curl_global_cleanup();

//	wxURL url(wxT("http://codelite.org/packages_test.txt"));
	wxURL url(wxT("http://codelite.org/packages.txt"));
	if (url.GetError() == wxURL_NOERR) {
		
		wxInputStream *in_stream = url.GetInputStream();
		if (!in_stream) {
			return;
		}

		unsigned char buffer[DLBUFSIZE+1];
		do {
			
			in_stream->Read(buffer, DLBUFSIZE);
			size_t bytes_read = in_stream->LastRead();
			if (bytes_read > 0) {
				
				buffer[bytes_read] = 0;
				wxString buffRead((const char*)buffer, wxConvUTF8);
				m_dataRead.Append(buffRead);
			}
			
		} while ( !in_stream->Eof() );
		
		delete in_stream;
		
		ParseFile();
	}
}

size_t WebUpdateJob::WriteData(void* buffer, size_t size, size_t nmemb, void* obj)
{
	WebUpdateJob *job = reinterpret_cast<WebUpdateJob*>(obj);
	if (job) {
		char *data = new char[size*nmemb+1];
		memcpy(data, buffer, size*nmemb);
		data[size*nmemb] = 0;

		job->m_dataRead.Append(_U(data));
		delete [] data;
		return size * nmemb;
	}
	return static_cast<size_t>(-1);
}

void WebUpdateJob::ParseFile()
{
	wxString packageName(wxT("MSW"));
#if defined(__WXGTK__)
	packageName = wxT("GTK");
#elif defined(__WXMAC__)
	packageName = wxT("MAC");
#endif
	
	// diffrentiate between the 64bit and the 32bit packages
#ifdef ON_64_BIT
	packageName << wxT("_64");
#endif

	wxArrayString lines = wxStringTokenize(m_dataRead, wxT("\n"));
	for (size_t i=0; i<lines.GetCount(); i++) {
		wxString line = lines.Item(i);
		line = line.Trim().Trim(false);
		if (line.StartsWith(wxT("#"))) {
			//comment line
			continue; 
		}

		// parse the line
		wxArrayString tokens = wxStringTokenize(line, wxT("|"));
		if (tokens.GetCount() > 3) {
			// find the entry with our package name
			if (tokens.Item(0).Trim().Trim(false) == packageName) {
				wxString url = tokens.Item(2).Trim().Trim(false);
				wxString rev = tokens.Item(1).Trim().Trim(false);
				wxString releaseNotesUrl = tokens.Item(3).Trim().Trim(false);
				
				long currev;
				long webrev(0);

				// convert strings to long
				currev = APP_VER_BUILD;

				wxString sUrlRev(rev);
				sUrlRev.ToLong(&webrev);

				if ( webrev > currev ) {
					// notify the user that a new version is available
					wxCommandEvent e(wxEVT_CMD_NEW_VERSION_AVAILABLE);
					e.SetClientData(new WebUpdateJobData(url.c_str(), releaseNotesUrl.c_str(), currev, webrev, false, m_userRequest));
					wxPostEvent(m_parent, e);
				} else {
					// version is up to date, notify the main thread about it
					wxCommandEvent e(wxEVT_CMD_VERSION_UPTODATE);
					e.SetClientData(new WebUpdateJobData(url.c_str(), releaseNotesUrl.c_str(), currev, webrev, true, m_userRequest));
					wxPostEvent(m_parent, e);
				}
				break;
			}
		}
	}
}
