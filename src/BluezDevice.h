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

class QDBusInterface;

class BluezDevice : public QObject
{

	Q_OBJECT;

public:

	BluezDevice(const QDBusObjectPath path, QObject *parent = 0);

	~BluezDevice(void);

	const QDBusObjectPath path(void) const;

private:

	Q_DISABLE_COPY(BluezDevice);

	QDBusObjectPath m_path;

	QDBusInterface *m_interface;
  
};

#endif /* _BLUEZ_DEVICE_H_ */
