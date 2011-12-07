/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TagTypeListModel.h"
#include "Tag.h"

#include <QStringList>

/* Not yet supported */
/* #define WLAN */

TagTypeListModel::TagTypeListModel(QObject *parent)
	: QAbstractListModel(parent)
{
}

int TagTypeListModel::rowCount(const QModelIndex &parent) const
{
	(void) parent;
#ifdef WLAN
	return 6;
#else
	return 5;
#endif
}

QVariant TagTypeListModel::data(const QModelIndex &index, int role) const
{
	(void) role;
	
	QStringList parameters;
	parameters << name(index) << icon(index);
	return qVariantFromValue(parameters);
}

const QString TagTypeListModel::name(const QModelIndex &index) const
{
	if (index.isValid() == false) {
		return "";
	}

	switch (index.row()) {
	case 0: return Tag::BLUETOOTH_TAG;
	case 1: return Tag::URL_TAG;
	case 2: return Tag::CALENDAR_TAG;
	case 3: return Tag::CONTACT_TAG;
	case 4: return Tag::TEXT_TAG;
#ifdef WLAN
	case 5: return Tag::WLAN_TAG;
#endif
	default: return "";
	};

}

const QString TagTypeListModel::icon(const QModelIndex &index) const
{
	if (index.isValid() == false) {
		return "";
	}

	return Tag::icon(name(index));
}


