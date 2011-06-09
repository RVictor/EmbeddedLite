#include "subversion_password_db.h"
#include "wxmd5.h"
#include <wx/log.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

SubversionPasswordDb::SubversionPasswordDb()
{
	// disable logging
	wxLog::EnableLogging(false);
	wxString configDir(wxStandardPaths::Get().GetUserDataDir());
	wxMkdir(configDir);
	
	configDir << wxFileName::GetPathSeparator() << wxT("subversion");
	wxMkdir(configDir);
	
	wxLog::EnableLogging(true);
	
	configDir << wxFileName::GetPathSeparator() << wxT("passwords.ini");
	m_fileConfig = new wxFileConfig(wxEmptyString, wxEmptyString, configDir, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);
}

SubversionPasswordDb::~SubversionPasswordDb()
{
	m_fileConfig->Flush();
	delete m_fileConfig;
}

bool SubversionPasswordDb::GetLogin(const wxString& url, wxString& user, wxString& password)
{
	wxString escapedUrl(wxMD5::GetDigest(url));
	if(m_fileConfig->HasGroup(escapedUrl) == false)
		return false;
	
	m_fileConfig->Read(escapedUrl + wxT("/user"),     &user);
	m_fileConfig->Read(escapedUrl + wxT("/password"), &password);
	return true;
}

void SubversionPasswordDb::SetLogin(const wxString& url, const wxString& user, const wxString& password)
{
	wxString escapedUrl(wxMD5::GetDigest(url));
	if(m_fileConfig->HasGroup(escapedUrl)) {
		m_fileConfig->DeleteGroup(escapedUrl);
	}
	
	m_fileConfig->Write(escapedUrl + wxT("/user"),     user);
	m_fileConfig->Write(escapedUrl + wxT("/password"), password);
	m_fileConfig->Flush();
}

void SubversionPasswordDb::DeleteLogin(const wxString& url)
{
	wxString escapedUrl(wxMD5::GetDigest(url));
	if(m_fileConfig->HasGroup(escapedUrl)) {
		m_fileConfig->DeleteGroup(escapedUrl);
	}
	m_fileConfig->Flush();
}
