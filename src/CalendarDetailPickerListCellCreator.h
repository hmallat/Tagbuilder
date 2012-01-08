/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CALENDAR_DETAIL_PICKER_LIST_CELL_CREATOR_H_
#define _CALENDAR_DETAIL_PICKER_LIST_CELL_CREATOR_H_

#include "ContentItemCellCreator.h"

class CalendarDetailPickerListCellCreator : public ContentItemCellCreator
{

public:

	CalendarDetailPickerListCellCreator(const QString styleName = "CommonContentItem");

	virtual void updateCell(const QModelIndex &index, MWidget *cell) const;

};

#endif /* _CALENDAR_DETAIL_PICKER_LIST_CELL_CREATOR_H_ */
