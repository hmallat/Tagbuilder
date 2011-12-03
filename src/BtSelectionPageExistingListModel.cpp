/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtSelectionPageExistingListModel.h"
#include "BluezSupplicant.h"

#include <QStringList>
#include <QBluetoothAddress>

#include <MDebug>

/* TODO: any and all updating of the model after initial construction */

BtSelectionPageExistingListModel::
BtSelectionPageExistingListModel(BluezSupplicant *bluez, QObject *parent)
	: BtSelectionPageListModel(parent),
	  m_bluez(bluez)
{
	connect(m_bluez, SIGNAL(bluezDeviceCreated(QDBusObjectPath)),
		this, SLOT(deviceCreated(QDBusObjectPath)));

	connect(m_bluez, SIGNAL(bluezDeviceRemoved(QDBusObjectPath)),
		this, SLOT(deviceRemoved(QDBusObjectPath)));

	connect(m_bluez, SIGNAL(bluezDeviceUpdated(QDBusObjectPath)),
		this, SLOT(deviceUpdated(QDBusObjectPath)));

	m_devices = bluez->devices();
}

void BtSelectionPageExistingListModel::deviceCreated(QDBusObjectPath which)
{
	(void) which;
	/* TODO */
	mDebug(__func__) << "device created, should update. ";
}

void BtSelectionPageExistingListModel::deviceRemoved(QDBusObjectPath which)
{
	(void) which;
	/* TODO */
	mDebug(__func__) << "device removed, should update. ";
}

void BtSelectionPageExistingListModel::deviceUpdated(QDBusObjectPath which)
{
	(void) which;
	/* TODO */
	mDebug(__func__) << "device changed, should update. ";
}

int BtSelectionPageExistingListModel::modelRowCount(const QModelIndex &parent) const
{
	(void) parent;
	return m_devices.length();
}

QVariant BtSelectionPageExistingListModel::indexData(const QModelIndex &index,
						     int role) const 
{
	(void) role;

	QBluetoothDeviceInfo info = m_devices[index.row()];
	QStringList parameters;
	parameters 
		<< info.name() 
		<< info.address().toString() 
		<< "icon-m-content-bluetooth" /* TODO */;
	return qVariantFromValue(parameters);
}

QString BtSelectionPageExistingListModel::indexName(const QModelIndex &index) const 
{
	QBluetoothDeviceInfo info = m_devices[index.row()];
	return info.name();
}

QString BtSelectionPageExistingListModel::indexIcon(const QModelIndex &index) const 
{
	(void)index;

	return "icon-m-content-bluetooth" /* TODO */;
}

QBluetoothDeviceInfo BtSelectionPageExistingListModel::device(const QModelIndex &index) const
{
	return m_devices[index.row()];
}
