/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtNdefRecord.h"

#define BDADDR_OFFSET 2
#define COD_OFFSET 8
#define NAME_OFFSET 13

const QString BtNdefRecord::name(void) const
{
	return "";
}

void BtNdefRecord::setName(const QString name)
{
	(void) name;
}

const QtMobility::QBluetoothAddress BtNdefRecord::address(void) const
{
	QByteArray data = payload();
	quint64 bytes = 0;
	for (int i = 5; i >= 0; i--) {
		bytes <<= 8;
		bytes |= ((quint64)data[BDADDR_OFFSET + i] & 0xff);
	}

	return QtMobility::QBluetoothAddress(bytes);
}

void BtNdefRecord::setAddress(const QtMobility::QBluetoothAddress address)
{
	QByteArray data = payload();
	quint64 bytes = address.toUInt64();
	for (int i = 0; i < 6; i++) {
		data[BDADDR_OFFSET + i] = bytes & 0xff;
		bytes >>= 8;
	}
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
}
