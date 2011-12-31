/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BookmarkSelectionPageListCellCreator.h"

#include <QVariant>
#include <MContentItem>
#include <QImage>

void BookmarkSelectionPageListCellCreator::updateCell(const QModelIndex &index, 
						      MWidget *cell) const
{
	MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QStringList parameters = data.value<QStringList>();

	contentItem->setTitle(parameters[0]);
	contentItem->setSubtitle(parameters[1]);
	/* TODO: any proper way of determining this? */
	/* TODO: how slow is image loading? should it be asynch? */
	if (parameters[2].startsWith("/") == true) {
		QImage img(parameters[2]);
		contentItem->setImage(img);
	} else {
		contentItem->setImageID(parameters[2]); 
	}
}
