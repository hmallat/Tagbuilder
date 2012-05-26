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

#include "BtSelectionPageScanListModel.h"
#include "BluezSupplicant.h"
#include "Util.h"

#include <QStringList>
#include <QBluetoothAddress>

#include <MDebug>

BtSelectionPageScanListModel::
BtSelectionPageScanListModel(BluezSupplicant *bluez, QObject *parent)
	: BtSelectionPageListModel(bluez, parent)
{
	if (m_bluez->isInitialized() == false) {
		connect(m_bluez, SIGNAL(initialized(void)),
			this, SLOT(initialized(void)));
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

	connect(m_bluez, 
		SIGNAL(discoveryStateChanged(enum BluezSupplicant::DiscoveryState)),
		this,
		SLOT(discoveryStateChanged(enum BluezSupplicant::DiscoveryState)));

	if (m_bluez->beginScan() == false) {
		Q_EMIT(scanFailure());
	}
}

void BtSelectionPageScanListModel::discoveryStateChanged(enum BluezSupplicant::DiscoveryState state)
{
	/* Since we only stop scan when going away, getting a state
	   change to ScanEnding or NotScanning is an error. */
	if (state == BluezSupplicant::NotScanning ||
	    state == BluezSupplicant::ScanEnding) {
		Q_EMIT(scanFailure());
	}
}

void BtSelectionPageScanListModel::deviceFound(QString which)
{
	mDebug(__func__) << "Scanned device " << which << " found. ";

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
			m_device_ids.length(),
			Util::animateLists);
	m_device_ids << which;
	m_devices[which] = created;
	endInsertRows();
}

void BtSelectionPageScanListModel::deviceLost(QString which)
{
	mDebug(__func__) << "Scanned device " << which << " lost. ";

	for (int i = 0; i < m_device_ids.length(); i++) {
		if (m_device_ids[i] == which) {
		  beginRemoveRows(QModelIndex(), i, i, Util::animateLists);
			m_devices.remove(which);
			m_device_ids.removeAt(i);
			endRemoveRows();
			return;
		}
	}

	mDebug(__func__) << "Device lost but not found, ignoring. ";
}
