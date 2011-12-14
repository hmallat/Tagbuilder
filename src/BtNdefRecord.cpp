/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

/*
 * Harvested records may have other EIR fields than name or CoD; keep
 * them as-is and modify only the name and class of device when needed. 
 */

#include "BtNdefRecord.h"
#include "Tag.h"

#include <MDebug>

#define BDADDR_OFFSET 2

#define EIR_LOCAL_NAME 0x09
#define EIR_LOCAL_NAME_INCOMPLETE 0x08
#define EIR_COD 0x0d

#define EIR_NOT_FOUND (-1)

bool BtNdefRecord::isValid(void) const
{
	/* Check record validity; check that all length fields are
	   sane and that all EIR records can be read. */ 

	const char *ptr = payload().data();
	int len = payload().length();

	if (len < 8) { /* cannot contain length and BDADDR */
		mDebug(__func__) << "Truncated payload, under 8 bytes. ";
		return false;
	}

	quint16 total = 
		((((quint16)ptr[1]) << 8) & 0xff00) |
		((((quint16)ptr[0]) << 0) & 0x00ff);
	if ((int)total != len) {
		return false;
	}

	int pos = 8;
	while (pos < len) {
		if (pos + 1 > len) {
			mDebug(__func__) 
				<< "Truncated payload, cannot read EIR length";
			return false;
		}
		
		quint8 eirlen = ptr[pos];
		pos++;

		if (pos + (int)eirlen > len) {
			mDebug(__func__) 
				<< "Truncated payload, no space for EIR data";
			return false;
		}
		pos += (int)eirlen;
	}

	if (pos != len) {
		mDebug(__func__) << "Extra data after EIR records. ";
		return false;
	}

	return true;
}

int BtNdefRecord::eirOffset(quint8 which) const
{
	const char *ptr = payload().data();
	int len = payload().length();
	int pos = 8; /* just behind length and BDADDR */

	while (true) {
		int current = pos;
		quint8 eirLen;
		quint8 eirType;

		if (pos + 1 > len) {
			mDebug(__func__) << "EIR " << which << " not found";
			return EIR_NOT_FOUND;
		}
		eirLen = ptr[pos];
		pos++;

		if (pos + (int)eirLen > len) {
			/* Should not happen after validity check */
			mDebug(__func__) << "Truncated EIR at " << current;
			return EIR_NOT_FOUND;
		}
		eirType = ptr[pos];

		if (eirType == which) {
			mDebug(__func__) << "EIR " << which 
					 << " at " << current;
			return current;
		}

		pos += (int)eirLen;
	}
}

int BtNdefRecord::eirLength(quint8 which) const
{
	int off = eirOffset(which);
	if (off == EIR_NOT_FOUND) {
		return -1;
	}
	return (int)(payload()[off]);
}

const QByteArray BtNdefRecord::eirData(quint8 which) const
{
	int off = eirOffset(which);
	if (off == EIR_NOT_FOUND) {
		return QByteArray();
	}
	quint8 eirLen = payload()[off];
	return payload().mid(off + 2, (int)eirLen - 1);
}

bool BtNdefRecord::removeEir(quint8 which)
{
	int off = eirOffset(which);
	if (off == EIR_NOT_FOUND) {
		mDebug(__func__) << "Not removing EIR " << which 
				 << ", not found";
		return false;
	}

	int len = eirLength(which);
	mDebug(__func__) << "Removing EIR " << which << ", "
			 << (1 + len) << " bytes at " << off;

	mDebug(__func__) << "Before removal: ";
	Tag::dump(*this);

	QByteArray data = payload();
	data.remove(off, 1 + len);
	data[0] = (data.length() >> 0) & 0xff;
	data[1] = (data.length() >> 8) & 0xff;
	setPayload(data);

	mDebug(__func__) << "After removal: ";
	Tag::dump(*this);

	return true;
}

void BtNdefRecord::appendEir(quint8 which, const QByteArray &eirData)
{
	mDebug(__func__) << "Appending EIR " << which << ", "
			 << (2 + eirData.length()) << " bytes. ";

	mDebug(__func__) << "Before append: ";
	Tag::dump(*this);

	QByteArray data = payload();
	data.append((char)(1 + eirData.length()));
	data.append((char)which);
	data.append(eirData);
	data[0] = (data.length() >> 0) & 0xff;
	data[1] = (data.length() >> 8) & 0xff;
	setPayload(data);

	mDebug(__func__) << "After append: ";
	Tag::dump(*this);
}

const QString BtNdefRecord::name(void) const
{
	if (isValid() == false) {
		return QString();
	}

	QByteArray nameEir = eirData(EIR_LOCAL_NAME);
	if (nameEir.isNull()) {
		nameEir = eirData(EIR_LOCAL_NAME_INCOMPLETE);
		if (nameEir.isNull()) {
			return QString();
		}
	}

	return QString::fromUtf8(nameEir);
}

void BtNdefRecord::setName(const QString name)
{
	if (isValid() == false) {
		return;
	}

	/* Construct data for EIR */
	char code = EIR_LOCAL_NAME;
	QByteArray utf8 = name.toUtf8();
	if (utf8.length() > 248) { /* Maximum local name length */
		utf8.chop(248);
		code = EIR_LOCAL_NAME_INCOMPLETE;
	}

	char oldCode = EIR_LOCAL_NAME;
	int nameOffset = eirOffset(oldCode);
	if (nameOffset == EIR_NOT_FOUND) {
		oldCode = EIR_LOCAL_NAME_INCOMPLETE;
		nameOffset = eirOffset(oldCode);
	}

	if (nameOffset == EIR_NOT_FOUND) {
		/* No previous name, so tack to the end (if there is a name) */
		if (name != "") {
			appendEir(code, utf8);
		}
	} else {
		/* Had previous name, replace that (if there is a name) */
		removeEir(oldCode);
		if (name != "") {
			appendEir(code, utf8);
		}
	}
}

const QBluetoothAddress BtNdefRecord::address(void) const
{
	if (isValid() == false) {
		return QBluetoothAddress();
	}

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
	if (isValid() == false) {
		return;
	}

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
	if (isValid() == false) {
		return 0;
	}

	const QByteArray codEir = eirData(EIR_COD);
	if (codEir.isNull()) {
		return 0;
	}

	const char *ptr = codEir.data();
	quint32 bytes = 0;
	for (int i = 2; i >= 0; i--) {
		bytes <<= 8;
		bytes |= ((quint32)ptr[i] & 0xff);
	}

	return bytes;
}

void BtNdefRecord::setClassOfDevice(quint32 cod)
{
	if (isValid() == false) {
		return;
	}

	/* Construct data for EIR */
	QByteArray eir;
	for (int i = 0; i < 3; i++) {
		eir.append((char)(cod & 0xff));
		cod >>= 8;
	}

	int codOffset = eirOffset(EIR_COD);
	if (codOffset == EIR_NOT_FOUND) {
		/* No previous CoD, so tack to the end */
		appendEir(EIR_COD, eir);
	} else {
		/* Had previous CoD, replace that */
		removeEir(EIR_COD);
		appendEir(EIR_COD, eir);
	}
}

BtNdefRecord BtNdefRecord::fromSupportedMimeType(const QNdefRecord &other)
{
	/* Format is kind-of documented in Nokia 6131 NFC Wiki:
	 *
	 *  1 configuration byte
	 *  6 BDADDR (in wrong format, mind you)
	 *  3 CoD (in wrong format, mind you)
	 * 16 authentication info (16 at most, can be less)
	 *  1 short name length
	 *  n short name 
	 *
	 * Name position depends on how long the authentication info is;
	 * in the absence of public documents, only handle cases which 
	 * can be deduced from available tags:
	 *
	 * - configuration byte 0x2n -> n bytes of authentication
	 *   information
	 * 
	 */

	const QByteArray nokiabt = other.payload();

	quint64 bdaddr = 0;
	for (int i = 0; i < 6; i++) {
		bdaddr <<= 8;
		bdaddr |= (quint64)nokiabt[1 + i];
	}

	quint32 cod = 0;
	for (int i = 0; i < 3; i++) {
		cod <<= 8;
		cod |= nokiabt[7 + i];
	}

	BtNdefRecord bt;
	bt.setAddress(QBluetoothAddress(bdaddr));
	bt.setClassOfDevice(cod);

	if ((nokiabt[0] & 0xf0) == 0x20) {
		int authlen = (int)(nokiabt[0] & 0x0f);
		int namepos = 10 + authlen;
		if (namepos < nokiabt.length()) {
			int namelen = nokiabt[namepos];
			if (namepos + namelen <= nokiabt.length()) {
				QByteArray name = nokiabt.mid(namepos + 1, 
							      namelen);
				bt.setName(QString::fromUtf8(name));
			}
		}
	}
	
	return bt;
}

bool BtNdefRecord::hasSupportedMimeType(const QNdefRecord &other)
{
	return (other.typeNameFormat() == QNdefRecord::ExternalRtd &&
		other.type() == "nokia.com:bt");
}

