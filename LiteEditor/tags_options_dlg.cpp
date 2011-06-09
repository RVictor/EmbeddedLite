/**
  \file tags_options_dlg.cpp

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifdef WX_PRECOMP

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#else
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "tags_options_dlg.h"
#include "ctags_manager.h"
#include "windowattrmanager.h"
#include "macros.h"
#include "wx/tokenzr.h"
#include "add_option_dialog.h"

//---------------------------------------------------------

TagsOptionsDlg::TagsOptionsDlg( wxWindow* parent, const TagsOptionsData& data)
		: TagsOptionsBaseDlg( parent )
		, m_data(data)
{
	m_colour_flags = m_data.GetCcColourFlags();
	InitValues();

	m_checkBoxMarkTagsFilesInBold->SetFocus();
	WindowAttrManager::Load(this, wxT("TagsOptionsDlgAttr"), NULL);
}

TagsOptionsDlg::~TagsOptionsDlg()
{
	WindowAttrManager::Save(this, wxT("TagsOptionsDlgAttr"), NULL);
}

void TagsOptionsDlg::InitValues()
{
	//initialize the EmbeddedLite page
	m_checkParseComments->SetValue                 (m_data.GetFlags() & CC_PARSE_COMMENTS ? true : false);
	m_checkDisplayFunctionTip->SetValue            (m_data.GetFlags() & CC_DISP_FUNC_CALLTIP ? true : false);
	m_checkDisplayTypeInfo->SetValue               (m_data.GetFlags() & CC_DISP_TYPE_INFO ? true : false);
	m_checkDisplayComments->SetValue               (m_data.GetFlags() & CC_DISP_COMMENTS ? true : false);
	m_checkFilesWithoutExt->SetValue               (m_data.GetFlags() & CC_PARSE_EXT_LESS_FILES ? true : false);
	m_checkColourLocalVars->SetValue               (m_data.GetFlags() & CC_COLOUR_VARS ? true : false);
	m_checkColourProjTags->SetValue                (m_data.GetFlags() & CC_COLOUR_WORKSPACE_TAGS ? true : false);
	m_checkCppKeywordAssist->SetValue              (m_data.GetFlags() & CC_CPP_KEYWORD_ASISST ? true : false);
	m_checkDisableParseOnSave->SetValue            (m_data.GetFlags() & CC_DISABLE_AUTO_PARSING ? true : false);
	m_checkBoxMarkTagsFilesInBold->SetValue        (m_data.GetFlags() & CC_MARK_TAGS_FILES_IN_BOLD ? true : false);
	m_checkBoxretagWorkspaceOnStartup->SetValue    (m_data.GetFlags() & CC_RETAG_WORKSPACE_ON_STARTUP ? true : false);
	m_checkBoxDeepUsingNamespaceResolving->SetValue(m_data.GetFlags() & CC_DEEP_SCAN_USING_NAMESPACE_RESOLVING ? true : false);

	m_checkBoxClass->SetValue                     (m_data.GetCcColourFlags() & CC_COLOUR_CLASS);
	m_checkBoxEnum->SetValue                      (m_data.GetCcColourFlags() & CC_COLOUR_ENUM);
	m_checkBoxFunction->SetValue                  (m_data.GetCcColourFlags() & CC_COLOUR_FUNCTION);
	m_checkBoxMacro->SetValue                     (m_data.GetCcColourFlags() & CC_COLOUR_MACRO);
	m_checkBoxNamespace->SetValue                 (m_data.GetCcColourFlags() & CC_COLOUR_NAMESPACE);
	m_checkBoxPrototype->SetValue                 (m_data.GetCcColourFlags() & CC_COLOUR_PROTOTYPE);
	m_checkBoxStruct->SetValue                    (m_data.GetCcColourFlags() & CC_COLOUR_STRUCT);
	m_checkBoxTypedef->SetValue                   (m_data.GetCcColourFlags() & CC_COLOUR_TYPEDEF);
	m_checkBoxUnion->SetValue                     (m_data.GetCcColourFlags() & CC_COLOUR_UNION);
	m_checkBoxEnumerator->SetValue                (m_data.GetCcColourFlags() & CC_COLOUR_ENUMERATOR);
	m_checkBoxMember->SetValue                    (m_data.GetCcColourFlags() & CC_COLOUR_MEMBER);
	m_checkBoxVariable->SetValue                  (m_data.GetCcColourFlags() & CC_COLOUR_VARIABLE);
	m_listBoxSearchPaths->Append                  (m_data.GetParserSearchPaths() );
	m_listBoxSearchPaths1->Append                 (m_data.GetParserExcludePaths() );
	m_spinCtrlMaxItemToColour->SetValue           (m_data.GetMaxItemToColour() );
	m_textPrep->SetValue                          (m_data.GetTokens());
	m_textTypes->SetValue                         (m_data.GetTypes());
	
	m_textFileSpec->SetValue(m_data.GetFileSpec());
	m_comboBoxLang->Clear();
	m_comboBoxLang->Append(m_data.GetLanguages());
	if ( m_data.GetLanguages().IsEmpty() == false ) {
		wxString lan = m_data.GetLanguages().Item(0);
		m_comboBoxLang->SetStringSelection(lan);
	} else {
		m_comboBoxLang->Append(wxT("c++"));
		m_comboBoxLang->SetSelection(0);
	}
}

void TagsOptionsDlg::OnButtonOK(wxCommandEvent &event)
{
	wxUnusedVar(event);
	CopyData();
	EndModal(wxID_OK);
}

void TagsOptionsDlg::OnButtonAdd(wxCommandEvent &event)
{
	wxUnusedVar(event);
	//pop up add option dialog
	AddOptionDlg dlg(this, m_textPrep->GetValue());
	if (dlg.ShowModal() == wxID_OK) {
		m_textPrep->SetValue(dlg.GetValue());
	}
}

void TagsOptionsDlg::CopyData()
{
	//save data to the interal member m_data
	SetFlag(CC_DISP_COMMENTS,                       m_checkDisplayComments->IsChecked());
	SetFlag(CC_DISP_FUNC_CALLTIP,                   m_checkDisplayFunctionTip->IsChecked());
	SetFlag(CC_DISP_TYPE_INFO,                      m_checkDisplayTypeInfo->IsChecked());
	SetFlag(CC_PARSE_COMMENTS,                      m_checkParseComments->IsChecked());
	SetFlag(CC_PARSE_EXT_LESS_FILES,                m_checkFilesWithoutExt->IsChecked());
	SetFlag(CC_COLOUR_VARS,                         m_checkColourLocalVars->IsChecked());
	SetFlag(CC_CPP_KEYWORD_ASISST,                  m_checkCppKeywordAssist->IsChecked());
	SetFlag(CC_DISABLE_AUTO_PARSING,                m_checkDisableParseOnSave->IsChecked());
	SetFlag(CC_COLOUR_WORKSPACE_TAGS,               m_checkColourProjTags->IsChecked());
	SetFlag(CC_MARK_TAGS_FILES_IN_BOLD,             m_checkBoxMarkTagsFilesInBold->IsChecked());
	SetFlag(CC_RETAG_WORKSPACE_ON_STARTUP,          m_checkBoxretagWorkspaceOnStartup->IsChecked());
	SetFlag(CC_DEEP_SCAN_USING_NAMESPACE_RESOLVING, m_checkBoxDeepUsingNamespaceResolving->IsChecked());
	
	SetColouringFlag(CC_COLOUR_CLASS,      m_checkBoxClass->IsChecked());
	SetColouringFlag(CC_COLOUR_ENUM,       m_checkBoxEnum->IsChecked());
	SetColouringFlag(CC_COLOUR_FUNCTION,   m_checkBoxFunction->IsChecked());
	SetColouringFlag(CC_COLOUR_MACRO,      m_checkBoxMacro->IsChecked());
	SetColouringFlag(CC_COLOUR_NAMESPACE,  m_checkBoxNamespace->IsChecked());
	SetColouringFlag(CC_COLOUR_PROTOTYPE,  m_checkBoxPrototype->IsChecked());
	SetColouringFlag(CC_COLOUR_STRUCT,     m_checkBoxStruct->IsChecked());
	SetColouringFlag(CC_COLOUR_TYPEDEF,    m_checkBoxTypedef->IsChecked());
	SetColouringFlag(CC_COLOUR_UNION,      m_checkBoxUnion->IsChecked());
	SetColouringFlag(CC_COLOUR_ENUMERATOR, m_checkBoxEnumerator->IsChecked());
	SetColouringFlag(CC_COLOUR_VARIABLE,   m_checkBoxVariable->IsChecked());
	SetColouringFlag(CC_COLOUR_MEMBER,     m_checkBoxMember->IsChecked());

	m_data.SetFileSpec(m_textFileSpec->GetValue());
	
	m_data.SetTokens(m_textPrep->GetValue());
	m_data.SetTypes(m_textTypes->GetValue());
	m_data.SetLanguages(m_comboBoxLang->GetStrings());
	m_data.SetLanguageSelection(m_comboBoxLang->GetStringSelection());
	m_data.SetParserSearchPaths( m_listBoxSearchPaths->GetStrings() );
	m_data.SetParserExcludePaths( m_listBoxSearchPaths1->GetStrings() );
	m_data.SetMaxItemToColour( m_spinCtrlMaxItemToColour->GetValue() );

}

void TagsOptionsDlg::SetFlag(CodeCompletionOpts flag, bool set)
{
	if (set) {
		m_data.SetFlags(m_data.GetFlags() | flag);
	} else {
		m_data.SetFlags(m_data.GetFlags() & ~(flag));
	}
}

void TagsOptionsDlg::SetColouringFlag(CodeCompletionColourOpts flag, bool set)
{
	if (set) {
		m_data.SetCcColourFlags(m_data.GetCcColourFlags() | flag);
	} else {
		m_data.SetCcColourFlags(m_data.GetCcColourFlags() & ~(flag));
	}
}

void TagsOptionsDlg::OnColourWorkspaceUI(wxUpdateUIEvent& e)
{
	e.Enable(m_checkColourProjTags->IsChecked());
}

void TagsOptionsDlg::OnAddSearchPath(wxCommandEvent& e)
{
	wxUnusedVar(e);
	wxString new_path = wxDirSelector(wxT("Add Parser Search Path:"), wxT(""), wxDD_DEFAULT_STYLE, wxDefaultPosition, this);
	if(new_path.IsEmpty() == false){
		if(m_listBoxSearchPaths->GetStrings().Index(new_path) == wxNOT_FOUND) {
			m_listBoxSearchPaths->Append(new_path);
		}
	}
}

void TagsOptionsDlg::OnAddSearchPathUI(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void TagsOptionsDlg::OnClearAll(wxCommandEvent& e)
{
	wxUnusedVar(e);
	m_listBoxSearchPaths->Clear();
}

void TagsOptionsDlg::OnClearAllUI(wxUpdateUIEvent& e)
{
	e.Enable(m_listBoxSearchPaths->IsEmpty() == false);
}

void TagsOptionsDlg::OnRemoveSearchPath(wxCommandEvent& e)
{
	wxUnusedVar(e);
	int sel = m_listBoxSearchPaths->GetSelection();
	if( sel != wxNOT_FOUND) {
		m_listBoxSearchPaths->Delete((unsigned int)sel);
	}
}

void TagsOptionsDlg::OnRemoveSearchPathUI(wxUpdateUIEvent& e)
{
	e.Enable(m_listBoxSearchPaths->GetSelection() != wxNOT_FOUND);
}

void TagsOptionsDlg::OnAddExcludePath(wxCommandEvent& e)
{
	wxUnusedVar(e);
	wxString new_path = wxDirSelector(wxT("Add Parser Search Path:"), wxT(""), wxDD_DEFAULT_STYLE, wxDefaultPosition, this);
	if(new_path.IsEmpty() == false){
		if(m_listBoxSearchPaths1->GetStrings().Index(new_path) == wxNOT_FOUND) {
			m_listBoxSearchPaths1->Append(new_path);
		}
	}
}

void TagsOptionsDlg::OnAddExcludePathUI(wxUpdateUIEvent& e)
{
	e.Enable(true);
}

void TagsOptionsDlg::OnClearAllExcludePaths(wxCommandEvent& e)
{
	wxUnusedVar(e);
	m_listBoxSearchPaths1->Clear();
}

void TagsOptionsDlg::OnClearAllExcludePathsUI(wxUpdateUIEvent& e)
{
	e.Enable(m_listBoxSearchPaths1->IsEmpty() == false);
}

void TagsOptionsDlg::OnRemoveExcludePath(wxCommandEvent& e)
{
	wxUnusedVar(e);
	int sel = m_listBoxSearchPaths1->GetSelection();
	if( sel != wxNOT_FOUND) {
		m_listBoxSearchPaths1->Delete((unsigned int)sel);
	}
}

void TagsOptionsDlg::OnRemoveExcludePathUI(wxUpdateUIEvent& e)
{
	e.Enable(m_listBoxSearchPaths1->GetSelection() != wxNOT_FOUND);
}

