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

#include <QVariant>

TagListCellCreator::TagListCellCreator(MainPage *page)
	: MAbstractCellCreator<MContentItem>(),
	  m_page(page)
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

	const QNdefMessage message = tag->message();
	quint32 total = 0;
	for (int i = 0; i < message.length(); i++) {
		const QNdefRecord record = message[i];
		total += 
			1 + /* NDEF header */
			1 + /* Type length */
			(record.payload().length() < 256 ? 1 : 4) + /* Pllen */
			record.type().length() + /* type */
			record.payload().length(); /* pl */
	}
	contentItem->setSubtitle(QObject::tr("%1 bytes").arg(total));
}
