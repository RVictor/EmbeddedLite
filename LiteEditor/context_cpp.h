//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// copyright            : (C) 2008 by Eran Ifrah
// file name            : context_cpp.h
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
#ifndef CONTEXT_CPP_H
#define CONTEXT_CPP_H

#include "context_base.h"
#include "cpptoken.h"
#include "ctags_manager.h"
#include <map>
#include "entry.h"

class RefactorSource;

class ContextCpp : public ContextBase
{
	std::map<wxString, int> m_propertyInt;
	wxMenu *m_rclickMenu;

	//images used by the C++ context
	static wxBitmap m_classBmp;
	static wxBitmap m_structBmp;
	static wxBitmap m_namespaceBmp;
	static wxBitmap m_variableBmp;
	static wxBitmap m_tpyedefBmp;
	static wxBitmap m_memberPrivateBmp;
	static wxBitmap m_memberPublicBmp;
	static wxBitmap m_memberProtectedeBmp;
	static wxBitmap m_functionPrivateBmp;
	static wxBitmap m_functionPublicBmp;
	static wxBitmap m_functionProtectedeBmp;
	static wxBitmap m_macroBmp;
	static wxBitmap m_enumBmp;
	static wxBitmap m_enumeratorBmp;
	static wxBitmap m_cppFileBmp;
	static wxBitmap m_hFileBmp;
	static wxBitmap m_otherFileBmp;

private:
	bool TryOpenFile(const wxFileName &fileName);
	void DisplayCompletionBox(const std::vector<TagEntryPtr> &tags, const wxString &word, bool showFullDecl);
	void DisplayFilesCompletionBox(const wxString &word);
	bool DoGetFunctionBody(long curPos, long &blockStartPos, long &blockEndPos, wxString &content);
	void Initialize();
	void DoCodeComplete(long pos);
	void DoCreateFile(const wxFileName &fn);
	int  DoGetCalltipParamterIndex();
	void DoUpdateCalltipHighlight();

public:
	ContextCpp(LEditor *container);
	virtual ~ContextCpp();
	ContextCpp();
	virtual ContextBase *NewInstance(LEditor *container);
	virtual void CompleteWord();
	virtual void CodeComplete(long pos = wxNOT_FOUND);
	virtual void GotoDefinition();
	virtual void GotoPreviousDefintion();
	virtual TagEntryPtr GetTagAtCaret(bool scoped, bool impl);
	virtual void AutoIndent(const wxChar&);
	virtual	bool IsCommentOrString(long pos);
	virtual	bool IsComment(long pos);
	virtual void AddMenuDynamicContent(wxMenu *menu);
	virtual void RemoveMenuDynamicContent(wxMenu *menu);
	virtual void ApplySettings();
	virtual void RetagFile();
	virtual wxString CallTipContent();
	virtual void SetActive();

	virtual void SemicolonShift();

	// ctrl-click style navigation support
	virtual int GetHyperlinkRange(int pos, int &start, int &end);
	virtual void GoHyperlink(int start, int end, int type, bool alt);

	//override swapfiles features
	virtual void SwapFiles(const wxFileName &fileName);

	//Event handlers
	virtual void OnDwellEnd(wxScintillaEvent &event);
	virtual void OnDwellStart(wxScintillaEvent &event);
	virtual void OnDbgDwellEnd(wxScintillaEvent &event);
	virtual void OnDbgDwellStart(wxScintillaEvent &event);
	virtual void OnSciUpdateUI(wxScintillaEvent &event);
	virtual void OnFileSaved();
	virtual void AutoAddComment();

	//Capture menu events
	//return this context specific right click menu
	virtual wxMenu *GetMenu() {
		return m_rclickMenu;
	}
	virtual void OnSwapFiles(wxCommandEvent &event);
	virtual void OnInsertDoxyComment(wxCommandEvent &event);
	virtual void OnCommentSelection(wxCommandEvent &event);
	virtual void OnCommentLine(wxCommandEvent &event);
	virtual void OnGenerateSettersGetters(wxCommandEvent &event);
	virtual void OnFindImpl(wxCommandEvent &event);
	virtual void OnFindDecl(wxCommandEvent &event);
	virtual void OnGotoFunctionStart(wxCommandEvent &event);
	virtual void OnGotoNextFunction(wxCommandEvent& event);
	virtual void OnKeyDown(wxKeyEvent &event);
	virtual void OnUpdateUI(wxUpdateUIEvent &event);
	virtual void OnContextOpenDocument(wxCommandEvent &event);
	virtual void OnAddIncludeFile(wxCommandEvent &e);
	virtual void OnMoveImpl(wxCommandEvent &e);
	virtual void OnAddImpl(wxCommandEvent &e);
	virtual void OnAddMultiImpl(wxCommandEvent &e);
	virtual void OnOverrideParentVritualFunctions(wxCommandEvent &e);
	virtual void OnRenameFunction(wxCommandEvent &e);
	virtual void OnRetagFile(wxCommandEvent &e);
	virtual void OnUserTypedXChars(const wxString &word);
	virtual void OnCallTipClick(wxScintillaEvent &e);
	virtual void OnCalltipCancel();
	DECLARE_EVENT_TABLE();

private:
	wxString      GetWordUnderCaret();
	wxString      GetFileImageString(const wxString &ext);
	wxString      GetImageString(const TagEntry &entry);
	wxString      GetExpression(long pos, bool onlyWord, LEditor *editor = NULL, bool forCC = true);
	void          DoGotoSymbol(TagEntryPtr tag);
	bool          IsIncludeStatement(const wxString &line, wxString *fileName = NULL);
	void          RemoveDuplicates(std::vector<TagEntryPtr>& src, std::vector<TagEntryPtr>& target);
	int           FindLineToAddInclude();
	void          MakeCppKeywordsTags(const wxString &word, std::vector<TagEntryPtr> &tags);
	void          DoOpenWorkspaceFile();
	void          DoSetProjectPaths();
	
public:
	void          DoMakeDoxyCommentString(DoxygenComment &dc);

private:
	/**
	 * \brief try to find a swapped file for this rhs. The logic is based on the C++ coding conventions
	 * a swapped file for a.cpp will be a.h or a.hpp
	 * \param rhs input
	 * \param lhs output
	 * \return true if such sibling file exist, false otherwise
	 */
	bool FindSwappedFile(const wxFileName &rhs, wxString &lhs);

	/**
	 * \brief parse list of files and construct a token database that will be used for refactoring
	 * \param word word to search
	 * \param files list of files to parse
	 */
	void BuildRefactorDatabase(const wxString& word, CppTokensMap &l);

	/**
	 * \brief replace list of tokens representd by li with 'word'
	 * \param li
	 * \return
	 */
	void ReplaceInFiles(const wxString &word, std::list<CppToken> &li);

	/**
	 * \brief
	 * \param ctrl
	 * \param pos
	 * \param word
	 * \param rs
	 * \return
	 */
	bool ResolveWord(LEditor *ctrl, int pos, const wxString &word, RefactorSource *rs);

	/**
	* \brief open file specified by the 'fileName' parameter and append 'text'
	* to its content
	* \param fileName file to open. Must be in full path
	* \param text string text to append
	* \return true on success, false otherwise
	*/
	LEditor* OpenFileAndAppend(const wxString &fileName, const wxString &text);

	/**
	 * @brief open file specified by the 'fileName' parameter and append 'text'
	 * to its content
	 * @param fileName file to open. Must be in full path
	 * @param text string text to append
	 * @param format set to true of formatting should take place after text insertion
	 * @return true on success, false otherwise
	 */
	bool OpenFileAppendAndFormat(const wxString &fileName, const wxString &text, bool format);

	/**
	 * @brief format editor
	 * @param editor
	 */
	void DoFormatEditor(LEditor *editor);
};

#endif // CONTEXT_CPP_H
