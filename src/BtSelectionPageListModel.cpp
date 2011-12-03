/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtSelectionPageListModel.h"

#include <QDBusObjectPath>
#include <QStringList>
#include <QBluetoothAddress>

QTM_USE_NAMESPACE;

BtSelectionPageListModel::BtSelectionPageListModel(BluezSupplicant *bluez,
						   QObject *parent)
	: QAbstractListModel(parent),
	  m_bluez(bluez)
{
}

int BtSelectionPageListModel::rowCount(const QModelIndex &parent) const
{
	(void) parent;
	return m_device_ids.length();
}

QVariant BtSelectionPageListModel::data(const QModelIndex &index, 
					int role) const
{
	(void) role;

	QDBusObjectPath path = m_device_ids[index.row()];
	QBluetoothDeviceInfo info = m_devices[path];
	QStringList parameters;
	parameters 
		<< info.name() 
		<< info.address().toString() 
		<< "icon-m-content-bluetooth" /* TODO */;
	return qVariantFromValue(parameters);
}

const QString BtSelectionPageListModel::name(const QModelIndex &index) const
{
	QDBusObjectPath path = m_device_ids[index.row()];
	QBluetoothDeviceInfo info = m_devices[path];
	return info.name();
}

const QString BtSelectionPageListModel::icon(const QModelIndex &index) const
{
	(void)index;
	return "icon-m-content-bluetooth" /* TODO */;
}

QBluetoothDeviceInfo BtSelectionPageListModel::device(const QModelIndex &index) const
{
	QDBusObjectPath path = m_device_ids[index.row()];
	return m_devices[path];
}
