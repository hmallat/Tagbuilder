/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtNdefRecord.h"

#include <MDebug>

#define BDADDR_OFFSET 2
#define COD_OFFSET 10
#define NAME_OFFSET 13

const QString BtNdefRecord::name(void) const
{
	QByteArray data = payload();
	char *ptr = data.data();
	int len = data.length();
	quint8 fieldlen;
	quint8 code;
	int pos = NAME_OFFSET;

	if (pos >= len) {
		goto fail;
	}
	fieldlen = ptr[pos++];

	if (pos + fieldlen > len) {
		goto fail;
	}

	code = ptr[pos++];
	fieldlen--;
	if (code != 0x08 && code != 0x09) { /* no need to support others */
		goto fail;
	}

	return QString::fromUtf8(&(ptr[pos]), fieldlen);

fail:
	return "";
}

void BtNdefRecord::setName(const QString name)
{
	char code = 0x09;
	QByteArray utf8 = name.toUtf8();
	if (utf8.length() > 248) { /* Maximum local name length */
		utf8.chop(248);
		code = 0x08;
	}

	int total = 15 + utf8.length();

	QByteArray data = payload();
	data.truncate(NAME_OFFSET);
	data.append((char)(1 + utf8.length()));
	data.append(code);
	data.append(utf8);
	data[0] = (total >> 0) & 0xff;
	data[1] = (total >> 8) & 0xff;
	setPayload(data);
}

const QBluetoothAddress BtNdefRecord::address(void) const
{
	QByteArray data = payload();
	quint64 bytes = 0;
	for (int i = 5; i >= 0; i--) {
		bytes <<= 8;
		bytes |= ((quint64)data[BDADDR_OFFSET + i] & 0xff);
	}

	return QBluetoothAddress(bytes);
}

void BtNdefRecord::setAddress(const QBluetoothAddress address)
{
	QByteArray data = payload();
	quint64 bytes = address.toUInt64();
	for (int i = 0; i < 6; i++) {
		data[BDADDR_OFFSET + i] = bytes & 0xff;
		bytes >>= 8;
	}
	setPayload(data);
}

quint32 BtNdefRecord::classOfDevice(void) const
{
	QByteArray data = payload();
	quint32 bytes = 0;
	for (int i = 2; i >= 0; i--) {
		bytes <<= 8;
		bytes |= ((quint32)data[COD_OFFSET + i] & 0xff);
	}

	return bytes;
}

void BtNdefRecord::setClassOfDevice(quint32 cod)
{
	QByteArray data = payload();
	for (int i = 0; i < 3; i++) {
		data[COD_OFFSET + i] = cod & 0xff;
		cod >>= 8;
	}
	setPayload(data);
}
