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

/* The Qt Connectivity interface doesn't provide all functionality I need,
   hence the BlueZ supplicant for querying certain data, such as known
   BT devices. */

#ifndef _BLUEZ_SUPPLICANT_H_
#define _BLUEZ_SUPPLICANT_H_

#include <QObject>
#include <QPair>
#include <QDBusObjectPath>
#include <QDBusConnection>
#include <QDBusVariant>
#include <QBluetoothDeviceInfo>

class BluezDevice;
class QDBusInterface;
class QDBusPendingCallWatcher;

QTM_USE_NAMESPACE;

class BluezSupplicant : public QObject
{

	Q_OBJECT;

public:

	enum DiscoveryState {
		NotScanning,
		ScanStarting,
		Scanning,
		ScanEnding
	};

	BluezSupplicant(QObject *parent = 0);

	~BluezSupplicant(void);

	void start(void);

	bool isInitialized(void) const;

	bool isAvailable(void) const;

	QBluetoothDeviceInfo device(QDBusObjectPath which) const;
	
	QList< QPair<QDBusObjectPath, QBluetoothDeviceInfo> > 
		devices(void) const;

	bool beginScan(void);

	bool endScan(void);

	enum DiscoveryState discoveryState(void);

	QBluetoothDeviceInfo scannedDevice(QString which) const;
	
	QList< QPair<QString, QBluetoothDeviceInfo> > 
		scannedDevices(void) const;

signals:

	void initialized(void);

	void bluezAdapterChanged(QDBusObjectPath which);

	void bluezAdapterRemoved(QDBusObjectPath which);

	void bluezDeviceCreated(QDBusObjectPath which);

	void bluezDeviceRemoved(QDBusObjectPath which);

	void bluezDeviceUpdated(QDBusObjectPath which);

	void bluezDeviceFound(QString which);

	void bluezDeviceLost(QString which);

	void discoveryStateChanged(enum BluezSupplicant::DiscoveryState what);

private Q_SLOTS:

	void defaultAdapterDone(QDBusPendingCallWatcher *watcher);

	void adapterChanged(const QDBusObjectPath which);

	void adapterRemoved(const QDBusObjectPath which);

	void listDevicesDone(QDBusPendingCallWatcher *watcher);

	void deviceCreated(const QDBusObjectPath which);

	void deviceRemoved(const QDBusObjectPath which);

	void deviceUpdated(const QDBusObjectPath which);

	void deviceFound(const QString, const QMap<QString, QVariant>);

	void deviceLost(const QString);

	void sessionRequested(QDBusPendingCallWatcher *watcher);

	void discoveryStarted(QDBusPendingCallWatcher *watcher);

	void discoveryEnded(QDBusPendingCallWatcher *watcher);

private:

	Q_DISABLE_COPY(BluezSupplicant);

	void callPending(void);

	void callFinished(void);

	void changeDiscoveryState(enum DiscoveryState newState);

	bool m_started;

	enum DiscoveryState m_scanning;

	bool m_initialized;

	int m_pendingCalls;

	QDBusConnection m_sys;

	QDBusInterface *m_manager;

	QDBusInterface *m_adapter;

	QList<BluezDevice *> m_devices;

	QList<BluezDevice *> m_scannedDevices;

};

#endif /* _BLUEZ_SUPPLICANT_H_ */
