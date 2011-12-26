/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "Util.h"

#include <QSystemInfo>
#include <QLocale>
#include <QContactPhoneNumber>
#include <QMap>

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
