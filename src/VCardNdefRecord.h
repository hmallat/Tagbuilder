/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
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
			      "text/directory",
			      QByteArray("BEGIN:VCARD\r\nEND:VCARD\r\n", 24));

};

Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(VCardNdefRecord,
				       QNdefRecord::Mime,
				       "text/directory");
				       
#endif /* _VCARD_NDEF_RECORD_H_ */