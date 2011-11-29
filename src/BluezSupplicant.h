/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

/* The Qt Connectivity interface doesn't provide all functionality I need,
   hence the BlueZ supplicant for querying certain data, such as known
   BT devices. */

#ifndef _BLUEZ_SUPPLICANT_H_
#define _BLUEZ_SUPPLICANT_H_

#include <QObject>
#include <QDBusObjectPath>
#include <QDBusConnection>
#include <QDBusVariant>

class BluezDevice;
class QDBusInterface;
class QDBusPendingCallWatcher;

class BluezSupplicant : public QObject
{

	Q_OBJECT;

public:

	BluezSupplicant(QObject *parent = 0);

	~BluezSupplicant(void);

	void start(void);

	bool isInitialized(void);

signals:

	void initialized(void);

	void bluezAdapterChanged(QDBusObjectPath which);

	void bluezAdapterRemoved(QDBusObjectPath which);

	void bluezDeviceCreated(QDBusObjectPath which);

	void bluezDeviceRemoved(QDBusObjectPath which);

private Q_SLOTS:

	void defaultAdapterDone(QDBusPendingCallWatcher *watcher);

	void adapterChanged(const QDBusObjectPath which);

	void adapterRemoved(const QDBusObjectPath which);

	void listDevicesDone(QDBusPendingCallWatcher *watcher);

	void deviceCreated(const QDBusObjectPath which);

	void deviceRemoved(const QDBusObjectPath which);

	void getDevicePropertiesDone(QDBusPendingCallWatcher *watcher);

	void devicePropertyChanged(QString name,
				   QDBusVariant value);

private:

	Q_DISABLE_COPY(BluezSupplicant);

	void callPending(void);

	void callFinished(void);

	bool m_started;

	bool m_initialized;

	int m_pendingCalls;

	QDBusConnection m_sys;

	QDBusInterface *m_manager;

	QDBusInterface *m_adapter;

	QList<BluezDevice *> m_devices;

};

#endif /* _BLUEZ_SUPPLICANT_H_ */
