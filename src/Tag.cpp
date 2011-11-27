/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "Tag.h"

#include <QNdefNfcTextRecord>
#include <QNdefNfcUriRecord>

/* TODO: maximum name length supported by tag storage is 65535,
   should limit somewhere what names are given to tags */

const QString Tag::BLUETOOTH_TAG = tr("Bluetooth Device");
const QString Tag::URL_TAG = tr("Bookmark");
const QString Tag::CALENDAR_TAG = tr("Calendar Entry");
const QString Tag::CONTACT_TAG = tr("Contact");
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
}

Tag::Tag(const QString &name, 
	 const QByteArray &message,
	 quint32 seconds,
	 QObject *parent)
	: QObject(parent),
	  m_name(name),
	  m_message(QNdefMessage::fromByteArray(message)),
	  m_creationTime(QDateTime::fromMSecsSinceEpoch(1000*(qint64)seconds))
{
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
	QNdefRecord record = m_message.at(0);
	
	if (record.isRecordType<QNdefNfcTextRecord>()) {
		return TEXT_TAG;
	} else if (record.isRecordType<QNdefNfcUriRecord>()) {
		return URL_TAG;
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
	static const QString UNKNOWN_ICON = "";

	return 
		type == BLUETOOTH_TAG ? BT_ICON :
		type == URL_TAG ? URL_ICON :
		type == CALENDAR_TAG ? CALENDAR_ICON : 
		type == CONTACT_TAG ? CONTACT_ICON :
		type == TEXT_TAG ? TEXT_ICON :
		type == WLAN_TAG ? WLAN_ICON : 
		UNKNOWN_ICON;
}
