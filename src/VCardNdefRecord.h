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

#ifndef _VCARD_NDEF_RECORD_H_
#define _VCARD_NDEF_RECORD_H_

#include <QNdefRecord>

QTM_USE_NAMESPACE;

class VCardNdefRecord : public QNdefRecord
{

public:

	Q_DECLARE_NDEF_RECORD(VCardNdefRecord,
			      QNdefRecord::Mime,
			      "text/vcard",
			      QByteArray("BEGIN:VCARD\r\nEND:VCARD\r\n", 24));

	static VCardNdefRecord fromSupportedMimeType(const QNdefRecord &other);

	static bool hasSupportedMimeType(const QNdefRecord &other);

};

Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(VCardNdefRecord,
				       QNdefRecord::Mime,
				       "text/vcard");

#endif /* _VCARD_NDEF_RECORD_H_ */
