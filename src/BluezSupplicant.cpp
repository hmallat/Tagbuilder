/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BluezSupplicant.h"
#include "BluezDevice.h"

extern "C"
{
#include <assert.h>
}

#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>
#include <QBluetoothAddress>

#include <MDebug>

/*
 * When started, queries for the default adapter and fills in the list
 * of known devices; populates device structures with attributes so
 * that Qt bluetooth device info is available. Monitors for changes in
 * any and all to keep the structures up to date. 
 */

/* TODO: get enough device props to construct qt bt device infos */
/* TODO: emit signals for device list changes after initialization */

BluezSupplicant::BluezSupplicant(QObject *parent)
	: QObject(parent),
	  m_started(false),
	  m_scanning(false),
	  m_initialized(false),
	  m_pendingCalls(0),
	  m_sys(QDBusConnection::systemBus()),
	  m_manager(new QDBusInterface("org.bluez",
				       "/",
				       "org.bluez.Manager",
				       m_sys,
				       this)),
	  m_adapter(0),
	  m_devices(),
	  m_scannedDevices()
{
}

BluezSupplicant::~BluezSupplicant(void)
{
}

void BluezSupplicant::callPending(void)
{
	m_pendingCalls++;
}

void BluezSupplicant::callFinished(void)
{
	m_pendingCalls--;
	if (m_initialized == false && m_pendingCalls == 0) {
		m_initialized = true;
		Q_EMIT(initialized());
	}
}

bool BluezSupplicant::isInitialized(void) const
{
	return m_initialized;
}

void BluezSupplicant::start(void)
{
	if (m_started == true) {
		mDebug(__func__) << "Already started. ";
		return;
	}

	mDebug(__func__) << "Starting. ";

	m_started = true;

	m_sys.connect("org.bluez",
		      "/",
		      "org.bluez.Manager",
		      "DefaultAdapterChanged",
		      this,
		      SLOT(adapterChanged(const QDBusObjectPath)));
	
	m_sys.connect("org.bluez",
		      "/",
		      "org.bluez.Manager",
		      "AdapterRemoved",
		      this,
		      SLOT(adapterRemoved(const QDBusObjectPath)));
	
	callPending();

	QDBusPendingCall call =
		m_manager->asyncCall("DefaultAdapter");
	QDBusPendingCallWatcher *watcher =
		new QDBusPendingCallWatcher(call, this);
	connect(watcher, 
		SIGNAL(finished(QDBusPendingCallWatcher *)),
		this, 
		SLOT(defaultAdapterDone(QDBusPendingCallWatcher *)));
}

void BluezSupplicant::defaultAdapterDone(QDBusPendingCallWatcher *watcher)
{
	mDebug(__func__) << "ENTER";

	QDBusPendingReply<QDBusObjectPath> reply = *watcher;
	if (reply.isError()) {
		mDebug(__func__) << "Failed to get default adapter. ";
	} else {
		adapterChanged(reply.argumentAt<0>());
	}

	watcher->deleteLater();

	callFinished();
}

void BluezSupplicant::adapterChanged(const QDBusObjectPath which)
{
	mDebug(__func__) << "ENTER";

	if (m_adapter != 0 && which.path() == m_adapter->path()) {
		mDebug(__func__) 
			<< "Adapter " << which.path() << " "
			<< "already known, not changing. ";
		return;
	}

	if (m_adapter != 0) {
		adapterRemoved(QDBusObjectPath(m_adapter->path()));
	}

	assert (m_adapter == 0);
	assert (m_devices.length() == 0);

	m_adapter = new QDBusInterface("org.bluez",
				       which.path(),
				       "org.bluez.Adapter",
				       m_sys,
				       this);

	m_sys.connect("org.bluez",
		      which.path(),
		      "org.bluez.Adapter",
		      "DeviceCreated",
		      this,
		      SLOT(deviceCreated(const QDBusObjectPath)));
	
	m_sys.connect("org.bluez",
		      which.path(),
		      "org.bluez.Adapter",
		      "DeviceRemoved",
		      this,
		      SLOT(deviceRemoved(const QDBusObjectPath)));
	
	callPending();

	QDBusPendingCall call =
		m_adapter->asyncCall("ListDevices");
	QDBusPendingCallWatcher *watcher =
		new QDBusPendingCallWatcher(call, this);
	connect(watcher, 
		SIGNAL(finished(QDBusPendingCallWatcher *)),
		this, 
		SLOT(listDevicesDone(QDBusPendingCallWatcher *)));

	mDebug(__func__) << "Adapter " << which.path() << " changed. ";
	Q_EMIT(bluezAdapterChanged(which));

}

void BluezSupplicant::adapterRemoved(const QDBusObjectPath which)
{
	mDebug(__func__) << "ENTER";

	if (m_adapter == 0 || m_adapter->path() != which.path()) {
		mDebug(__func__) 
			<< "Adapter " << which.path() << " "
			<< "not known, not removing. ";
		return;
	}

	/* Stop listening to this adapter's signals */
	m_sys.disconnect("org.bluez",
			 which.path(),
			 "org.bluez.Adapter",
			 "DeviceCreated",
			 this,
			 SLOT(deviceCreated(const QDBusObjectPath)));
	
	m_sys.disconnect("org.bluez",
			 which.path(),
			 "org.bluez.Adapter",
			 "DeviceRemoved",
			 this,
			 SLOT(deviceRemoved(const QDBusObjectPath)));
	
	/* Remove all devices associated with the path */
	while (m_devices.length() != 0) {
		deviceRemoved(m_devices[0]->path());
	}

	delete m_adapter;
	m_adapter = 0;
	
	mDebug(__func__) << "Adapter " << which.path() << " removed. ";
	Q_EMIT(bluezAdapterRemoved(which));
	
}

void BluezSupplicant::listDevicesDone(QDBusPendingCallWatcher *watcher)
{
	mDebug(__func__) << "ENTER";

	QDBusPendingReply< QList<QDBusObjectPath> > reply = *watcher;
	if (reply.isError()) {
		mDebug(__func__) << "Failed to get device list. ";
	} else {
		QList<QDBusObjectPath> paths = reply.argumentAt<0>();
		mDebug(__func__) << paths.length() << " devices found. ";
		for (int i = 0; i < paths.length(); i++) {
			deviceCreated(paths[i]);
		}
	}

	watcher->deleteLater();

	callFinished();
}

void BluezSupplicant::deviceCreated(const QDBusObjectPath which)
{
	mDebug(__func__) << "ENTER";

	for (int i = 0; i < m_devices.length(); i++) {
		if (m_devices[i]->path().path() == which.path()) {
			mDebug(__func__) 
				<< "Device " << which.path() << " "
				<< "already known, not adding twice. ";
			return;
		}
	}

	BluezDevice *device = new BluezDevice(which, this);
	m_devices << device;

	connect(device, SIGNAL(deviceUpdated(QDBusObjectPath)),
		this, SLOT(deviceUpdated(QDBusObjectPath)));

	mDebug(__func__) << "Device " << which.path() << " added. ";
	Q_EMIT(bluezDeviceCreated(which));

}

void BluezSupplicant::deviceRemoved(const QDBusObjectPath which)
{
	mDebug(__func__) << "ENTER";

	int i;

	for (i = 0; i < m_devices.length(); i++) {
		if (m_devices[i]->path().path() == which.path()) {
			break;
		}
	}

	if (i == m_devices.length()) {
		mDebug(__func__) 
			<< "Device " << which.path() << " "
			<< "not known, not removing. ";
		return;
	}

	/* Remove listening to signals for this device */
	m_sys.disconnect("org.bluez",
			 which.path(),
			 "org.bluez.Device",
			 "PropertyChanged",
			 this,
			 SLOT(devicePropertyChanged(QString, 
						    QDBusVariant)));
	
	/* Finally, kill the device itself */
	BluezDevice *device = m_devices[i];
	m_devices.removeAt(i);
	delete device;
	
	mDebug(__func__) << "Device " << which.path() << " removed. ";
	Q_EMIT(bluezDeviceRemoved(which));
	
}

void BluezSupplicant::deviceUpdated(const QDBusObjectPath which)
{
	Q_EMIT(bluezDeviceUpdated(which));
}

QBluetoothDeviceInfo BluezSupplicant::device(QDBusObjectPath which) const
{
	for (int i = 0; i < m_devices.length(); i++) {
		if (m_devices[i]->path().path() == which.path()) {
			return m_devices[i]->toBluetoothDeviceInfo();
		}
	}

	return QBluetoothDeviceInfo();
}

QList< QPair<QDBusObjectPath, QBluetoothDeviceInfo> > 
BluezSupplicant::devices(void) const
{
	QList< QPair<QDBusObjectPath, QBluetoothDeviceInfo> > list;

	for (int i = 0; i < m_devices.length(); i++) {
		QPair<QDBusObjectPath, QBluetoothDeviceInfo> 
			pair(m_devices[i]->path(), 
			     m_devices[i]->toBluetoothDeviceInfo());
		list << pair;
	}

	return list;
}

bool BluezSupplicant::beginScan(void)
{
	if (isInitialized() == false) {
		mDebug("Not yet initialized. ");
		return false;
	}

	if (m_scanning == true) {
		mDebug(__func__) << "Already scanning. ";
		return true;
	}

	m_sys.connect("org.bluez",
		      m_adapter->path(),
		      "org.bluez.Adapter",
		      "DeviceFound",
		      this,
		      SLOT(deviceFound(const QString, 
				       const QMap<QString, QVariant>)));
	
	m_sys.connect("org.bluez",
		      m_adapter->path(),
		      "org.bluez.Adapter",
		      "DeviceLost",
		      this,
		      SLOT(deviceRemoved(const QString)));
	
	m_adapter->asyncCall("StartDiscovery");

	m_scanning = true;

	return true;
}

void BluezSupplicant::endScan(void)
{
	if (isInitialized() == false) {
		mDebug("Not yet initialized. ");
		return;
	}

	if (m_scanning == false) {
		mDebug(__func__) << "Already not scanning. ";
		return;
	}

	m_sys.disconnect("org.bluez",
			 m_adapter->path(),
			 "org.bluez.Adapter",
			 "DeviceFound",
			 this,
			 SLOT(deviceFound(const QString, 
					  const QMap<QString, QVariant>)));
	
	m_sys.disconnect("org.bluez",
			 m_adapter->path(),
			 "org.bluez.Adapter",
			 "DeviceLost",
			 this,
			 SLOT(deviceRemoved(const QString)));
	
	m_adapter->asyncCall("StopDiscovery");

	while (m_scannedDevices.length() != 0) {
		BluezDevice *device = m_scannedDevices.takeFirst();
		delete device;
	}

	m_scanning = false;
}

void BluezSupplicant::deviceFound(const QString address, 
				  const QMap<QString, QVariant> properties)
{
	mDebug(__func__) << "ENTER";

	for (int i = 0; i < m_scannedDevices.length(); i++) {
		if (m_scannedDevices[i]->address() == address) {
			mDebug(__func__) 
				<< "Device " << address << " "
				<< "already known, not adding twice. ";
			return;
		}
	}

	BluezDevice *device = new BluezDevice(address, properties, this);
	m_scannedDevices << device;

	mDebug(__func__) << "Device " << address << " added. ";
	Q_EMIT(bluezDeviceFound(address));
}

void BluezSupplicant::deviceLost(const QString address)
{
	mDebug(__func__) << "ENTER";

	int i;

	for (i = 0; i < m_scannedDevices.length(); i++) {
		if (m_scannedDevices[i]->address() == address) {
			break;
		}
	}

	if (i == m_scannedDevices.length()) {
		mDebug(__func__) 
			<< "Device " << address << " "
			<< "not known, not removing. ";
		return;
	}

	BluezDevice *device = m_scannedDevices[i];
	m_scannedDevices.removeAt(i);
	delete device;
	
	mDebug(__func__) << "Device " << address << " removed. ";
	Q_EMIT(bluezDeviceLost(address));
}

QBluetoothDeviceInfo BluezSupplicant::scannedDevice(QString which) const
{
	for (int i = 0; i < m_scannedDevices.length(); i++) {
		if (m_scannedDevices[i]->address() == which) {
			return m_scannedDevices[i]->toBluetoothDeviceInfo();
		}
	}

	return QBluetoothDeviceInfo();
}

QList< QPair<QString, QBluetoothDeviceInfo> > 
BluezSupplicant::scannedDevices(void) const
{
	QList< QPair<QString, QBluetoothDeviceInfo> > list;

	for (int i = 0; i < m_scannedDevices.length(); i++) {
		QPair<QString, QBluetoothDeviceInfo> 
			pair(m_scannedDevices[i]->address(), 
			     m_scannedDevices[i]->toBluetoothDeviceInfo());
		list << pair;
	}

	return list;
}
