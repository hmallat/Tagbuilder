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

#include "SmartPosterRecord.h"

#include <QNdefMessage>

/*
 * I'm not entirely happy with the QNdefRecord subclassing design. 
 * Seems that modifying a complex tag like Smart Poster either requires
 * re-parsing and re-constructing the payload bytes constantly, or 
 * then writing really clever code to maintain the structure. 
 *
 * I can't be bothered with cleverness right now, so just rewrite the
 * whole blessed payload everytime there is a modification... and
 * parse it all every time someone wants to query something. Terribly
 * suboptimal, yes, and should be improved on drastically if the class
 * was actually used in anything more intensive than a simple tag
 * creator.
 * 
 * Or perhaps I'm just thinking in too complicated a way...
 *
 */

#define FIND_ONE(type, set) {						\
		QList<QNdefRecord> r = records();			\
		for (int i = 0; i < r.length(); i++) {			\
			if (r[i].isRecordType<type>()) {		\
				set = type(r[i]);			\
				return true;				\
			}						\
		}							\
									\
		return false;						\
	}

#define SET_OR_APPEND_ONE(type, rec) {					\
		QList<QNdefRecord> r = records();			\
		int i;							\
		for (i = 0; i < r.length(); i++) {			\
			if (r[i].isRecordType<type>()) {		\
				r[i] = rec;				\
				break;					\
			}						\
		}							\
		if (i == r.length()) {					\
			r << rec;					\
		}							\
		setRecords(r);						\
	}

SmartPosterRecord::SmartPosterRecord(QNdefNfcUriRecord uri)
{
	QList<QNdefRecord> records;
	records << uri;
	setRecords(records);
}

QList<QNdefRecord> SmartPosterRecord::records(void) const
{
	QNdefMessage message = QNdefMessage::fromByteArray(payload());
	return message;
}

void SmartPosterRecord::setRecords(QList<QNdefRecord> records)
{
	QNdefMessage message(records);
	setPayload(message.toByteArray());
}

bool SmartPosterRecord::uri(QNdefNfcUriRecord &u) const
{
	FIND_ONE(QNdefNfcUriRecord, u);
}

void SmartPosterRecord::setUri(QNdefNfcUriRecord u)
{
	SET_OR_APPEND_ONE(QNdefNfcUriRecord, u);
}

bool SmartPosterRecord::act(SmartPosterRecord::ActionRecord &a) const
{
	FIND_ONE(SmartPosterRecord::ActionRecord, a);
}

void SmartPosterRecord::setAct(SmartPosterRecord::ActionRecord a)
{
	SET_OR_APPEND_ONE(SmartPosterRecord::ActionRecord, a);
}

bool SmartPosterRecord::size(SmartPosterRecord::SizeRecord &s) const
{
	FIND_ONE(SmartPosterRecord::SizeRecord, s);
}

void SmartPosterRecord::setSize(SmartPosterRecord::SizeRecord s)
{
	SET_OR_APPEND_ONE(SmartPosterRecord::SizeRecord, s);
}

bool SmartPosterRecord::type(SmartPosterRecord::TypeRecord &t) const
{
	FIND_ONE(SmartPosterRecord::TypeRecord, t);
}

void SmartPosterRecord::setType(SmartPosterRecord::TypeRecord t)
{
	SET_OR_APPEND_ONE(SmartPosterRecord::TypeRecord, t);
}

QList<QNdefNfcTextRecord> SmartPosterRecord::titles(void) const
{
	QList<QNdefRecord> r = records();
	QList<QNdefNfcTextRecord> t;

	for (int i = 0; i < r.length(); i++) {
		if (r[i].isRecordType<QNdefNfcTextRecord>() == true) {
			t << QNdefNfcTextRecord(r[i]);
		}
	}

	return t;
}

void SmartPosterRecord::setTitles(QList<QNdefNfcTextRecord> titles)
{
	QList<QNdefRecord> r = records();
	QList<QNdefRecord> s;
	int i;

	for (i = 0; i < r.length(); i++) {
		if (r[i].isRecordType<QNdefNfcTextRecord>() == false) {
			s << r[i];
		}
	}

	for (i = 0; i < titles.length(); i++) {
		s << titles[i];
	}

	setRecords(s);
}

QList<QNdefRecord> SmartPosterRecord::otherRecords(void) const
{
	QList<QNdefRecord> r = records();
	QList<QNdefRecord> t;

	for (int i = 0; i < r.length(); i++) {
		if (r[i].isRecordType<QNdefNfcUriRecord>() == false &&
		    r[i].isRecordType<QNdefNfcTextRecord>() == false &&
		    r[i].isRecordType<SmartPosterRecord::ActionRecord>() == false &&
		    r[i].isRecordType<SmartPosterRecord::SizeRecord>() == false &&
		    r[i].isRecordType<SmartPosterRecord::TypeRecord>() == false) {
			t << r[i];
		}
	}

	return t;
}

void SmartPosterRecord::setOtherRecords(QList<QNdefRecord> others)
{
	QList<QNdefRecord> r = records();
	QList<QNdefRecord> s;
	int i;

	for (i = 0; i < r.length(); i++) {
		if (r[i].isRecordType<QNdefNfcUriRecord>() == true ||
		    r[i].isRecordType<QNdefNfcTextRecord>() == true ||
		    r[i].isRecordType<SmartPosterRecord::ActionRecord>() == true ||
		    r[i].isRecordType<SmartPosterRecord::SizeRecord>() == true ||
		    r[i].isRecordType<SmartPosterRecord::TypeRecord>() == true) {
			s << r[i];
		}
	}

	s << others;
	setRecords(s);
}

enum SmartPosterRecord::ActionRecord::Action 
SmartPosterRecord::ActionRecord::action(void) const
{
	quint8 b = payload()[0];
	return 
		(b == 0x00) ? ActionDo :
		(b == 0x01) ? ActionSave :
		(b == 0x02) ? ActionEdit :
		ActionUndefined;
}
	
void SmartPosterRecord::ActionRecord::setAction
(enum SmartPosterRecord::ActionRecord::Action a)
{
	char c = 
		(a == ActionDo)   ? 0x00 :
		(a == ActionSave) ? 0x01 :
		(a == ActionEdit) ? 0x02 :
		0xff;
	QByteArray p(&c, 1);
	setPayload(p);
}

