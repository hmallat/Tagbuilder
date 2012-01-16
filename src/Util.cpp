/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "Util.h"
#include "VCardNdefRecord.h"
#include "VCalNdefRecord.h"

#include <QSystemInfo>
#include <QLocale>
#include <QContactPhoneNumber>
#include <QMap>
#include <MInputMethodState>

#include <QVersitContactExporter>
#include <QVersitContactImporter>
#include <QVersitReader>
#include <QVersitWriter>

#include <QContactName>
#include <QContactPhoneNumber>
#include <QContactEmailAddress>
#include <QContactAddress>

#include <QVersitOrganizerExporter>
#include <QVersitOrganizerImporter>
#include <QVersitReader>
#include <QVersitWriter>

#include <QOrganizerEventTime>
#include <QOrganizerJournalTime>
#include <QOrganizerTodoTime>
#include <QOrganizerItemComment>
#include <QOrganizerItemDescription>
#include <QOrganizerItemDisplayLabel>
#include <QOrganizerItemLocation>

#include <MDebug>

static QSystemInfo *_singleton_sysinfo = NULL;

static QSystemInfo *_systeminfo(void)
{
	if (_singleton_sysinfo == NULL) {
		_singleton_sysinfo = new QSystemInfo();
	}
	return _singleton_sysinfo;
}

quint32 Util::messageLength(const QNdefMessage message)
{
	quint32 total = 0;
	for (int i = 0; i < message.length(); i++) {
		const QNdefRecord record = message[i];
		total += 
			1 + /* NDEF header */
			1 + /* Type length */
			(record.payload().length() < 256 ? 1 : 4) + /* Pllen */
			record.type().length() + /* type */
			record.payload().length(); /* pl */
	}

	return total;
}

QString Util::currentLanguageCode(void)
{
	return _systeminfo()->currentLanguage();
}

QStringList Util::availableLanguageCodes(void)
{
	return _systeminfo()->availableLanguages();
}

QString Util::languageCodeToString(const QString &code)
{
	QLocale locale(code);
	QString lang = locale.languageToString(locale.language());
	mDebug(__func__) << code << "->" << lang;
	return lang;
}

QString Util::phoneNumberSubtypeToString(const QString &type)
{
	static QMap<QString, QString> typeMap;
	static bool typeMapInitialized = false;

	if (typeMapInitialized == false) {
		const char* types[] = {
			QContactPhoneNumber::SubTypeAssistant.latin1(),
			QContactPhoneNumber::SubTypeBulletinBoardSystem.latin1(),
			QContactPhoneNumber::SubTypeCar.latin1(),
			QContactPhoneNumber::SubTypeDtmfMenu.latin1(),
			QContactPhoneNumber::SubTypeFax.latin1(),
			QContactPhoneNumber::SubTypeLandline.latin1(),
			QContactPhoneNumber::SubTypeMessagingCapable.latin1(),
			QContactPhoneNumber::SubTypeMobile.latin1(),
			QContactPhoneNumber::SubTypeModem.latin1(),
			QContactPhoneNumber::SubTypePager.latin1(),
			QContactPhoneNumber::SubTypeVideo.latin1(),
			QContactPhoneNumber::SubTypeVoice.latin1()
		};
		const QString trans[] = {
			QObject::tr("Assistant"),
			QObject::tr("BBS"),
			QObject::tr("Car phone"),
			QObject::tr("DTMF Menu"),
			QObject::tr("Fax"),
			QObject::tr("Landline"),
			QObject::tr("Messaging"),
			QObject::tr("Mobile"),
			QObject::tr("Modem"),
			QObject::tr("Pager"),
			QObject::tr("Video"),
			QObject::tr("Voice")
		};
		const int count = 12;

		for (int i = 0; i < count; i++) {
			QString key = QString::fromLatin1(types[i]);
			typeMap[key] = trans[i];
		}
		typeMapInitialized = true;
	}

	if (typeMap.contains(type)) {
		return typeMap[type];
	}
	return QObject::tr("");
}

QString Util::contactDetailContextToString(const QString &ctxt)
{
	static QMap<QString, QString> ctxtMap;
	static bool ctxtMapInitialized = false;

	if (ctxtMapInitialized == false) {
		const char* ctxts[] = {
			QContactDetail::ContextHome.latin1(),
			QContactDetail::ContextWork.latin1()
		};
		const QString trans[] = {
			QObject::tr("Home"),
			QObject::tr("Work")
		};
		const int count = 2;

		for (int i = 0; i < count; i++) {
			QString key = QString::fromLatin1(ctxts[i]);
			ctxtMap[key] = trans[i];
		}
		ctxtMapInitialized = true;
	}

	if (ctxtMap.contains(ctxt)) {
		return ctxtMap[ctxt];
	}
	return QObject::tr("");
}

QString Util::eventTimeToString(const QDateTime &begin,
				const QDateTime &end,
				bool forceDate)
{
	/* Make a nice string rep of an event's time range, without
	   repeating fields that don't need to be repeated. */

	if (end == QDateTime()) { 
		return begin.time().toString(Qt::SystemLocaleShortDate);
	} 

	if (begin.date() == end.date()) { /* within the same day */
		QString s;

		if (forceDate == true) {
			s += begin.date().toString(Qt::SystemLocaleShortDate);
			s += " ";
		}

		s += begin.time().toString(Qt::SystemLocaleShortDate);
		s += "-";
		s += end.time().toString(Qt::SystemLocaleShortDate);

		return s;
	}

	return 
		begin.toString(Qt::SystemLocaleShortDate) +
		"-" +
		end.toString(Qt::SystemLocaleShortDate);
}

QString Util::eventDurationToString(const QDateTime &begin,
				    const QDateTime &end)
{
	if (end == QDateTime()) { 
		return "";
	}

	int diff = begin.secsTo(end);
	QString rep;

	if (diff >= 24*60*60) {
		int days = diff / (24*60*60);
		rep += QString("%1 %2").arg(days).arg(days == 1 
						      ? QObject::tr("day") 
						      : QObject::tr("days"));
		diff %= 24*60*60;
		if (diff != 0) {
			rep += " ";
		}
	}

	if (diff >= 60*60) {
		int hours = diff / (60*60);
		rep += QString("%1 %2").arg(hours).arg(hours == 1 
						       ? QObject::tr("hour") 
						       : QObject::tr("hours"));
		diff %= 60*60;
		if (diff != 0) {
			rep += " ";
		}
	}

	if (diff >= 60) {
		int mins = diff / 60;
		rep += QString("%1 %2").arg(mins).arg(mins == 1 
						      ? QObject::tr("minute") 
						      : QObject::tr("minutes"));
		diff %= 60;
		if (diff != 0) {
			rep += " ";
		}
	}

	if (diff > 0) {
		rep += QString("%1 %2").arg(diff).arg(diff == 1 
						      ? QObject::tr("second") 
						      : QObject::tr("seconds"));
	}

	return rep;
}

int Util::imAttributeExtensionId(void)
{
	static int id = -1;

	if (id == -1) {
		id = MInputMethodState::instance()->registerAttributeExtension();
	}

	return id;
}

QContact Util::contactFromNdef(const QNdefMessage &message,
			       enum ContactDetail filter)
{
	ContactDetail detail[CONTACT_DETAILS] = {
		Name, 
		PhoneNumber, 
		EmailAddress,
		PhysicalAddress
	};
	
	QVersitContactImporter importer;
	QList<QContactDetail> details;
	QContact contact;

	if (message.length() != 1 ||
	    message[0].isRecordType<VCardNdefRecord>() == false) {
		return QContact();
	}

	mDebug(__func__) << message[0].typeNameFormat();
	mDebug(__func__) << QString::fromAscii(message[0].type());
	mDebug(__func__) << QString::fromAscii(message[0].payload());

	QVersitReader reader(message[0].payload());
	if (reader.startReading() == false ||
	    reader.waitForFinished() == false) {
		mDebug(__func__) << "Reader fail, " << reader.error() << ". ";
		goto fail;
	}

	if (reader.results().length() == 0) {
		mDebug(__func__) << "No results. ";
		goto fail;
	}

	if (importer.importDocuments(reader.results()) == false ||
	    importer.contacts().length() == 0) {
		mDebug(__func__) << "Importer fail. ";
		goto fail;
	}

	/* We don't want all possible details, only those which are
	   selected from the set of supported details. */ 

	for (int i = 0; i < CONTACT_DETAILS; i++) {
		const QString name = contactDetailName(detail[i]);
		if ((filter & detail[i]) != 0) {
			mDebug(__func__) << "Appending details for " << name;
			details << importer.contacts()[0].details(name);
		}
	}

	mDebug(__func__) << "Saving " << details.length() 
			 << " details to contact. ";
	for (int i = 0; i < details.length(); i++) {
		contact.saveDetail(&(details[i]));
	}

	return contact;

fail:
	return QContact();
}

const QString Util::contactDetailName(Util::ContactDetail detail)
{
	if (detail == Name) {
		return QContactName::DefinitionName;
	} else if (detail == PhoneNumber) {
		return QContactPhoneNumber::DefinitionName;
	} else if (detail == EmailAddress) {
		return QContactEmailAddress::DefinitionName;
	} else if (detail == PhysicalAddress) {
		return QContactAddress::DefinitionName;
	}

	return QString();
}

QNdefMessage Util::ndefFromContact(const QContact &contact)
{
	QNdefMessage message;
	VCardNdefRecord vc;
	QByteArray data;
	QVersitWriter writer(&data);
	QVersitContactExporter exporter;

	if (exporter.exportContacts(QList<QContact>() << contact,
				    QVersitDocument::VCard30Type) == false) {
		goto fail;
	}

	if (writer.startWriting(exporter.documents()) == false ||
	    writer.waitForFinished() == false) {
		goto fail;
	}

	vc.setPayload(data);
	message << vc;
	return message;
	
fail:
	return QNdefMessage();
}

const QString Util::calendarDetailName(Util::CalendarDetail detail)
{
	if (detail == Label) {
		return QOrganizerItemDisplayLabel::DefinitionName;
	} else if (detail == Location) {
		return QOrganizerItemLocation::DefinitionName;
	} else if (detail == EventTime) {
		return QOrganizerEventTime::DefinitionName;
	} else if (detail == JournalTime) {
		return QOrganizerJournalTime::DefinitionName;
	} else if (detail == TodoTime) {
		return QOrganizerTodoTime::DefinitionName;
	} else if (detail == Description) {
		return QOrganizerItemDescription::DefinitionName;
	} else if (detail == Comment) {
		return QOrganizerItemComment::DefinitionName;
	}

	return QString();
}

QOrganizerItem Util::organizerItemFromNdef(const QNdefMessage &message)
{
	CalendarDetail detail[CALENDAR_DETAILS] = {
		Label,
		Location,
		EventTime,
		JournalTime,
		TodoTime,
		Description,
		Comment
	};

	QVersitOrganizerImporter importer;
	QList<QOrganizerItemDetail> details;
	QOrganizerItem item;

	if (message.length() != 1 ||
	    message[0].isRecordType<VCalNdefRecord>() == false) {
		return QOrganizerItem();
	}

	mDebug(__func__) << message[0].typeNameFormat();
	mDebug(__func__) << QString::fromAscii(message[0].type());
	mDebug(__func__) << QString::fromAscii(message[0].payload());

	QVersitReader reader(message[0].payload());
	if (reader.startReading() == false ||
	    reader.waitForFinished() == false) {
		mDebug(__func__) << "Reader fail, " << reader.error() << ". ";
		goto fail;
	}

	if (reader.results().length() == 0) {
		mDebug(__func__) << "No results. ";
		goto fail;
	}

	if (importer.importDocument(reader.results()[0]) == false ||
	    importer.items().length() == 0) {
		mDebug(__func__) << "Importer fail. ";
		goto fail;
	}

	/* We don't want all possible details, only those which are
	   selected from the set of supported details. */ 

	for (int i = 0; i < CALENDAR_DETAILS; i++) {
		const QString name = calendarDetailName(detail[i]);
		mDebug(__func__) << "Appending details for " << name;
		details << importer.items()[0].details(name);
	}

	item.setType(importer.items()[0].type());
	mDebug(__func__) << "Saving " << details.length() 
			 << " details to item. ";
	for (int i = 0; i < details.length(); i++) {
		mDebug(__func__) << "Saving " << details[i].definitionName();
		item.saveDetail(&(details[i]));
	}

	return item;

fail:
	return QOrganizerItem();
}

QNdefMessage Util::ndefFromOrganizerItem(const QOrganizerItem &item)
{
	QNdefMessage message;
	VCalNdefRecord vc;
	QByteArray data;
	QVersitWriter writer(&data);
	QVersitOrganizerExporter exporter;
	QList<QOrganizerItem> items;

	items << item;
	if (exporter.exportItems(items,
				 QVersitDocument::ICalendar20Type) == false) {
		QVersitOrganizerExporter::Error e = exporter.errorMap()[0];
		mDebug(__func__) << "Export fail, " << e;
		goto fail;
	}

	if (writer.startWriting(exporter.document()) == false ||
	    writer.waitForFinished() == false) {
		mDebug(__func__) << "Write fail";
		goto fail;
	}

	vc.setPayload(data);
	message << vc;
	return message;

fail:
	return QNdefMessage();
}
