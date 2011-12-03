/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtSelectionPageExistingListModel.h"
#include "BluezSupplicant.h"
#include "BluezDevice.h"

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

	QList<const BluezDevice *> devices = bluez->devices();
	for (int i = 0; i < devices.length(); i++) {
		m_device_ids << devices[i]->path();
		m_devices[devices[i]->path()] =
			devices[i]->toBluetoothDeviceInfo();
	}

}

void BtSelectionPageExistingListModel::deviceCreated(QDBusObjectPath which)
{
	for (int i = 0; i < m_device_ids.length(); i++) {
		if (m_device_ids[i] == which) {
			mDebug(__func__) 
				<< "Device created but already known, "
				<< "ignoring. ";
			return;
		}
	}

	const BluezDevice *created = m_bluez->device(which);
	if (created == 0) {
		mDebug(__func__) << "Device created but not found, ignoring. ";
		return;
	} 

	beginInsertRows(QModelIndex(), 
			m_device_ids.length(), 
			m_device_ids.length());
	m_device_ids << which;
	m_devices[which] = created->toBluetoothDeviceInfo();
	endInsertRows();
}

void BtSelectionPageExistingListModel::deviceRemoved(QDBusObjectPath which)
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

	mDebug(__func__) << "Device removed but not found, ignoring. ";
}

void BtSelectionPageExistingListModel::deviceUpdated(QDBusObjectPath which)
{
	const BluezDevice *changed = m_bluez->device(which);
	if (changed == 0) {
		mDebug(__func__) << "Device changed but not found, ignoring. ";
		return;
	} 

	for (int i = 0; i < m_device_ids.length(); i++) {
		if (m_device_ids[i] == which) {
			m_devices[which] = changed->toBluetoothDeviceInfo();
			dataChanged(createIndex(i, 0), createIndex(i, 0));
			return;
		}
	}
}

int BtSelectionPageExistingListModel::modelRowCount(const QModelIndex &parent) const
{
	(void) parent;
	return m_device_ids.length();
}

QVariant BtSelectionPageExistingListModel::indexData(const QModelIndex &index,
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

QString BtSelectionPageExistingListModel::indexName(const QModelIndex &index) const 
{
	QDBusObjectPath path = m_device_ids[index.row()];
	QBluetoothDeviceInfo info = m_devices[path];
	return info.name();
}

QString BtSelectionPageExistingListModel::indexIcon(const QModelIndex &index) const 
{
	(void)index;

	return "icon-m-content-bluetooth" /* TODO */;
}

QBluetoothDeviceInfo BtSelectionPageExistingListModel::device(const QModelIndex &index) const
{
	QDBusObjectPath path = m_device_ids[index.row()];
	return m_devices[path];
}
