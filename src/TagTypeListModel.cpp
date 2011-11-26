/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TagTypeListModel.h"

#include <QStringList>

const QString TagTypeListModel::BLUETOOTH_TAG = tr("Bluetooth Device");
const QString TagTypeListModel::URL_TAG = tr("Bookmark");
const QString TagTypeListModel::CALENDAR_TAG = tr("Calendar Entry");
const QString TagTypeListModel::CONTACT_TAG = tr("Contact");
const QString TagTypeListModel::TEXT_TAG = tr("Text");
const QString TagTypeListModel::WLAN_TAG = tr("Wireless Network");

#define TYPES 6

static const QString names[TYPES] = {
	TagTypeListModel::BLUETOOTH_TAG,
	TagTypeListModel::URL_TAG,
	TagTypeListModel::CALENDAR_TAG,
	TagTypeListModel::CONTACT_TAG,
	TagTypeListModel::TEXT_TAG,
	TagTypeListModel::WLAN_TAG
};

static const QString icons[TYPES] = {
	"icon-m-content-bluetooth", 
	"icon-m-content-url", 
	"icon-m-content-calendar", 
	"icon-m-content-avatar-placeholder", 
	"icon-m-content-text", 
	"icon-m-content-wlan" 
};

TagTypeListModel::TagTypeListModel(QObject *parent)
	: QAbstractListModel(parent)
{
}

int TagTypeListModel::rowCount(const QModelIndex &parent) const
{
	(void) parent;
	return TYPES;
}

QVariant TagTypeListModel::data(const QModelIndex &index, int role) const
{
	(void) role;
	
	if (index.isValid() == false || 
	    index.row() >= TYPES) {
		return QVariant();
	}

	QStringList parameters;
	parameters << names[index.row()] << icons[index.row()];
	return qVariantFromValue(parameters);
}

const QString TagTypeListModel::name(const QModelIndex &index) const
{
	if (index.isValid() == false || 
	    index.row() >= TYPES) {
		return "";
	}

	return names[index.row()];
}

const QString TagTypeListModel::icon(const QModelIndex &index) const
{
	if (index.isValid() == false || 
	    index.row() >= TYPES) {
		return "";
	}

	return icons[index.row()];
}


