#include "compilerassembleroptionspage.h"
#include "build_settings_config.h"
#include "compiler.h"
#include "globals.h"

CompilerAssemblerOptionsPage::CompilerAssemblerOptionsPage( wxWindow* parent, const wxString &cmpname )
: CompilerAssemblerOptionsBase( parent )
, m_cmpname(cmpname)
, m_selectedCmpOption(wxNOT_FOUND)
{
	m_listAssemblerOptions->InsertColumn(0, wxT("Switch"));
	m_listAssemblerOptions->InsertColumn(1, wxT("Help"));
	
	CompilerPtr cmp = BuildSettingsConfigST::Get()->GetCompiler(m_cmpname);
	const Compiler::CmpCmdLineOptions& cmpOptions = cmp->GetAssemblerOptions();
	Compiler::CmpCmdLineOptions::const_iterator itCmpOption = cmpOptions.begin();
	for ( ; itCmpOption != cmpOptions.end(); ++itCmpOption)
	{
		const Compiler::CmpCmdLineOption& cmpOption = itCmpOption->second;
		long idx = m_listAssemblerOptions->InsertItem(m_listAssemblerOptions->GetItemCount(), cmpOption.name);
		m_listAssemblerOptions->SetItem(idx, 1, cmpOption.help);
	}
	
	m_listAssemblerOptions->SetColumnWidth(0, 100);
	m_listAssemblerOptions->SetColumnWidth(1, wxLIST_AUTOSIZE);
	
}

void CompilerAssemblerOptionsPage::Save(CompilerPtr cmp)
{
	Compiler::CmpCmdLineOptions cmpOptions;
	for (int idx = 0; idx < m_listAssemblerOptions->GetItemCount(); ++idx)
	{
		Compiler::CmpCmdLineOption cmpOption;
		cmpOption.name = m_listAssemblerOptions->GetItemText(idx);
		cmpOption.help = GetColumnText(m_listAssemblerOptions, idx, 1);
		
		cmpOptions[cmpOption.name] = cmpOption;
	}
	cmp->SetAssemblerOptions(cmpOptions);
}

void CompilerAssemblerOptionsPage::OnAssemblerOptionActivated( wxListEvent& event )
{
	if (m_selectedCmpOption == wxNOT_FOUND) {
		return;
	}
	
	wxString name = m_listAssemblerOptions->GetItemText(m_selectedCmpOption);
	wxString help = GetColumnText(m_listAssemblerOptions, m_selectedCmpOption, 1);
	CompilerAssemblerOptionDialog dlg(this, name, help);
	if (dlg.ShowModal() == wxID_OK)
	{
		SetColumnText(m_listAssemblerOptions, m_selectedCmpOption, 0, dlg.m_sName);
		SetColumnText(m_listAssemblerOptions, m_selectedCmpOption, 1, dlg.m_sHelp);
		m_listAssemblerOptions->SetColumnWidth(1, wxLIST_AUTOSIZE);
	}
}

void CompilerAssemblerOptionsPage::OnNewAssemblerOption( wxCommandEvent& event )
{
	CompilerAssemblerOptionDialog dlg(this, wxEmptyString, wxEmptyString);
	if (dlg.ShowModal() == wxID_OK)
	{
		long idx = m_listAssemblerOptions->InsertItem(m_listAssemblerOptions->GetItemCount(), dlg.m_sName);
		m_listAssemblerOptions->SetItem(idx, 1, dlg.m_sHelp);
		m_listAssemblerOptions->SetColumnWidth(1, wxLIST_AUTOSIZE);
	}
}

void CompilerAssemblerOptionsPage::OnDeleteAssemblerOption( wxCommandEvent& event )
{
	if (m_selectedCmpOption != wxNOT_FOUND) {
		if (wxMessageBox(_("Are you sure you want to delete this assembler option?"), wxT("EmbeddedLite"), wxYES_NO|wxCANCEL) == wxYES) {
			m_listAssemblerOptions->DeleteItem(m_selectedCmpOption);
			m_listAssemblerOptions->SetColumnWidth(1, wxLIST_AUTOSIZE);
			m_selectedCmpOption = wxNOT_FOUND;
		}
	}
}

void CompilerAssemblerOptionsPage::OnAssemblerOptionDeSelected(wxListEvent& event)
{
#ifndef __WXMAC__
	m_selectedCmpOption = wxNOT_FOUND;
#endif
	event.Skip();
}

void CompilerAssemblerOptionsPage::OnAssemblerOptionSelected(wxListEvent& event)
{
	m_selectedCmpOption = event.m_itemIndex;
	event.Skip();
}
