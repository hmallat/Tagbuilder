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
#include <QDBusPendingCallWatcher>
#include <QDBusPendingReply>
#include <QBluetoothAddress>

#include <MDebug>

BluezDevice::BluezDevice(const QDBusObjectPath path, QObject *parent)
	: QObject(parent),
	  m_sys(QDBusConnection::systemBus()),
	  m_path(path),
	  m_interface(new QDBusInterface("org.bluez",
					 path.path(),
					 "org.bluez.Device",
					 m_sys, 
					 this)),
	  m_address(""),
	  m_name(""),
	  m_alias(""),
	  m_cod(0)
{
	m_sys.connect("org.bluez",
		      path.path(),
		      "org.bluez.Device",
		      "PropertyChanged",
		      this,
		      SLOT(propertyChanged(QString, QDBusVariant)));
	
	QDBusPendingCall call =
		m_interface->asyncCall("GetProperties");
	QDBusPendingCallWatcher *watcher =
		new QDBusPendingCallWatcher(call, this);
	connect(watcher, 
		SIGNAL(finished(QDBusPendingCallWatcher *)),
		this, 
		SLOT(getPropertiesDone(QDBusPendingCallWatcher *)));

}

BluezDevice::BluezDevice(const QString address,
			 QMap<QString, QVariant> properties,
			 QObject *parent)
	: QObject(parent),
	  m_sys(QDBusConnection::systemBus()),
	  m_path("/fake"),
	  m_interface(0),
	  m_address(address),
	  m_name(""),
	  m_alias(""),
	  m_cod(0)
{
	QMapIterator<QString, QVariant> p(properties);
	while(p.hasNext()) {
		p.next();
		QString name = p.key();
		QVariant value = p.value();
		propertyChanged(name, value);
	}
}

BluezDevice::~BluezDevice(void)
{
}

const QDBusObjectPath BluezDevice::path(void) const
{
	return m_path;
}

QDBusInterface *BluezDevice::interface(void) const
{
	return m_interface;
}

const QString BluezDevice::address(void) const
{
	return m_address;
}

const QString BluezDevice::name(void) const
{
	return m_name;
}

const QString BluezDevice::alias(void) const
{
	return (m_alias != "") ? m_alias : m_name;
}

uint BluezDevice::cod(void) const
{
	return m_cod;
}

void BluezDevice::getPropertiesDone(QDBusPendingCallWatcher *watcher)
{
	mDebug(__func__) << "ENTER";

	QDBusPendingReply< QMap<QString, QVariant> > reply = *watcher;
	if (reply.isError()) {
		mDebug(__func__) << "Failed to get device properties. ";
		goto exit;
	} else {
		QMap<QString, QVariant> properties = reply.argumentAt<0>();
		QMapIterator<QString, QVariant> p(properties);
		while(p.hasNext()) {
			p.next();
			QString name = p.key();
			QVariant value = p.value();
			propertyChanged(name, value);
		}
	}

exit:
	watcher->deleteLater();
}

void BluezDevice::propertyChanged(const QString name, const QVariant value)
{
	mDebug(__func__) << "ENTER";

	if (name == "Address") {
		m_address = value.toString();
		mDebug(__func__) << "Address = " << m_address;
		Q_EMIT(deviceUpdated(path()));
	} else if (name == "Name") {
		m_name = value.toString();
		mDebug(__func__) << "Name = " << m_name;
		Q_EMIT(deviceUpdated(path()));
	} else if (name == "Alias") {
		m_alias = value.toString();
		mDebug(__func__) << "Alias = " << m_alias;
		Q_EMIT(deviceUpdated(path()));
	} else if (name == "Class") {
		m_cod = value.toUInt();
		mDebug(__func__) << "CoD = " << m_cod;
		Q_EMIT(deviceUpdated(path()));
	}
}

void BluezDevice::propertyChanged(const QString name, const QDBusVariant value)
{
	mDebug(__func__) << "ENTER";
	propertyChanged(name, value.variant());
}

QBluetoothDeviceInfo BluezDevice::toBluetoothDeviceInfo(void) const
{
	return QBluetoothDeviceInfo(QBluetoothAddress(address()),
				    alias(),
				    cod());
}

