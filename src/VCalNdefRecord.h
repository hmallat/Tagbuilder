/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _VCAL_NDEF_RECORD_H_
#define _VCAL_NDEF_RECORD_H_

#include <QNdefRecord>

QTM_USE_NAMESPACE;

class VCalNdefRecord : public QNdefRecord
{

public:

	Q_DECLARE_NDEF_RECORD(VCalNdefRecord,
			      QNdefRecord::Mime,
			      "text/calendar",
			      QByteArray("BEGIN:VCAL\r\nEND:VCAL\r\n", 22));

};

Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(VCalNdefRecord,
				       QNdefRecord::Mime,
				       "text/calendar");
				       
#endif /* _VCAL_NDEF_RECORD_H_ */
