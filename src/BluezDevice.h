/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BLUEZ_DEVICE_H_
#define _BLUEZ_DEVICE_H_

#include <QObject>
#include <QDBusObjectPath>
#include <QDBusConnection>

class QDBusInterface;
class QDBusVariant;
class QDBusPendingCallWatcher;
class QDBusVariant;

class BluezDevice : public QObject
{

	Q_OBJECT;

public:

	BluezDevice(const QDBusObjectPath path, QObject *parent = 0);

	~BluezDevice(void);

	const QDBusObjectPath path(void) const;

	QDBusInterface *interface(void) const;

	const QString address(void) const;

	const QString name(void) const;

	const QString alias(void) const;

	uint cod(void) const;

signals:

	void deviceUpdated(void);

private slots:

	void getPropertiesDone(QDBusPendingCallWatcher *watcher);

	void propertyChanged(const QString, const QDBusVariant);

	void propertyChanged(const QString, const QVariant);

private:

	Q_DISABLE_COPY(BluezDevice);

	QDBusConnection m_sys;

	QDBusObjectPath m_path;

	QDBusInterface *m_interface;

	QString m_address;

	QString m_name;

	QString m_alias;

	uint m_cod;

};

#endif /* _BLUEZ_DEVICE_H_ */
