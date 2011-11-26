/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_H_
#define _TAG_H_

#include <QNdefMessage>
#include <QDateTime>
#include <QString>
#include <QVariant>

QTM_USE_NAMESPACE;

class Tag 
{

public:

	Tag(const QString &name, const QNdefMessage &message);

	Tag(const QString &name, 
	    const QByteArray &message,
	    quint32 seconds);

	~Tag(void);

	const QString &name(void) const;

	const QNdefMessage &message(void) const;

	const QDateTime &creationTime(void) const;

private:

	Tag(void);

	Tag(const Tag &tag);

	Tag &operator=(const Tag &);

	QString m_name;

	QNdefMessage m_message;

	QDateTime m_creationTime;

};

Q_DECLARE_METATYPE(const Tag *);

#endif /* _TAG_H_ */
