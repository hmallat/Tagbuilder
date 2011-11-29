/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BluezSupplicant.h"

#include <QDBusInterface>
#include <QDBusPendingCall>
#include <QDBusPendingReply>
#include <QDBusPendingCallWatcher>

#include <MDebug>

/*
 * When started, fills in the list for known adapters and for each
 * adapter fills in the list of known devices; populates device
 * structures with attributes so that Qt bluetooth device info
 * is available. Monitors for changes in any and all. 
 */

BluezSupplicant::BluezSupplicant(QObject *parent)
	: QObject(parent),
	  m_started(false),
	  m_sys(QDBusConnection::systemBus()),
	  m_manager(new QDBusInterface("org.bluez",
				       "/",
				       "org.bluez.Manager",
				       m_sys,
				       this)),
	  m_adapters()
{
}

BluezSupplicant::~BluezSupplicant(void)
{
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
		      "AdapterAdded",
		      this,
		      SLOT(adapterAdded(const QDBusObjectPath)));
	
	m_sys.connect("org.bluez",
		      "/",
		      "org.bluez.Manager",
		      "AdapterRemoved",
		      this,
		      SLOT(adapterRemoved(const QDBusObjectPath)));
	
	QDBusPendingCall call =
		m_manager->asyncCall("ListAdapters");
	QDBusPendingCallWatcher *watcher =
		new QDBusPendingCallWatcher(call, this);
	connect(watcher, 
		SIGNAL(finished(QDBusPendingCallWatcher *)),
		this, 
		SLOT(listAdaptersDone(QDBusPendingCallWatcher *)));
}

void BluezSupplicant::listAdaptersDone(QDBusPendingCallWatcher *watcher)
{
	QList<QDBusObjectPath> paths;

	QDBusPendingReply< QList<QDBusObjectPath> > reply = *watcher;
	if (reply.isError()) {
		mDebug(__func__) << "Failed to get adapter list. ";
	} else {
		paths = reply.argumentAt<0>();
		mDebug(__func__) << paths.length() << " adapters found. ";
		for (int i = 0; i < paths.length(); i++) {
			adapterAdded(paths[i]);
		}
	}

	watcher->deleteLater();
}

void BluezSupplicant::adapterAdded(const QDBusObjectPath which)
{

	for (int i = 0; i < m_adapters.length(); i++) {
		if (m_adapters[i]->path() == which.path()) {
			mDebug(__func__) 
				<< "Adapter " << which.path() << " "
				<< "already known, not adding twice. ";
			return;
		}
	}

	m_adapters << new QDBusInterface("org.bluez",
					 which.path(),
					 "org.bluez.Adapter",
					 m_sys,
					 this);

	/* TODO: monitor for device add/rm signals for this adapter,
	   query device list. */

	mDebug(__func__) << "Adapter " << which.path() << " removed. ";
	Q_EMIT(bluezAdapterAdded(which));

}

void BluezSupplicant::adapterRemoved(const QDBusObjectPath which)
{

	for (int i = 0; i < m_adapters.length(); i++) {
		if (m_adapters[i]->path() == which.path()) {
			
			/* TODO: stop monitoring device add/rm signals
			   for this adapter, remove devices for this
			   adapter from known list. */

			QDBusInterface *adapter = m_adapters[i];
			m_adapters.removeAt(i);
			delete adapter;

			mDebug(__func__) 
				<< "Adapter " << which.path() << " removed. ";
			Q_EMIT(bluezAdapterRemoved(which));

			return;
		}
	}

	mDebug(__func__) 
		<< "Adapter " << which.path() << " "
		<< "not known, not removing. ";

}

#if 0












bool BluezSupplicant::queryDefaultAdapter(void)
{
	QDBusPendingCall call;
	QDBusPendingCallWatcher *watcher;

	switch (m_adapterQuery) {

	case QueryNotStarted:
		m_adapterQueryState = QueryInProgress;

		call = m_bluezManagerInterface->asyncCall("DefaultAdapter");
		watcher = new QDBusPendingCallWatcher(call, this);
		connect(watcher, 
			SIGNAL(finished(QDBusPendingCallWatcher *)),
			this, 
			SLOT(defaultAdapterQueryDone
			     (QDBusPendingCallWatcher *)));
		return true;

	case QueryPending:
		/* should not happen */
		m_adapterQueryState = QueryNotStarted;
		return false;

	case QueryInProgress:
		/* thou shalt be signaled */
		return true;

	case QueryFinished:
		if (m_bluezAdapterInterface != 0) {
			Q_EMIT(defaultAdapterQueried(true));
		} else {
			Q_EMIT(defaultAdapterQueried(false));
		}
		return true;

	};
}

bool BluezSupplicant::queryKnownDevices(void)
{
	return push(QueryKnownDevices);

	QDBusPendingCall call =
		m_bluezAdapterInterface->asyncCall("ListDevices");
	QDBusPendingCallWatcher *watcher = 
		new QDBusPendingCallWatcher(call, this);
	connect(watcher, SIGNAL(finished(QDBusPendingCallWatcher *)),
		this, SLOT(defaultAdapterQueried(QDBusPendingCallWatcher *)));
}

void BluezSupplicant::defaultAdapterQueried(QDBusPendingCallWatcher *watcher)
{
	QDBusObjectPath adapter;

	QDBusPendingReply<QDBusObjectPath> reply = *watcher;
	if (reply.isError()) {
		mDebug(__func__) << "Failed to get default adapter. ";
	} else {
		adapter = reply.argumentAt<0>();
		m_bluezAdapterInterface = 
			new QDBusInterface("org.bluez",
					   adapter.path(),
					   "org.bluez.Adapter",
					   QDBusConnection::systemBus(),
					   this);
	}

	watcher->deleteLater();
	m_defaultAdapterQueryState = QueryIdle;

	if (m_knownDeviceQueryState == QueryPending) {
		if (m_bluezAdapterInterface == NULL) {
			Q_EMIT(knownDevicesQueryError(...));
			m_knownDeviceQueryState = QueryIdle;
		} else {
			...
		}
	}
}

void BluezSupplicant::knownDevicesQueried(QDBusPendingCallWatcher *watcher) 
{
	QDBusPendingReply< QList<QDBusObjectPath> > reply = *watcher;
	if (reply.isError()) {
		mDebug(__func__) << "Failed to get default adapter. ";
		goto exit;
	} 

exit:
	watcher->deleteLater();
}

#endif
