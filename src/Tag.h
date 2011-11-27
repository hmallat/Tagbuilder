/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_H_
#define _TAG_H_

#include <QObject>
#include <QNdefMessage>
#include <QDateTime>
#include <QString>
#include <QVariant>

QTM_USE_NAMESPACE;

class Tag : public QObject
{

	Q_OBJECT;

public:

	static const QString BLUETOOTH_TAG;

	static const QString URL_TAG;

	static const QString CALENDAR_TAG;

	static const QString CONTACT_TAG;

	static const QString TEXT_TAG;

	static const QString WLAN_TAG;

	Tag(const QString &name, 
	    const QNdefMessage &message,
	    QObject *parent = 0);

	Tag(const QString &name, 
	    const QByteArray &message,
	    quint32 seconds,
	    QObject *parent = 0);

	virtual ~Tag(void);

	const QString &type(void) const;

	const QString &name(void) const;

	const QNdefMessage &message(void) const;

	const QDateTime &creationTime(void) const;

	static const QString &icon(const QString &type);

private:

	static const QString UNKNOWN_TAG;

	Q_DISABLE_COPY(Tag);

	QString m_name;

	QNdefMessage m_message;

	QDateTime m_creationTime;

};

Q_DECLARE_METATYPE(const Tag *);

#endif /* _TAG_H_ */
