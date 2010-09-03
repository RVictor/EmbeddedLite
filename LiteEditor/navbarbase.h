///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Aug 25 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __navbarbase__
#define __navbarbase__

#include <wx/intl.h>

#include <wx/string.h>
#include <wx/choice.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/splitter.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class NavBarBase
///////////////////////////////////////////////////////////////////////////////
class NavBarBase : public wxPanel 
{
	private:
	
	protected:
		wxSplitterWindow* m_splitter;
		wxPanel* m_panel1;
		wxChoice* m_scope;
		wxPanel* m_panel2;
		wxChoice* m_func;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnScope( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnScopeListMouseDown( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnFunction( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFuncListMouseDown( wxMouseEvent& event ) { event.Skip(); }
		
	
	public:
		
		NavBarBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
		~NavBarBase();
		void m_splitterOnIdle( wxIdleEvent& )
		{
			m_splitter->SetSashPosition( 150 );
			m_splitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( NavBarBase::m_splitterOnIdle ), NULL, this );
		}
		
	
};

#endif //__navbarbase__
