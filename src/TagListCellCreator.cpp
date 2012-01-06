/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TagListCellCreator.h"
#include "Tag.h"
#include "MainPage.h"
#include "Util.h"

#include <QVariant>

void TagListCellCreator::updateCell(const QModelIndex &index, 
				    MWidget *cell) const
{
	QVariant data = index.data(Qt::DisplayRole);
	const Tag *tag = data.value<const Tag *>();
	quint32 total = Util::messageLength(tag->message());

	MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
	contentItem->setTitle(tag->name());
	contentItem->setImageID(Tag::icon(tag->type()));
	contentItem->setSubtitle(QObject::tr("%1 bytes").arg(total));
}
