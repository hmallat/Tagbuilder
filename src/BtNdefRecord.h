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

#ifndef _BT_NDEF_RECORD_H_
#define _BT_NDEF_RECORD_H_

#include <QNdefRecord>
#include <QBluetoothAddress>

QTM_USE_NAMESPACE;

class BtNdefRecord : public QNdefRecord
{

public:

	Q_DECLARE_NDEF_RECORD(BtNdefRecord,
			      QNdefRecord::Mime,
			      "application/vnd.bluetooth.ep.oob",
			      QByteArray("\x0d\x00"
					 "\x00\x00\x00\x00\x00\x00"
					 "\x04\x0d\x00\x00\x00",
					 13));

	const QString name(void) const;

	void setName(const QString name);

	const QBluetoothAddress address(void) const;

	void setAddress(const QBluetoothAddress address);

	quint32 classOfDevice(void) const;

	void setClassOfDevice(quint32 cod);

	bool isValid(void) const;

	static BtNdefRecord fromSupportedMimeType(const QNdefRecord &other);

	static bool hasSupportedMimeType(const QNdefRecord &other);

private:

	int eirOffset(quint8 which) const;

	int eirLength(quint8 which) const;

	const QByteArray eirData(quint8 which) const;

	bool removeEir(quint8 which);

	void appendEir(quint8 which, const QByteArray &eirData);

};

Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(BtNdefRecord,
				       QNdefRecord::Mime,
				       "application/vnd.bluetooth.ep.oob");
				       
#endif /* _BT_NDEF_RECORD_H_ */
