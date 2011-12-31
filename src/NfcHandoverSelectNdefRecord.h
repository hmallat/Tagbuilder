/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _NFC_HANDOVER_SELECT_NDEF_RECORD_H_
#define _NFC_HANDOVER_SELECT_NDEF_RECORD_H_

#include <QNdefRecord>

QTM_USE_NAMESPACE;

class NfcHandoverSelectNdefRecord : public QNdefRecord
{

public:

	Q_DECLARE_NDEF_RECORD(NfcHandoverSelectNdefRecord,
			      QNdefRecord::NfcRtd,
			      "Hs",
			      QByteArray("\x12", 1));

};

Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(NfcHandoverSelectNdefRecord,
				       QNdefRecord::NfcRtd,
				       "Hs");

#endif /* _NFC_HANDOVER_SELECT_NDEF_RECORD_H_ */
