/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CONTACT_SELECTION_PAGE_LIST_CELL_CREATOR_H_
#define _CONTACT_SELECTION_PAGE_LIST_CELL_CREATOR_H_

#include <MAbstractCellCreator>
#include <MContentItem>

class ContactSelectionPageListCellCreator : 
public MAbstractCellCreator<MContentItem>
{

public:

	void updateCell(const QModelIndex &index, MWidget *cell) const;

};

#endif /* _CONTACT_SELECTION_PAGE_LIST_CELL_CREATOR_H_ */