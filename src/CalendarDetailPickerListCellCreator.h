/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CALENDAR_DETAIL_PICKER_LIST_CELL_CREATOR_H_
#define _CALENDAR_DETAIL_PICKER_LIST_CELL_CREATOR_H_

#include <MAbstractCellCreator>

class MWidgetController;

class CalendarDetailPickerListCellCreator : public MAbstractCellCreator<MWidgetController>
{

public:

	CalendarDetailPickerListCellCreator(void);

	virtual QSizeF cellSize(void) const;

	virtual MWidget *createCell(const QModelIndex &index, 
				    MWidgetRecycler &recycler) const;

	virtual void updateCell(const QModelIndex &index, MWidget *cell) const;

private:

	QSizeF m_cellSize;

};

#endif /* _CALENDAR_DETAIL_PICKER_LIST_CELL_CREATOR_H_ */
