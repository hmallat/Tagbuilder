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

class QDBusInterface;
class QDBusPendingCallWatcher;

class BluezSupplicant : public QObject
{

	Q_OBJECT;

public:

	BluezSupplicant(QObject *parent = 0);

	~BluezSupplicant(void);

	void start(void);

	const QDBusObjectPath defaultAdapter(void) const;

signals:

	void bluezAdapterAdded(QDBusObjectPath which);

	void bluezAdapterRemoved(QDBusObjectPath which);

private Q_SLOTS:

	void listAdaptersDone(QDBusPendingCallWatcher *watcher);

	void adapterAdded(const QDBusObjectPath which);

	void adapterRemoved(const QDBusObjectPath which);

private:

	Q_DISABLE_COPY(BluezSupplicant);

	bool m_started;

	QDBusConnection m_sys;

	QDBusInterface *m_manager;

	QList<QDBusInterface *> m_adapters;

};

#endif /* _BLUEZ_SUPPLICANT_H_ */
