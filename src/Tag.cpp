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

#include "Tag.h"
#include "BtNdefRecord.h"
#include "VCalNdefRecord.h"
#include "VCardNdefRecord.h"
#include "SmartPosterRecord.h"
#include "NfcHandoverSelectNdefRecord.h"

#include <QNdefNfcTextRecord>
#include <QNdefNfcUriRecord>

#include <MDebug>

extern "C"
{
#include <ctype.h>
}

/* TODO: maximum name length supported by tag storage is 65535,
   should limit somewhere what names are given to tags */

const QString Tag::BLUETOOTH_TAG = tr("Bluetooth Device");
const QString Tag::URL_TAG = tr("Bookmark");
const QString Tag::CALENDAR_TAG = tr("Calendar Entry");
const QString Tag::CONTACT_TAG = tr("Contact Card");
const QString Tag::TEXT_TAG = tr("Text");
const QString Tag::WLAN_TAG = tr("Wireless Network");
const QString Tag::UNKNOWN_TAG = tr("Unknown tag");

Tag::Tag(const QString &name, 
	 const QNdefMessage &message,
	 QObject *parent)
	: QObject(parent),
	  m_name(name),
	  m_message(message),
	  m_creationTime(QDateTime::currentDateTime())
{
	if (m_message.isEmpty()) {
		mDebug(__func__) << "Empty NDEF for tag " << name;
	} else {
		for (int i = 0; i < m_message.length(); i++) {
			mDebug(__func__) << "NDEF for tag " << name 
					 << " record " << i << " has " 
					 << m_message[i].payload().length()
					 << " bytes. ";
		}
	}
}

Tag::Tag(const QString &name, 
	 const QByteArray &message,
	 quint32 seconds,
	 QObject *parent)
	: QObject(parent),
	  m_name(name),
	  m_message(),
	  m_creationTime(QDateTime::fromMSecsSinceEpoch(1000*(qint64)seconds))
{
	mDebug(__func__) << "Parsing from byte array of " << message.length() << " bytes. ";
	m_message = QNdefMessage::fromByteArray(message);
	if (m_message.isEmpty()) {
		mDebug(__func__) << "Empty NDEF for tag " << name;
	} else {
		mDebug(__func__) << "NDEF for tag " << name
				 << " has " << m_message.length()
				 << " records. ";
		for (int i = 0; i < m_message.length(); i++) {
			mDebug(__func__) << "NDEF for tag " << name 
					 << " record " << i << " has " 
					 << m_message[i].payload().length()
					 << " bytes, has TNF "
					 << m_message[i].typeNameFormat()
					 << " and has type "
					 << QString::fromAscii(m_message[i].type().data(),
							       m_message[i].type().length()) 
					 << ". ";
		}
	}
}

Tag::~Tag(void)
{
}

const QString &Tag::name(void) const
{
	return m_name;
}

const QNdefMessage &Tag::message(void) const
{
	return m_message;
}

const QDateTime &Tag::creationTime(void) const
{
	return m_creationTime;
}

const QString &Tag::type(void) const
{
	return type(m_message);
}

const QString &Tag::type(const QNdefMessage &message)
{
	QNdefRecord first = message[0];
	
	if (first.isRecordType<QNdefNfcTextRecord>() &&
	    message.length() == 1) {
		return TEXT_TAG;
	} else if ((first.isRecordType<QNdefNfcUriRecord>() ||
		    first.isRecordType<SmartPosterRecord>())  &&
		   message.length() == 1) {
		return URL_TAG;
	} else if (first.isRecordType<BtNdefRecord>() &&
		   message.length() == 1) {
		return BLUETOOTH_TAG;
	} else if (first.isRecordType<VCalNdefRecord>() &&
		   message.length() == 1) {
		return CALENDAR_TAG;
	} else if (first.isRecordType<VCardNdefRecord>() &&
		   message.length() == 1) {
		return CONTACT_TAG;
	} else if (first.isRecordType<NfcHandoverSelectNdefRecord>() &&
		   message.length() == 2) {
		QNdefRecord second = message[1];
		if (second.isRecordType<BtNdefRecord>()) {
			return BLUETOOTH_TAG;
		} else {
			return UNKNOWN_TAG;
		}
	} else {
		return UNKNOWN_TAG;
	}
}

const QString &Tag::icon(const QString &type)
{
	static const QString BT_ICON = "icon-m-content-bluetooth";
	static const QString URL_ICON = "icon-m-content-url";
	static const QString CALENDAR_ICON = "icon-m-content-calendar";
	static const QString CONTACT_ICON = "icon-m-content-avatar-placeholder";
	static const QString TEXT_ICON = "icon-m-content-text";
	static const QString WLAN_ICON = "icon-m-content-wlan";
	static const QString UNKNOWN_ICON = "icon-m-content-file-unknown";

	return 
		type == BLUETOOTH_TAG ? BT_ICON :
		type == URL_TAG ? URL_ICON :
		type == CALENDAR_TAG ? CALENDAR_ICON : 
		type == CONTACT_TAG ? CONTACT_ICON :
		type == TEXT_TAG ? TEXT_ICON :
		type == WLAN_TAG ? WLAN_ICON : 
		UNKNOWN_ICON;
}

#define CHUNK 16

void Tag::dump(const QNdefRecord &record)
{
	static const char *h = "0123456789abcdef";

	QNdefRecord::TypeNameFormat tnf = record.typeNameFormat();
	QByteArray type = record.type();
	QByteArray data = record.payload();

	mDebug(__func__) << "TNF " << tnf << "(" <<
		(tnf == QNdefRecord::Empty ? "empty" :
		 tnf == QNdefRecord::NfcRtd ? "forum" : 
		 tnf == QNdefRecord::Mime ? "mime" :
		 tnf == QNdefRecord::Uri ? "uri" :
		 tnf == QNdefRecord::ExternalRtd ? "external" :
		 tnf == QNdefRecord::Unknown ? "unknown" : 
		 "unspecified") << ")";

	mDebug(__func__) << "Type '" << QString::fromAscii(type) << "'";

	mDebug(__func__) << "Payload";
	for (int pos = 0; pos < data.length(); pos += CHUNK) {
		QString line;

		line.append("[");
		line.append(h[(pos >> 12) & 0xf]);
		line.append(h[(pos >>  8) & 0xf]);
		line.append(h[(pos >>  4) & 0xf]);
		line.append(h[(pos >>  0) & 0xf]);
		line.append("] ");

		for (int cur = 0; cur < CHUNK; cur++) {
			if (pos + cur < data.length()) {
				line.append(h[(data[pos + cur] >> 4) & 0xf]);
				line.append(h[(data[pos + cur] >> 0) & 0xf]);
				line.append(' ');
			} else {
				line.append("   ");
			}
		}

		for (int cur = 0; cur < CHUNK; cur++) {
			if (pos + cur < data.length()) {
				line.append((isprint(data[pos + cur])
					     ? data[pos + cur]
					     : '.'));
			} else {
				line.append(' ');
			}
		}

		mDebug(__func__) << line;
	}
}

void Tag::dump(const QNdefMessage &message)
{
	mDebug(__func__) << "--------------------------------";

	mDebug(__func__) 
		<< "NDEF message with " << message.length() << " records. ";

	for (int i = 0; i < message.length(); i++) {
		mDebug(__func__) << "Record " << i << ":";
		dump(message[i]);
	}

	mDebug(__func__) << "--------------------------------";
}


