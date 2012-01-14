/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _NFCD_MONITOR_H_
#define _NFCD_MONITOR_H_

#include <QObject>
#include <QDBusConnection>
#include <QDBusObjectPath>

class QDBusInterface;
class QDBusPendingCallWatcher;
class QDBusVariant;

class NfcdMonitor : public QObject 
{

	Q_OBJECT;

public:

	enum NfcStatus {
		NfcStatusUnknown,
		NfcStatusOff,
		NfcStatusOn
	};

	NfcdMonitor(QObject *parent = 0);

	~NfcdMonitor(void);

	void start(void);

	enum NfcStatus status(void);

Q_SIGNALS:

	void nfcStatusChanged(void);

private Q_SLOTS:

	void defaultAdapterDone(QDBusPendingCallWatcher *watcher);

	void adapterChanged(const QDBusObjectPath which);

	void adapterRemoved(const QDBusObjectPath which);

	void getPropertiesDone(QDBusPendingCallWatcher *watcher);

	void propertyChanged(const QString name, const QDBusVariant value);

private:

	void setStatus(enum NfcStatus status);

	void propertyChanged(const QString name, const QVariant value);

	bool m_started;

	enum NfcStatus m_status;

	QDBusConnection m_sys;

	QDBusInterface *m_manager;

	QDBusInterface *m_adapter;

};

#endif /* _NFCD_MONITOR_H_ */
