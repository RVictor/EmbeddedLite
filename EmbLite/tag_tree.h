/**
  \file 

  \brief EmbeddedLite file
  \author V. Ridtchenko

  \notes

  Copyright: (C) 2010 by Victor Ridtchenko

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
*/
#ifndef EMBEDDEDLITE_TAG_TREE_H
#define EMBEDDEDLITE_TAG_TREE_H

#include <wx/string.h>
#include "smart_ptr.h"
#include "tree.h"
#include "entry.h"

typedef TreeNode<wxString, TagEntry> TagNode;

/**
 * Tree representation of tags.
 * This is basically a data structure representing the GUI symbol tree.
 *
 * \date 08-23-2006
 * \author eran
 *
 */
class TagTree : public Tree<wxString, TagEntry>
{
public:
	/**
	 * Construct a tree with roots' values key & data.
	 * \param key Root's key
	 * \param data Root's data
	 */
	TagTree(const wxString& key, const TagEntry& data);


	/**
	 * Destructor
	 */
	virtual ~TagTree();

	/**
	 * Add an entry to the tree.
	 * This functions will add all parents of the tag to the tree if non exist (or some of some exist).
	 * For example: if TagName is box::m_string, this functions will make sure that a node 'box' exists.
	 * If not, it will add it and then will add m_string to it as its child.
	 * \param tag Tag to add
	 * \return new node that was added to the tree.
	 */
	TagNode* AddEntry(TagEntry& tag);

};

typedef SmartPtr<TagTree> TagTreePtr;

#endif // EMBEDDEDLITE_TAG_TREE_H
