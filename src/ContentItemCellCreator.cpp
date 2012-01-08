/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContentItemCellCreator.h"

ContentItemCellCreator::ContentItemCellCreator(MContentItem::ContentItemStyle style,
					       const QString styleName)
	: MAbstractCellCreator<MWidgetController>(),
	  m_cellSize(QSizeF(-1, -1)),
	  m_style(style),
	  m_styleName(styleName)
{
  MContentItem *cell = new MContentItem(m_style);
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
		cell = new MContentItem(m_style);
		cell->setStyleName(m_styleName);
        }

        updateCell(index, cell);

        return cell;
}

