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

TagListCellCreator::TagListCellCreator(void)
	: MAbstractCellCreator<MContentItem>()
{
}

void TagListCellCreator::updateCell(const QModelIndex &index, 
				    MWidget *cell) const
{
	MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	const Tag *tag = data.value<const Tag *>();
	contentItem->setTitle(tag->name());
	contentItem->setImageID(Tag::icon(tag->type()));

	quint32 total = Util::messageLength(tag->message());
	contentItem->setSubtitle(QObject::tr("%1 bytes").arg(total));
}
