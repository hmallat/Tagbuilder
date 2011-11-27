/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BT_NDEF_RECORD_H_
#define _BT_NDEF_RECORD_H_

#include <QNdefRecord>
#include <QBluetoothAddress>

class BtNdefRecord : public QtMobility::QNdefRecord
{

public:

	Q_DECLARE_NDEF_RECORD(BtNdefRecord,
			      QNdefRecord::Mime,
			      "application/vnd.bluetooth.ep.oob",
			      QByteArray("\x0f\x00"
					 "\x00\x00\x00\x00\x00\x00"
					 "\x04\x0d\x00\x00\x00"
					 "\x01\x09", 
					 15));

	const QString name(void) const;

	void setName(const QString name);

	const QtMobility::QBluetoothAddress address(void) const;

	void setAddress(const QtMobility::QBluetoothAddress address);

	quint32 classOfDevice(void) const;

	void setClassOfDevice(quint32 cod);

private:

};

Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(BtNdefRecord,
				       QNdefRecord::Mime,
				       "application/vnd.bluetooth.ep.oob");
				       
#endif /* _BT_NDEF_RECORD_H_ */
