/*

    NFC Tag Builder for Nokia N9
    Copyright (C) 2011,2012  Hannu Mallat <hmallat@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

	static const QString UNKNOWN_TAG;

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

	static const QString &type(const QNdefMessage &message);

	static const QString &icon(const QString &type);

	static void dump(const QNdefRecord &record);

	static void dump(const QNdefMessage &message);

private:

	Q_DISABLE_COPY(Tag);

	QString m_name;

	QNdefMessage m_message;

	QDateTime m_creationTime;

};

Q_DECLARE_METATYPE(const Tag *);

#endif /* _TAG_H_ */
