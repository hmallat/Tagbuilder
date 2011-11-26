/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TagListModel.h"
#include "TagStorage.h"
#include "Tag.h"

TagListModel::TagListModel(QObject *parent)
	: QAbstractListModel(parent)
{
}

int TagListModel::rowCount(const QModelIndex &parent) const
{
	(void) parent;
	return TagStorage::storedTags().length();
}

QVariant TagListModel::data(const QModelIndex &index, int role) const
{
	(void) role;
	
	if (index.isValid() == false || 
	    index.row() >= TagStorage::storedTags().length()) {
		return QVariant();
	}

	Tag *tag = TagStorage::storedTags().at(index.row());
	return qVariantFromValue(tag);
}

