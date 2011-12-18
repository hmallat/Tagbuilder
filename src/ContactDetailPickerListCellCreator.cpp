/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactDetailPickerListCellCreator.h"
#include "ContactDetailCell.h"

#include <QVariant>

ContactDetailPickerListCellCreator::ContactDetailPickerListCellCreator(void)
	: MAbstractCellCreator<MWidgetController>(),
	  m_cellSize(QSizeF(-1, -1))
{
	ContactDetailCell *cell = new ContactDetailCell;
	m_cellSize = cell->effectiveSizeHint(Qt::PreferredSize);
	delete cell;
}

QSizeF ContactDetailPickerListCellCreator::cellSize(void) const
{
	return m_cellSize;
}

MWidget *ContactDetailPickerListCellCreator::
createCell(const QModelIndex &index, 
	   MWidgetRecycler &recycler) const {

        ContactDetailCell *cell = 
		dynamic_cast<ContactDetailCell*>
		(recycler.take(ContactDetailCell::staticMetaObject.className()));
        
        if (cell == NULL) {
		cell = new ContactDetailCell();
		cell->setLayoutPosition(M::CenterPosition);
        }

        updateCell(index, cell);

        return cell;
}

void ContactDetailPickerListCellCreator::updateCell(const QModelIndex &index, 
						    MWidget *cell) const
{
	ContactDetailCell *detail = qobject_cast<ContactDetailCell *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QStringList parameters = data.value<QStringList>();
	detail->setTitle(parameters[0]);
	detail->setSubtitle(parameters[1]); 
}
