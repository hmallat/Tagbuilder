/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CONTENT_ITEM_CELL_CREATOR_H_
#define _CONTENT_ITEM_CELL_CREATOR_H_

#include <MAbstractCellCreator>
#include <MContentItem>

class ContentItemCellCreator : public MAbstractCellCreator<MWidgetController>
{

public:

	ContentItemCellCreator(MContentItem::ContentItemStyle style = MContentItem::IconAndTwoTextLabels);

	virtual MWidget *createCell(const QModelIndex &index, 
				    MWidgetRecycler &recycler) const;

	virtual QSizeF cellSize(void) const;

private:

	QSizeF m_cellSize;

	MContentItem::ContentItemStyle m_style;

};

#endif /* _CONTENT_ITEM_CELL_CREATOR_H_ */
