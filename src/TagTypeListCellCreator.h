/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_TYPE_LIST_CELL_CREATOR_H_
#define _TAG_TYPE_LIST_CELL_CREATOR_H_

#include "ContentItemCellCreator.h"

class TagTypeListCellCreator : public ContentItemCellCreator
{

public:

	TagTypeListCellCreator(void);

	virtual void updateCell(const QModelIndex &index, MWidget *cell) const;

};

#endif /* _TAG_TYPE_LIST_CELL_CREATOR_H_ */
