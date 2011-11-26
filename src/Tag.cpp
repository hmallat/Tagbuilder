/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "Tag.h"

Tag::Tag(const QString &name, const QNdefMessage &message)
	: m_name(name),
	  m_message(message),
	  m_creationTime(QDateTime::currentDateTime())
{
}

Tag::Tag(const QString &name, 
	 const QByteArray &message,
	 quint32 seconds)
	: m_name(name),
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
