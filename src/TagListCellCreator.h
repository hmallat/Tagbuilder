/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_LIST_CELL_CREATOR_H_
#define _TAG_LIST_CELL_CREATOR_H_

#include <MAbstractCellCreator>
#include <MContentItem>

class TagListCellCreator : public MAbstractCellCreator<MContentItem>
{

public:

	TagListCellCreator(void);

	void updateCell(const QModelIndex &index, MWidget *cell) const;

};

#endif /* _TAG_LIST_CELL_CREATOR_H_ */
