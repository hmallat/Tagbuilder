/*

    NFC Tag Builder for Nokia N9
    Copyright (C) 2011,2012  Hannu Mallat <hmallat@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "BtSelectionPageExistingListModel.h"
#include "BluezSupplicant.h"
#include "Util.h"

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
		m_ready = false;
	} else {
		initialized();
		m_ready = true;
	}
}

bool BtSelectionPageExistingListModel::isReady(void)
{
	return m_ready;
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
			m_device_ids.length(),
			Util::animateLists);
	m_device_ids << which.path();
	m_devices[which.path()] = created;
	endInsertRows();
}

void BtSelectionPageExistingListModel::deviceRemoved(QDBusObjectPath which)
{
	for (int i = 0; i < m_device_ids.length(); i++) {
		if (m_device_ids[i] == which.path()) {
		  beginRemoveRows(QModelIndex(), i, i, Util::animateLists);
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
