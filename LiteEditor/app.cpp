/**
  \file app.cpp

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#include <wx/socket.h>
#include "fileextmanager.h"
#include <wx/splash.h>
#include "evnvarlist.h"
#include "environmentconfig.h"
#include "conffilelocator.h"
#include "app.h"
#include <wx/snglinst.h>
#include <wx/image.h>
#include <wx/filefn.h>
#include "dirsaver.h"

#include "xmlutils.h"
#include "editor_config.h"
#include <wx/xrc/xmlres.h>
#include <wx/sysopt.h>
#include "manager.h"
#include "macros.h"
#include "wx/txtstrm.h"
#include "wx/wfstream.h"
#include "stack_walker.h"
#include <wx/cmdline.h>
#include "procutils.h"
#include "globals.h"
#include "wx/tokenzr.h"
#include "wx/dir.h"
#include <wx/stdpaths.h>
#include "frame.h"
#include "elconfig.h"

#define __PERFORMANCE
#include "performance.h"

#if defined(__WXMAC__)||defined(__WXGTK__)
#include <signal.h> // sigprocmask
#endif

#ifdef __WXMSW__
#include <wx/msw/registry.h> //registry keys
#endif

#ifdef __WXMAC__
#include <mach-o/dyld.h>

//On Mac we determine the base path using system call
//_NSGetExecutablePath(path, &path_len);
wxString MacGetBasePath()
{
	char path[257];
	uint32_t path_len = 256;
	_NSGetExecutablePath(path, &path_len);

	//path now contains
	//EmbeddedLite.app/Contents/MacOS/
	wxFileName fname(wxString(path, wxConvUTF8));

	//remove he MacOS part of the exe path
	wxString file_name = fname.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR);
	wxString rest;
	file_name.EndsWith(wxT("MacOS/"), &rest);
	rest.Append(wxT("SharedSupport/"));

	return rest;
}
#endif

/**
  Returns application version as a string.
  \return application version as a string.
*/
wxString
AppGetVersion(bool bIncludeBuild)
{
  if (bIncludeBuild)
  {
    return wxString::Format(wxT("v%d.%d.%d.%d"), APP_VER_HI, APP_VER_LO, APP_VER_SUB_LO, APP_VER_BUILD);
  }
  else
  {
    return wxString::Format(wxT("v%d.%d.%d"), APP_VER_HI, APP_VER_LO, APP_VER_SUB_LO);
  }
}

//-------------------------------------------------
// helper method to draw the revision + version
// on our splash screen
//-------------------------------------------------
static wxBitmap clDrawSplashBitmap(const wxBitmap& bitmap, const wxString &mainTitle)
{
	wxBitmap bmp ( bitmap.GetWidth(), bitmap.GetHeight()  );
    wxMemoryDC dcMem;
	
    dcMem.SelectObject( bmp );
	dcMem.DrawBitmap  ( bitmap, 0, 0, true);

	//write the main title & subtitle
	wxCoord w, h;
	wxFont font = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	wxFont smallfont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	font.SetPointSize(12);
	smallfont.SetPointSize(10);
	dcMem.SetFont(font);
	dcMem.GetMultiLineTextExtent(mainTitle, &w, &h);
	wxCoord bmpW = bitmap.GetWidth();

	//draw shadow
	dcMem.SetTextForeground(wxT("WHITE"));
	
	wxCoord textX = (bmpW - w)/2;
	dcMem.DrawText(mainTitle, textX, 11);
	dcMem.SelectObject(wxNullBitmap);
	return bmp;
}

//-----------------------------------------------------
// Splashscreen
//-----------------------------------------------------
class clSplashScreen : public wxSplashScreen
{
public:
	clSplashScreen(const wxBitmap& bmp) : wxSplashScreen(bmp, wxSPLASH_CENTRE_ON_SCREEN|wxSPLASH_TIMEOUT, 5000, NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE| wxFRAME_NO_TASKBAR| wxSTAY_ON_TOP)
	{
	}
};

#if wxVERSION_NUMBER < 2900
static const wxCmdLineEntryDesc cmdLineDesc[] = {
	{wxCMD_LINE_SWITCH, wxT("v"), wxT("version"), wxT("Print current version"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_SWITCH, wxT("h"), wxT("help"), wxT("Print usage"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_SWITCH, wxT("p"), wxT("no-plugins"), wxT("Start EmbeddedLite without any plugins"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_OPTION, wxT("l"), wxT("line"), wxT("Open the file at a given line number"), wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_OPTION, wxT("b"), wxT("basedir"),  wxT("Use this path as EmbeddedLite installation path"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_PARAM,  NULL, NULL, wxT("Input file"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_MULTIPLE|wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_NONE }
};
#else
static const wxCmdLineEntryDesc cmdLineDesc[] = {
	{wxCMD_LINE_SWITCH, "v", "version", "Print current version", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_SWITCH, "h", "help", "Print usage", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_SWITCH, "p", "no-plugins", "Start codelite without any plugins", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_OPTION, "l", "line", "Open the file at a given line number", wxCMD_LINE_VAL_NUMBER, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_OPTION, "b", "basedir",  "The base directory of EmbeddedLite", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_PARAM,  NULL, NULL, "Input file", wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_MULTIPLE|wxCMD_LINE_PARAM_OPTIONAL },
	{wxCMD_LINE_NONE }
};
#endif

static void massCopy(const wxString &sourceDir, const wxString &spec, const wxString &destDir)
{
	wxArrayString files;
	wxDir::GetAllFiles(sourceDir, &files, spec, wxDIR_FILES);
	for ( size_t i=0; i<files.GetCount(); i++ ) {
		wxFileName fn(files.Item(i));
		wxCopyFile(files.Item(i), destDir + wxT("/") + fn.GetFullName());
	}
}

IMPLEMENT_APP(App)

extern void InitXmlResource();
App::App(void)
		: m_pMainFrame(NULL)
		, m_singleInstance(NULL)
		, m_loadPlugins(true)
#ifdef __WXMSW__
		, m_handler(NULL)
#endif
{
}

App::~App(void)
{
	wxImage::CleanUpHandlers();
#ifdef __WXMSW__
	if (m_handler) {
		FreeLibrary(m_handler);
		m_handler = NULL;
	}
#endif
	if ( m_singleInstance ) {
		delete m_singleInstance;
	}
	wxAppBase::ExitMainLoop();
}

bool App::OnInit()
{
#if defined(__WXGTK__) || defined(__WXMAC__)
//	block signal pipe
	sigset_t mask_set;
	sigemptyset( &mask_set );
	sigaddset(&mask_set, SIGPIPE);
	sigprocmask(SIG_SETMASK, &mask_set, NULL);
#endif
  wxString strInstallPath;

	wxSocketBase::Initialize();

#if wxUSE_STACKWALKER
	//trun on fatal exceptions handler
	wxHandleFatalExceptions(true);
#endif

#ifdef __WXMSW__
	// as described in http://jrfonseca.dyndns.org/projects/gnu-win32/software/drmingw/
	// load the exception handler dll so we will get Dr MinGW at runtime
	m_handler = LoadLibrary(wxT("exchndl.dll"));
#endif

	// Init resources and add the PNG handler
	wxSystemOptions::SetOption(_T("msw.remap"), 0);
	wxXmlResource::Get()->InitAllHandlers();
	wxImage::AddHandler( new wxPNGHandler );
	wxImage::AddHandler( new wxCURHandler );
	wxImage::AddHandler( new wxICOHandler );
	wxImage::AddHandler( new wxXPMHandler );
	wxImage::AddHandler( new wxGIFHandler );
	InitXmlResource();
	//wxLog::EnableLogging(false);
	wxString strUserDataDir(wxEmptyString);

	//parse command line
	wxCmdLineParser parser;
	parser.SetDesc(cmdLineDesc);
	parser.SetCmdLine(wxAppBase::argc, wxAppBase::argv);
	if (parser.Parse() != 0) {
		return false;
	}

	if(parser.Found(wxT("h"))){
		// print usage
		parser.Usage();
		return false;
	}
	
	if(parser.Found(wxT("p"))){
		// Load codelite without plugins
		SetLoadPlugins(false);
	}

	wxString newBaseDir(wxEmptyString);
	if (parser.Found(wxT("b"), &newBaseDir)) {
		strUserDataDir = newBaseDir;
	}

#if defined (__WXGTK__)
	if (strUserDataDir.IsEmpty()) 
  {
    SetAppName(wxT("embeddedlite"));
    strUserDataDir = wxStandardPaths::Get().GetUserDataDir(); // ~/Library/Application Support/embeddedlite or ~/.embeddedlite

    ManagerST::Get()->SetAppDataDir(strUserDataDir);

//    wxFileName appFn(wxAppBase::argv[0]);
//    appFn.Normalize();
    
//dnw    strInstallPath = static_cast<wxStandardPaths&>(wxStandardPaths::Get()).GetInstallPrefix() + wxT("/share/embeddedlite");
//dnw    ManagerST::Get()->SetAppShareDir(strInstallPath);
    wxFileName appFn(wxAppBase::argv[0]);
    appFn.Normalize();
    ManagerST::Get()->SetAppShareDir(appFn.GetPath());
    // Assume the install directory is the same as path to executable
    ManagerST::Get()->SetAppInstallDirectory(appFn.GetPath());
    strInstallPath = appFn.GetPath();     
	} 
  else 
  {
		wxFileName StartupDir(strUserDataDir);
		StartupDir.MakeAbsolute();
    StartupDir.Normalize();
    
    strUserDataDir = StartupDir.GetPath();
    strInstallPath = strUserDataDir;
    ManagerST::Get()->SetAppDataDir(strUserDataDir);
    ManagerST::Get()->SetAppShareDir(strUserDataDir);
    ManagerST::Get()->SetAppInstallDirectory(strUserDataDir);
  }
  
  ManagerST::Get()->SetPluginsDir(ManagerST::Get()->GetAppShareDir() + wxFileName::GetPathSeparator() + wxT("plugins"));
#elif defined (__WXMAC__)
	SetAppName(wxT("embeddedlite"));
	strUserDataDir = wxStandardPaths::Get().GetUserDataDir();
	strInstallPath = MacGetBasePath();
	ManagerST::Get()->SetAppDataDir(strInstallPath);
#else //__WXMSW__
	if (strUserDataDir.IsEmpty())  //did we got a basedir from user?
  { 
    SetAppName(wxT("embeddedlite"));
    strUserDataDir = wxStandardPaths::Get().GetUserDataDir(); // ~/Library/Application Support/embeddedlite or ~/.embeddedlite
   
    ManagerST::Get()->SetAppDataDir(strUserDataDir);

    wxFileName appFn(wxAppBase::argv[0]);
    appFn.Normalize();
    ManagerST::Get()->SetAppShareDir(appFn.GetPath());
    // Assume the install directory is the same as path to executable
    ManagerST::Get()->SetAppInstallDirectory(appFn.GetPath());
    strInstallPath = appFn.GetPath();     
	}
  else
  {
    wxFileName StartupDir(strUserDataDir);
    StartupDir.MakeAbsolute();
    StartupDir.Normalize();
    
    strUserDataDir = StartupDir.GetPath();
    strInstallPath = strUserDataDir;
    ManagerST::Get()->SetAppDataDir(strUserDataDir);
    ManagerST::Get()->SetAppShareDir(strUserDataDir);
    ManagerST::Get()->SetAppInstallDirectory(strUserDataDir);
  }
  
  ManagerST::Get()->SetPluginsDir(ManagerST::Get()->GetAppShareDir() + wxFileName::GetPathSeparator() + wxT("plugins"));
#endif

  ManagerST::Get()->SetConfigDir(strUserDataDir + wxFileName::GetPathSeparator() + wxT("config"));
  ManagerST::Get()->SetLexersDir(strUserDataDir + wxFileName::GetPathSeparator() + wxT("lexers"));

#if 0
  do
  {
    wxString strMsg;
    
    strMsg = wxT("strInstallPath: ") + ManagerST::Get()->GetAppInstallDirectory() + wxT("\n")
      + strMsg + wxT("GetAppShareDir: ") + ManagerST::Get()->GetAppShareDir() + wxT("\n")
      + strMsg + wxT("GetAppDataDir: ") + ManagerST::Get()->GetAppDataDir() + wxT("\n")
      + strMsg + wxT("GetConfigDir: ") + ManagerST::Get()->GetConfigDir() + wxT("\n")
      + strMsg + wxT("GetLexersDir: ") + ManagerST::Get()->GetLexersDir() + wxT("\n")
      + strMsg + wxT("GetPluginsDir: ") + ManagerST::Get()->GetPluginsDir() + wxT("\n");
    wxMessageBox(strMsg, wxT("Working configuration"));
  } while (0);
#endif
  //Create the directory structure
  wxLogNull noLog;
  wxMkdir(strUserDataDir);
  wxMkdir(ManagerST::Get()->GetLexersDir());
  wxString strSubDir = ManagerST::Get()->GetLexersDir() + wxT("/Default");
  if (!::wxDirExists(strSubDir))
  {
    wxMkdir(strSubDir);
    CopyDir(strInstallPath + wxT("/lexers/BlackTheme"), strSubDir);
  }
  strSubDir = ManagerST::Get()->GetLexersDir() + wxT("/BlackTheme");
  if (!::wxDirExists(strSubDir))
  {
    wxMkdir(strSubDir);
    CopyDir(strInstallPath + wxT("/lexers/Default"), strSubDir);
  }

  if (!::wxDirExists(ManagerST::Get()->GetConfigDir()))
  {
    wxMkdir(ManagerST::Get()->GetConfigDir());
    wxCopyFile(strInstallPath + wxT("/config/embeddedlite.xml.default"), ManagerST::Get()->GetConfigDir() + wxT("/embeddedlite.xml.default"));
    wxCopyFile(strInstallPath + wxT("/config/build_settings.xml.default"), ManagerST::Get()->GetConfigDir() + wxT("/build_settings.xml.default"));
    wxCopyFile(strInstallPath + wxT("/config/embeddedlite.layout.default"), ManagerST::Get()->GetConfigDir() + wxT("/embeddedlite.layout.default"));
    wxCopyFile(strInstallPath + wxT("/config/debuggers.xml.default"), ManagerST::Get()->GetConfigDir() + wxT("/debuggers.xml.default"));
    wxCopyFile(strInstallPath + wxT("/config/accelerators.conf.default"), ManagerST::Get()->GetConfigDir() + wxT("/accelerators.conf.default"));
    wxCopyFile(strInstallPath + wxT("/config/plugins.xml.default"), ManagerST::Get()->GetConfigDir() + wxT("/plugins.xml.default"));
  }
/*
#if defined (__WXGTK__)
	if (strUserDataDir.IsEmpty()) {
		SetAppName(wxT("embeddedlite"));
		strUserDataDir = wxStandardPaths::Get().GetUserDataDir(); // ~/Library/Application Support/embeddedlite or ~/.embeddedlite

		//Create the directory structure
		wxLogNull noLog;
		wxMkdir(strUserDataDir);
		wxMkdir(strUserDataDir + wxT("/lexers/"));
		wxMkdir(strUserDataDir + wxT("/lexers/Default"));
		wxMkdir(strUserDataDir + wxT("/lexers/BlackTheme"));
		wxMkdir(strUserDataDir + wxT("/rc/"));
		wxMkdir(strUserDataDir + wxT("/images/"));
		wxMkdir(strUserDataDir + wxT("/templates/"));
		wxMkdir(strUserDataDir + wxT("/config/"));

		//copy the settings from the global location if needed
		wxString strInstallPath( INSTALL_DIR, wxConvUTF8 );
		if ( ! CopySettings(strUserDataDir, strInstallPath ) ) return false;
		ManagerST::Get()->SetAppDataDir( strInstallPath );

	} else {
		wxFileName fn(strUserDataDir);
		fn.MakeAbsolute();
		ManagerST::Get()->SetAppDataDir( fn.GetFullPath() );
	}
#elif defined (__WXMAC__)
	SetAppName(wxT("embeddedlite"));
	strUserDataDir = wxStandardPaths::Get().GetUserDataDir();
	
	{
		wxLogNull noLog;
		
		//Create the directory structure
		wxMkdir(strUserDataDir);
		wxMkdir(strUserDataDir + wxT("/lexers/"));
		wxMkdir(strUserDataDir + wxT("/lexers/Default"));
		wxMkdir(strUserDataDir + wxT("/lexers/BlackTheme"));
		wxMkdir(strUserDataDir + wxT("/rc/"));
		wxMkdir(strUserDataDir + wxT("/images/"));
		wxMkdir(strUserDataDir + wxT("/templates/"));
		wxMkdir(strUserDataDir + wxT("/config/"));	
	}

	wxString strInstallPath( MacGetBasePath() );
	ManagerST::Get()->SetAppDataDir( strInstallPath );
	//copy the settings from the global location if needed
	CopySettings(strUserDataDir, strInstallPath);

#else //__WXMSW__
	if (strUserDataDir.IsEmpty()) { //did we got a basedir from user?
		strUserDataDir = ::wxGetCwd();
	}
	wxFileName fnHomdDir(strUserDataDir + wxT("/"));

	// try to locate the menu/rc.xrc file
	wxFileName fn(strUserDataDir + wxT("/rc"), wxT("menu.xrc"));
	if(!fn.FileExists()){
		// we got wrong home directory
		wxFileName appFn( wxAppBase::argv[0] );
		strUserDataDir = appFn.GetPath();
	}

	if(fnHomdDir.IsRelative()){
		fnHomdDir.MakeAbsolute();
		strUserDataDir = fnHomdDir.GetPath();
	}

	ManagerST::Get()->SetAppDataDir( strUserDataDir );
#endif
*/

	// Update codelite revision and Version
	EditorConfigST::Get()->Init(APP_VER_BUILD, AppGetVersion(false));

	wxString strStartupDir = wxGetCwd();
	::wxSetWorkingDirectory(ManagerST::Get()->GetAppShareDir());    //rvv,todo: not a GetAppShareDir() ?
	// Load all of the XRC files that will be used. You can put everything
	// into one giant XRC file if you wanted, but then they become more
	// diffcult to manage, and harder to reuse in later projects.
	// The menubar
	if (!wxXmlResource::Get()->Load(ManagerST::Get()->GetAppShareDir() + wxT("/rc/menu.xrc")))
		return false;

	// keep the startup directory
	ManagerST::Get()->SetStarupDirectory(strStartupDir);

	// set the performance output file name
	PERF_OUTPUT(wxString::Format(wxT("%s/embeddedlite.perf"), wxGetCwd().c_str()).mb_str(wxConvUTF8));

	// Initialize the configuration file locater
	ConfFileLocator::Instance()->Initialize(ManagerST::Get()->GetAppInstallDirectory(), ManagerST::Get()->GetStarupDirectory());

	Manager *mgr = ManagerST::Get();

	// set the CTAGS_REPLACEMENT environment variable
	wxSetEnv(wxT("CTAGS_REPLACEMENTS"), ManagerST::Get()->GetStarupDirectory() + wxT("/ctags.replacements"));

	//show splashscreen here
	long style = wxSIMPLE_BORDER;
#if defined (__WXMSW__) || defined (__WXGTK__)
	style |= wxFRAME_NO_TASKBAR;
#endif

	//read the last frame size from the configuration file
	// Initialise editor configuration files
	EditorConfig *cfg = EditorConfigST::Get();
	if ( !cfg->Load(ManagerST::Get()->GetConfigDir()) ) 
  {
		wxLogMessage(wxT("Failed to load configuration file: ") + ManagerST::Get()->GetConfigDir() + wxT("/embeddedlite.xml"), wxT("EmbeddedLite"), wxICON_ERROR | wxOK);
		return false;
	}

	// check for single instance
	if ( !CheckSingularity(parser, strStartupDir) ) {
		return false;
	}

#ifdef __WXMSW__
	// Update PATH environment variable with the install directory and
	// MinGW default installation (if exists)
	wxString pathEnv;
	if(wxGetEnv(wxT("PATH"), &pathEnv) == false)
  {
		wxLogMessage(_("WARNING: Failed to load environment variable PATH!"));
	}
  else
  {
		pathEnv << wxT(";") << ManagerST::Get()->GetAppInstallDirectory() << wxT(";");

		// read the installation path of MinGW & WX
		wxRegKey rk(wxT("HKEY_CURRENT_USER\\Software\\EmbeddedLite"));
		if(rk.Exists()) {
/*rvv,nn
			m_parserPaths.Clear();
			wxString strWx, strMingw;
			if(rk.HasValue(wxT("wx"))){
				rk.QueryValue(wxT("wx"), strWx);
			}

			if(rk.HasValue(wxT("mingw"))){
				rk.QueryValue(wxT("mingw"), strMingw);
			}

			EvnVarList vars;
			EnvironmentConfig::Instance()->Load(ManagerST::Get()->GetConfigDir());
			EnvironmentConfig::Instance()->ReadObject(wxT("Variables"), &vars);

			if(strWx.IsEmpty() == false) {
				// we have WX installed on this machine, set the path of WXWIN & WXCFG to point to it
				std::map<wxString, wxString> envs = vars.GetVariables(wxT("Default"));
				
				if(envs.find(wxT("WXWIN")) == envs.end()) {
					vars.AddVariable(wxT("Default"), wxT("WXWIN"), strWx);
					vars.AddVariable(wxT("Default"), wxT("PATH"),  wxT("$(WXWIN)\\lib\\gcc_dll;$(PATH)"));
				}
				
				if(envs.find(wxT("WXCFG")) == envs.end())
					vars.AddVariable(wxT("Default"), wxT("WXCFG"), wxT("gcc_dll\\mswu"));

				EnvironmentConfig::Instance()->WriteObject(wxT("Variables"), &vars);
				wxSetEnv(wxT("WX_INCL_HOME"), strWx + wxT("\\include"));
			}

			if(strMingw.IsEmpty() == false) {
				// Add the installation include paths
				pathEnv << wxT(";") << strMingw << wxT("\\bin");
				wxSetEnv(wxT("MINGW_INCL_HOME"), strMingw);
			}
*/    
		}

		if(wxSetEnv(wxT("PATH"), pathEnv) == false)
    {
			wxLogMessage(_("WARNING: Failed to update environment variable PATH"));
		}
	}
#endif

	GeneralInfo inf;
	cfg->ReadObject(wxT("GeneralInfo"), &inf);

	bool bShowSplash = false;
  bShowSplash = inf.GetFlags() & CL_SHOW_SPLASH ? true : false;

	m_splash = NULL;
	if (bShowSplash) {
		wxBitmap bitmap;
		wxString splashName(mgr->GetAppShareDir() + wxT("/images/splashscreen.png"));
		if (bitmap.LoadFile(splashName, wxBITMAP_TYPE_PNG))
    {
			wxString mainTitle = AppGetVersion();
			wxBitmap splash = clDrawSplashBitmap(bitmap, mainTitle);
			m_splash = new clSplashScreen(splash);
		}
	}
	
	// Create the main application window (a dialog in this case)
	// NOTE: Vertical dimension comprises the caption bar.
	//       Horizontal dimension has to take into account the thin
	//       hilighting border around the dialog (2 points in
	//       Win 95).
	Frame::Initialize( parser.GetParamCount() == 0 );
	m_pMainFrame = Frame::Get();

	// update the accelerators table
	ManagerST::Get()->UpdateMenuAccelerators();
	m_pMainFrame->Show(TRUE);
	SetTopWindow(m_pMainFrame);

	long lineNumber(0);
	parser.Found(wxT("l"), &lineNumber);
	if(lineNumber > 0){
		lineNumber--;
	}else{
		lineNumber = 0;
	}

	for (size_t i=0; i< parser.GetParamCount(); i++) {
		wxString argument = parser.GetParam(i);

		//convert to full path and open it
		wxFileName fn(argument);
		fn.MakeAbsolute(strStartupDir);

		if (fn.GetExt() == EL_WORKSPACE_EXT) {
			ManagerST::Get()->OpenWorkspace(fn.GetFullPath());
		} else {
			Frame::Get()->GetMainBook()->OpenFile(fn.GetFullPath(), wxEmptyString, lineNumber);
		}
	}

	wxLogMessage(wxString::Format(wxT("Install Path: %s"), ManagerST::Get()->GetAppInstallDirectory().c_str()));
	wxLogMessage(wxString::Format(wxT("Startup Path: %s"), ManagerST::Get()->GetStarupDirectory().c_str()));
	wxLogMessage(wxString::Format(wxT("Application data path: %s"), ManagerST::Get()->GetAppDataDir().c_str()));
	wxLogMessage(wxString::Format(wxT("Application share path: %s"), ManagerST::Get()->GetAppShareDir().c_str()));
	wxLogMessage(wxString::Format(wxT("Application config path: %s"), ManagerST::Get()->GetConfigDir().c_str()));
	wxLogMessage(wxString::Format(wxT("Lexers path: %s"), ManagerST::Get()->GetLexersDir().c_str()));
	wxLogMessage(wxString::Format(wxT("Plugins path: %s"), ManagerST::Get()->GetPluginsDir().c_str()));

  return TRUE;
}

int App::OnExit()
{
	EditorConfigST::Free();
	ConfFileLocator::Release();
	return 0;
}

bool App::CopySettings(const wxString &destDir, wxString& strInstallPath)
{
	wxLogNull noLog;
	
	///////////////////////////////////////////////////////////////////////////////////////////
	// copy new settings from the global installation location which is currently located at
	// /usr/local/share/embeddedlite/ (Linux) or at embeddedlite.app/Contents/SharedSupport
	///////////////////////////////////////////////////////////////////////////////////////////
	CopyDir(strInstallPath + wxT("/templates/"), destDir + wxT("/templates/"));
	CopyDir(strInstallPath + wxT("/lexers/"), destDir + wxT("/lexers/"));
	massCopy  (strInstallPath + wxT("/images/"), wxT("*.png"), destDir + wxT("/images/"));
	wxCopyFile(strInstallPath + wxT("/config/embeddedlite.xml.default"), destDir + wxT("/config/embeddedlite.xml.default"));
	wxCopyFile(strInstallPath + wxT("/rc/menu.xrc"), destDir + wxT("/rc/menu.xrc"));
	wxCopyFile(strInstallPath + wxT("/index.html"), destDir + wxT("/index.html"));
	wxCopyFile(strInstallPath + wxT("/svnreport.html"), destDir + wxT("/svnreport.html"));
	wxCopyFile(strInstallPath + wxT("/astyle.sample"), destDir + wxT("/astyle.sample"));
	wxCopyFile(strInstallPath + wxT("/config/accelerators.conf.default"), destDir + wxT("/config/accelerators.conf.default"));
	return true;
}

void App::OnFatalException()
{
#if wxUSE_STACKWALKER
	Manager *mgr = ManagerST::Get();
	wxString startdir = mgr->GetStarupDirectory();
	startdir << wxT("/crash.log");

	wxFileOutputStream outfile(startdir);
	wxTextOutputStream out(outfile);
	out.WriteString(wxDateTime::Now().FormatTime() + wxT("\n"));
	StackWalker walker(&out);
	walker.Walk();
	wxAppBase::ExitMainLoop();
#endif
}

bool App::CheckSingularity(const wxCmdLineParser &parser, const wxString &strStartupDir)
{
	// check for single instance
	long singleInstance(1);
	EditorConfigST::Get()->GetLongValue(wxT("SingleInstance"), singleInstance);
	if ( singleInstance ) {
		const wxString name = wxString::Format(wxT("EmbeddedLite-%s"), wxGetUserId().c_str());

		m_singleInstance = new wxSingleInstanceChecker(name);
		if (m_singleInstance->IsAnotherRunning()) {
			// prepare commands file for the running instance
			wxString files;
			for (size_t i=0; i< parser.GetParamCount(); i++) {
				wxString argument = parser.GetParam(i);

				//convert to full path and open it
				wxFileName fn(argument);
				fn.MakeAbsolute(strStartupDir);
				files << fn.GetFullPath() << wxT("\n");
			}

			if (files.IsEmpty() == false) {
				Mkdir(ManagerST::Get()->GetStarupDirectory() + wxT("/ipc"));

				wxString file_name, tmp_file;
				tmp_file 	<< ManagerST::Get()->GetStarupDirectory()
							<< wxT("/ipc/command.msg.tmp");

				file_name 	<< ManagerST::Get()->GetStarupDirectory()
							<< wxT("/ipc/command.msg");

				// write the content to a temporary file, once completed,
				// rename the file to the actual file name
				WriteFileUTF8(tmp_file, files);
				wxRenameFile(tmp_file, file_name);
			}
			return false;
		}
	}
	return true;
}

void App::MacOpenFile(const wxString& fileName)
{
	switch (FileExtManager::GetType(fileName)) {
	case FileExtManager::TypeWorkspace:
		ManagerST::Get()->OpenWorkspace(fileName);
		break;
	default:
		Frame::Get()->GetMainBook()->OpenFile(fileName);
		break;
	}
}
