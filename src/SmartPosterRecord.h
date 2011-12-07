/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _SMART_POSTER_RECORD_H_
#define _SMART_POSTER_RECORD_H_

#include <QNdefRecord>
#include <QNdefNfcUriRecord>
#include <QNdefNfcTextRecord>
#include <QList>

QTM_USE_NAMESPACE;

class SmartPosterRecord : public QNdefRecord
{

public:

	class ActionRecord;

	class SizeRecord;

	class TypeRecord;

public:

	Q_DECLARE_NDEF_RECORD(SmartPosterRecord,
			      QNdefRecord::NfcRtd,
			      "Sp",
			      QByteArray("\xd0\x00\x00", 3));

	SmartPosterRecord(QNdefNfcUriRecord uri);

	bool uri(QNdefNfcUriRecord &u) const;

	void setUri(QNdefNfcUriRecord u);

	bool act(ActionRecord &a) const;

	void setAct(ActionRecord a);

	bool size(SizeRecord &s) const;

	void setSize(SizeRecord s);

	bool type(TypeRecord &t) const;

	void setType(TypeRecord t);

	QList<QNdefNfcTextRecord> titles(void) const;

	void setTitles(QList<QNdefNfcTextRecord> t);

	QList<QNdefRecord> otherRecords(void) const;

	void setOtherRecords(QList<QNdefRecord> o);

private:

	QList<QNdefRecord> records(void) const;

	void setRecords(QList<QNdefRecord> records);

};

class SmartPosterRecord::ActionRecord : public QNdefRecord
{
	
public:
	
	enum Action {
		ActionExecute = 0,
		ActionSave = 1,
		ActionEdit = 2
	};
	
	Q_DECLARE_NDEF_RECORD(ActionRecord,
			      QNdefRecord::NfcRtd,
			      "act",
			      QByteArray("\0", 1));
	
	enum Action action(void) const;
	
	void setAction(enum Action);
	
};

class SmartPosterRecord::SizeRecord : public QNdefRecord 
{
	
public:
	
	Q_DECLARE_NDEF_RECORD(SizeRecord,
			      QNdefRecord::NfcRtd,
			      "s",
			      QByteArray("\0\0\0\0", 4));
	
	quint32 size(void) const;
	
	void setSize(quint32 size);
	
};

class SmartPosterRecord::TypeRecord : public QNdefRecord 
{
	
public:
	
	Q_DECLARE_NDEF_RECORD(TypeRecord,
			      QNdefRecord::NfcRtd,
			      "t",
			      QByteArray("application/octet-stream"));
	
	QString type(void) const;
	
		void setType(QString type);
		
};

Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(SmartPosterRecord::ActionRecord, 
				       QNdefRecord::NfcRtd,
				       "act");

Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(SmartPosterRecord::SizeRecord, 
				       QNdefRecord::NfcRtd,
				       "s");

Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(SmartPosterRecord::TypeRecord, 
				       QNdefRecord::NfcRtd,
				       "t");

Q_DECLARE_ISRECORDTYPE_FOR_NDEF_RECORD(SmartPosterRecord, 
				       QNdefRecord::NfcRtd,
				       "Sp");

#endif /* _SMART_POSTER_RECORD_H_ */

