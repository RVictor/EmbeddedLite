#ifndef SVNCOMMITHANDLER_H
#define SVNCOMMITHANDLER_H

#include "svn_default_command_handler.h" // Base class

//----------------------------------------------------
// Commit Handler
//----------------------------------------------------

class SvnCommitHandler : public SvnDefaultCommandHandler {
public:
	SvnCommitHandler(Subversion2 *plugin, int commandId, wxEvtHandler *owner) : SvnDefaultCommandHandler(plugin, commandId, owner) {};
	virtual ~SvnCommitHandler(){};

public:
	virtual void Process(const wxString &output);
};

//----------------------------------------------------
// Update Handler
//----------------------------------------------------

class SvnUpdateHandler : public SvnDefaultCommandHandler {

public:
	SvnUpdateHandler(Subversion2 *plugin, int commandId, wxEvtHandler *owner) : SvnDefaultCommandHandler(plugin, commandId, owner) {};
	virtual ~SvnUpdateHandler(){};

public:
	virtual void Process(const wxString &output);
};

//----------------------------------------------------
// Diff Handler
//----------------------------------------------------

class SvnDiffHandler : public SvnDefaultCommandHandler {

public:
	SvnDiffHandler(Subversion2 *plugin, int commandId, wxEvtHandler *owner) : SvnDefaultCommandHandler(plugin, commandId, owner) {};
	virtual ~SvnDiffHandler(){};

public:
	virtual void Process(const wxString &output);
};

//----------------------------------------------------
// Patch Handler
//----------------------------------------------------

class SvnPatchHandler : public SvnDefaultCommandHandler {

public:
	SvnPatchHandler(Subversion2 *plugin, int commandId, wxEvtHandler *owner) : SvnDefaultCommandHandler(plugin, commandId, owner) {};
	virtual ~SvnPatchHandler(){};

public:
	virtual void Process(const wxString &output);
};

//----------------------------------------------------
// Dry-Run Patch Handler
//----------------------------------------------------

class SvnPatchDryRunHandler : public SvnDefaultCommandHandler {

public:
	SvnPatchDryRunHandler(Subversion2 *plugin, int commandId, wxEvtHandler *owner) : SvnDefaultCommandHandler(plugin, commandId, owner) {};
	virtual ~SvnPatchDryRunHandler(){};

public:
	virtual void Process(const wxString &output);
};

//----------------------------------------------------
//Svn version handler
//----------------------------------------------------

class SvnVersionHandler : public SvnDefaultCommandHandler {

public:
	SvnVersionHandler(Subversion2 *plugin, int commandId, wxEvtHandler *owner) : SvnDefaultCommandHandler(plugin, commandId, owner) {};
	virtual ~SvnVersionHandler(){};

public:
	virtual void Process(const wxString &output);
};

//----------------------------------------------------
//Svn Log handler
//----------------------------------------------------

class SvnLogHandler : public SvnDefaultCommandHandler {
	bool m_compact;
protected:
	wxString Compact(const wxString &message);
public:
	SvnLogHandler(Subversion2 *plugin, bool compact, int commandId, wxEvtHandler *owner) : SvnDefaultCommandHandler(plugin, commandId, owner), m_compact(compact) {};
	virtual ~SvnLogHandler(){};

public:
	virtual void Process(const wxString &output);
};

//----------------------------------------------------
// Svn Checkout handler
//----------------------------------------------------

class SvnCheckoutHandler : public SvnDefaultCommandHandler {
public:
	SvnCheckoutHandler(Subversion2 *plugin, int commandId, wxEvtHandler *owner) : SvnDefaultCommandHandler(plugin, commandId, owner){};
	virtual ~SvnCheckoutHandler(){};

public:
	virtual void Process(const wxString &output);
};

//----------------------------------------------------
// Svn blame handler
//----------------------------------------------------

class SvnBlameHandler : public SvnCommandHandler {
public:
	SvnBlameHandler(Subversion2 *plugin, int commandId, wxEvtHandler *owner) : SvnCommandHandler(plugin, commandId, owner){};
	virtual ~SvnBlameHandler(){};

public:
	virtual void Process(const wxString &output);
};
#endif // SVNCOMMITHANDLER_H
