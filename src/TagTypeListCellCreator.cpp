/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TagTypeListCellCreator.h"

#include <QVariant>
#include <MContentItem>

TagTypeListCellCreator::TagTypeListCellCreator(void)
	: ContentItemCellCreator(MContentItem::IconAndSingleTextLabel)
{
}

void TagTypeListCellCreator::updateCell(const QModelIndex &index, 
					MWidget *cell) const
{
	MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QStringList parameters = data.value<QStringList>();
	contentItem->setTitle(parameters[0]);
	contentItem->setImageID(parameters[1]);
}
