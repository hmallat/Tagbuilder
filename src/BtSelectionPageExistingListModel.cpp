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

BtSelectionPageExistingListModel::
BtSelectionPageExistingListModel(BluezSupplicant *bluez, QObject *parent)
	: BtSelectionPageListModel(bluez, parent)
{
	if (m_bluez->isInitialized() == false) {
		connect(m_bluez, SIGNAL(initialized(void)),
			this, SLOT(initialized(void)));
	} else {
		initialized();
	}
}

void BtSelectionPageExistingListModel::initialized(void)
{
	connect(m_bluez, SIGNAL(bluezDeviceCreated(QDBusObjectPath)),
		this, SLOT(deviceCreated(QDBusObjectPath)));

	connect(m_bluez, SIGNAL(bluezDeviceRemoved(QDBusObjectPath)),
		this, SLOT(deviceRemoved(QDBusObjectPath)));

	connect(m_bluez, SIGNAL(bluezDeviceUpdated(QDBusObjectPath)),
		this, SLOT(deviceUpdated(QDBusObjectPath)));

	QList< QPair<QDBusObjectPath, QBluetoothDeviceInfo> > 
		devices = m_bluez->devices();
	for (int i = 0; i < devices.length(); i++) {
		m_device_ids << devices[i].first.path();
		m_devices[devices[i].first.path()] = devices[i].second;
	}
}

void BtSelectionPageExistingListModel::deviceCreated(QDBusObjectPath which)
{
	for (int i = 0; i < m_device_ids.length(); i++) {
		if (m_device_ids[i] == which.path()) {
			mDebug(__func__) 
				<< "Device created but already known, "
				<< "ignoring. ";
			return;
		}
	}

	QBluetoothDeviceInfo created = m_bluez->device(which);
	if (created.isValid() == false) {
		mDebug(__func__) << "Device created but not found, ignoring. ";
		return;
	} 

	beginInsertRows(QModelIndex(), 
			m_device_ids.length(), 
			m_device_ids.length());
	m_device_ids << which.path();
	m_devices[which.path()] = created;
	endInsertRows();
}

void BtSelectionPageExistingListModel::deviceRemoved(QDBusObjectPath which)
{
	for (int i = 0; i < m_device_ids.length(); i++) {
		if (m_device_ids[i] == which.path()) {
			beginRemoveRows(QModelIndex(), i, i);
			m_devices.remove(which.path());
			m_device_ids.removeAt(i);
			endRemoveRows();
			return;
		}
	}

	mDebug(__func__) << "Device removed but not found, ignoring. ";
}

void BtSelectionPageExistingListModel::deviceUpdated(QDBusObjectPath which)
{
	QBluetoothDeviceInfo changed = m_bluez->device(which);
	if (changed.isValid() == false) {
		mDebug(__func__) << "Device changed but not found, ignoring. ";
		return;
	} 

	for (int i = 0; i < m_device_ids.length(); i++) {
		if (m_device_ids[i] == which.path()) {
			m_devices[which.path()] = changed;
			dataChanged(createIndex(i, 0), createIndex(i, 0));
			return;
		}
	}
}
