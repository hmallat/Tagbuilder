/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtSelectionPageScanListModel.h"
#include "BluezSupplicant.h"

#include <QStringList>
#include <QBluetoothAddress>

#include <MDebug>

BtSelectionPageScanListModel::
BtSelectionPageScanListModel(BluezSupplicant *bluez, QObject *parent)
	: BtSelectionPageListModel(bluez, parent)
{
	if (m_bluez->isInitialized() == false) {
		connect(m_bluez, SIGNAL(initialized(void)),
			this, SLOT(bluezInitialized(void)));
	} else {
		initialized();
	}

}

BtSelectionPageScanListModel::~BtSelectionPageScanListModel(void)
{
	m_bluez->endScan();
}

void BtSelectionPageScanListModel::initialized(void)
{
	connect(m_bluez, SIGNAL(bluezDeviceCreated(QDBusObjectPath)),
		this, SLOT(deviceFound(QDBusObjectPath)));

	connect(m_bluez, SIGNAL(bluezDeviceRemoved(QDBusObjectPath)),
		this, SLOT(deviceLost(QDBusObjectPath)));

#if 0
	QList< QPair<QDBusObjectPath, QBluetoothDeviceInfo> > 
		devices = m_bluez->scannedDevices();
	for (int i = 0; i < devices.length(); i++) {
		m_device_ids << devices[i].first;
		m_devices[devices[i].first] = devices[i].second;
	}
#endif

	m_bluez->beginScan();
}

void BtSelectionPageScanListModel::deviceFound(QDBusObjectPath which)
{
	(void) which;
	/* TODO */
}

void BtSelectionPageScanListModel::deviceLost(QDBusObjectPath which)
{
	(void) which;
	/* TODO */
}

