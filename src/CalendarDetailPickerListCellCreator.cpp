/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CalendarDetailPickerListCellCreator.h"
#include "CalendarDetailCell.h"

#include <QVariant>

CalendarDetailPickerListCellCreator::CalendarDetailPickerListCellCreator(void)
	: MAbstractCellCreator<MWidgetController>(),
	  m_cellSize(QSizeF(-1, -1))
{
	CalendarDetailCell *cell = new CalendarDetailCell;
	m_cellSize = cell->effectiveSizeHint(Qt::PreferredSize);
	delete cell;
}

QSizeF CalendarDetailPickerListCellCreator::cellSize(void) const
{
	return m_cellSize;
}

MWidget *CalendarDetailPickerListCellCreator::
createCell(const QModelIndex &index, 
	   MWidgetRecycler &recycler) const {

        CalendarDetailCell *cell = 
		dynamic_cast<CalendarDetailCell*>
		(recycler.take(CalendarDetailCell::staticMetaObject.className()));
        
        if (cell == NULL) {
		cell = new CalendarDetailCell();
		cell->setLayoutPosition(M::CenterPosition);
        }

        updateCell(index, cell);

        return cell;
}

void CalendarDetailPickerListCellCreator::updateCell(const QModelIndex &index, 
						    MWidget *cell) const
{
	CalendarDetailCell *detail = qobject_cast<CalendarDetailCell *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QStringList parameters = data.value<QStringList>();
	if (parameters.length() > 0) {
		detail->setTitle(parameters[0]);
	} else {
		detail->setTitle("missing-0");
	}
	if (parameters.length() > 1) {
		detail->setSubtitle(parameters[1]); 
	} else {
		detail->setTitle("missing-1");
	}
}
