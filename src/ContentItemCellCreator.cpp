/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContentItemCellCreator.h"

ContentItemCellCreator::ContentItemCellCreator(void)
	: MAbstractCellCreator<MWidgetController>(),
	  m_cellSize(QSizeF(-1, -1))
{
	MContentItem *cell = new MContentItem;
	m_cellSize = cell->effectiveSizeHint(Qt::PreferredSize);
	delete cell;
}

QSizeF ContentItemCellCreator::cellSize(void) const
{
	return m_cellSize;
}

MWidget *ContentItemCellCreator::createCell(const QModelIndex &index, 
					    MWidgetRecycler &recycler) const 
{
        MContentItem *cell = 
		dynamic_cast<MContentItem *>
		(recycler.take(MContentItem::staticMetaObject.className()));
        
        if (cell == NULL) {
		cell = new MContentItem();
		cell->setStyleName("CommonContentItem");
        }

        updateCell(index, cell);

        return cell;
}

