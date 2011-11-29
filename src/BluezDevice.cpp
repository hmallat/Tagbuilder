/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BluezDevice.h"

#include <QDBusConnection>
#include <QDBusInterface>

BluezDevice::BluezDevice(const QDBusObjectPath path, QObject *parent)
	: QObject(parent),
	  m_path(path),
	  m_interface(new QDBusInterface("org.bluez",
					 path.path(),
					 "org.bluez.Device",
					 QDBusConnection::systemBus(),
					 this))
{
}

BluezDevice::~BluezDevice(void)
{
}

const QDBusObjectPath BluezDevice::path(void) const
{
	return m_path;
}


