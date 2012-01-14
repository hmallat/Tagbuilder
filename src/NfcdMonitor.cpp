/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "NfcdMonitor.h"

#include <QDBusServiceWatcher>
#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusInterface>
#include <QDBusReply>

#include <MDebug>

NfcdMonitor::NfcdMonitor(QObject *parent)
	: QObject(parent),
	  m_started(false),
	  m_status(NfcdMonitor::NfcStatusUnknown),
	  m_sys(QDBusConnection::systemBus()),
	  m_manager(new QDBusInterface("com.nokia.nfc",
				       "/",
				       "com.nokia.nfc.Manager",
				       m_sys,
				       this)),
	  m_adapter(0)
{
}

NfcdMonitor::~NfcdMonitor(void)
{
}

void NfcdMonitor::start(void)
{
	mDebug(__func__) << "Starting NFC daemon monitoring. ";

	if (m_started == true) {
		mDebug(__func__) << "Already started. ";
		return;
	}

	m_started = true;

	/* We might see this if nfcd crashes */
	m_sys.connect("com.nokia.nfc",
		      "/",
		      "com.nokia.nfc.Manager",
		      "DefaultAdapterChanged",
		      this,
		      SLOT(adapterChanged(const QDBusObjectPath)));
	
	/* We might see this if nfcd exits */
	m_sys.connect("com.nokia.nfc",
		      "/",
		      "com.nokia.nfc.Manager",
		      "AdapterRemoved",
		      this,
		      SLOT(adapterRemoved(const QDBusObjectPath)));
	
	QDBusPendingCall call =
		m_manager->asyncCall("DefaultAdapter");
	QDBusPendingCallWatcher *watcher =
		new QDBusPendingCallWatcher(call, this);
	connect(watcher, 
		SIGNAL(finished(QDBusPendingCallWatcher *)),
		this, 
		SLOT(defaultAdapterDone(QDBusPendingCallWatcher *)));
}

void NfcdMonitor::defaultAdapterDone(QDBusPendingCallWatcher *watcher)
{
	mDebug(__func__) << "ENTER";

	QDBusPendingReply<QDBusObjectPath> reply = *watcher;
	if (reply.isError()) {
		mDebug(__func__) << "Failed to get default adapter. ";
	} else {
		adapterChanged(reply.argumentAt<0>());
	}

	watcher->deleteLater();
}

void NfcdMonitor::adapterChanged(const QDBusObjectPath which)
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

	m_adapter = new QDBusInterface("com.nokia.nfc",
				       which.path(),
				       "com.nokia.nfc.Adapter",
				       m_sys,
				       this);

	m_sys.connect("com.nokia.nfc",
		      which.path(),
		      "com.nokia.nfc.Adapter",
		      "PropertyChanged",
		      this,
		      SLOT(propertyChanged(const QString, 
					   const QDBusVariant)));
	
	QDBusPendingCall call =
		m_adapter->asyncCall("GetProperties");
	QDBusPendingCallWatcher *watcher =
		new QDBusPendingCallWatcher(call, this);
	connect(watcher, 
		SIGNAL(finished(QDBusPendingCallWatcher *)),
		this, 
		SLOT(getPropertiesDone(QDBusPendingCallWatcher *)));

	mDebug(__func__) << "Adapter " << which.path() << " changed. ";
}

void NfcdMonitor::adapterRemoved(const QDBusObjectPath which)
{
	mDebug(__func__) << "ENTER";

	if (m_adapter == 0 || m_adapter->path() != which.path()) {
		mDebug(__func__) 
			<< "Adapter " << which.path() << " "
			<< "not known, not removing. ";
		return;
	}

	/* Stop listening to this adapter's signals */
	m_sys.disconnect("com.nokia.nfc",
			 which.path(),
			 "com.nokia.nfc.Adapter",
			 "PropertyChanged",
			 this,
			 SLOT(propertyChanged(const QString, 
					      const QDBusVariant)));
	
	delete m_adapter;
	m_adapter = 0;
	
	setStatus(NfcStatusUnknown);

	mDebug(__func__) << "Adapter " << which.path() << " removed. ";
}

enum NfcdMonitor::NfcStatus NfcdMonitor::status(void)
{
	return m_status;
}

void NfcdMonitor::setStatus(enum NfcdMonitor::NfcStatus status)
{
	m_status = status;
	Q_EMIT(nfcStatusChanged());
}

void NfcdMonitor::getPropertiesDone(QDBusPendingCallWatcher *watcher)
{
	mDebug(__func__) << "ENTER";

	QDBusPendingReply< QMap<QString, QVariant> > reply = *watcher;
	if (reply.isError()) {
		mDebug(__func__) << "Failed to get adapter properties. ";
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

void NfcdMonitor::propertyChanged(const QString name, const QDBusVariant value)
{
	mDebug(__func__) << "ENTER";
	propertyChanged(name, value.variant());
}

void NfcdMonitor::propertyChanged(const QString name, const QVariant value)
{
	(void) name, (void) value;

	mDebug(__func__) << "ENTER";

	if (name == "State") {
		QString s = value.toString();
		if (s == "on") {
			setStatus(NfcStatusOn);
		} else if (s == "off") {
			setStatus(NfcStatusOff);
		} else {
			setStatus(NfcStatusUnknown);
		}
	}
}

