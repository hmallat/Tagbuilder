/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TagListCellCreator.h"
#include "Tag.h"

#include <QVariant>

static const QString _messageTypeToIcon(const QNdefMessage &message)
{
	QNdefRecord record = message.at(0);
	if (record.typeNameFormat() == QNdefRecord::NfcRtd) {
		if (record.type() == "T") {
			return "icon-m-content-text";
		} else if (record.type() == "U" ||
			   record.type() == "Sp") {
			return "icon-m-content-uri";
		} else {
			return "";
		}
	} else {
		return "";
	}
}

void TagListCellCreator::updateCell(const QModelIndex &index, 
				    MWidget *cell) const
{
	MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	Tag *tag = data.value<Tag *>();
	contentItem->setTitle(tag->name());
	contentItem->setImageID(_messageTypeToIcon(tag->message()));
}
