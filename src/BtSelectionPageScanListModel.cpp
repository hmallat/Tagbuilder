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
	connect(m_bluez, SIGNAL(bluezDeviceFound(QString)),
		this, SLOT(deviceFound(QString)));

	connect(m_bluez, SIGNAL(bluezDeviceLost(QString)),
		this, SLOT(deviceLost(QString)));

	QList< QPair<QString, QBluetoothDeviceInfo> > 
		scannedDevices = m_bluez->scannedDevices();
	for (int i = 0; i < scannedDevices.length(); i++) {
		m_device_ids << scannedDevices[i].first;
		m_devices[scannedDevices[i].first] = scannedDevices[i].second;
	}

	m_bluez->beginScan();
}

void BtSelectionPageScanListModel::deviceFound(QString which)
{
	for (int i = 0; i < m_device_ids.length(); i++) {
		if (m_device_ids[i] == which) {
			mDebug(__func__) 
				<< "Device found but already known, "
				<< "ignoring. ";
			return;
		}
	}

	QBluetoothDeviceInfo created = m_bluez->scannedDevice(which);
	if (created.isValid() == false) {
		mDebug(__func__) << "Device found but not found, ignoring. ";
		return;
	} 

	beginInsertRows(QModelIndex(), 
			m_device_ids.length(), 
			m_device_ids.length());
	m_device_ids << which;
	m_devices[which] = created;
	endInsertRows();
}

void BtSelectionPageScanListModel::deviceLost(QString which)
{
	for (int i = 0; i < m_device_ids.length(); i++) {
		if (m_device_ids[i] == which) {
			beginRemoveRows(QModelIndex(), i, i);
			m_devices.remove(which);
			m_device_ids.removeAt(i);
			endRemoveRows();
			return;
		}
	}

	mDebug(__func__) << "Device lost but not found, ignoring. ";
}
