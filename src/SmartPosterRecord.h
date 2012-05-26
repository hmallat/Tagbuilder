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
			      QByteArray("\xd1\x01\x01U\x00", 5));

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
		ActionDo = 0,
		ActionSave = 1,
		ActionEdit = 2,
		ActionUndefined
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

