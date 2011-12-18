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
#include <MDebug>

QTM_USE_NAMESPACE;

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
	(void)type;
	return "whatever";
}

QString Util::addressSubtypeToString(const QString &type)
{
	(void)type;
	return "whatever";
}

