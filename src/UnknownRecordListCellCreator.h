/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _UNKNOWN_RECORD_LIST_CELL_CREATOR_H_
#define _UNKNOWN_RECORD_LIST_CELL_CREATOR_H_

#include "ContentItemCellCreator.h"

class UnknownRecordListCellCreator : public ContentItemCellCreator
{

public:

	UnknownRecordListCellCreator(void);

	virtual void updateCell(const QModelIndex &index, MWidget *cell) const;

};

#endif /* _UNKNOWN_RECORD_LIST_CELL_CREATOR_H_ */
