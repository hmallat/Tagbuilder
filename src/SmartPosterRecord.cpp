/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
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

