/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <QNdefMessage>

QTM_USE_NAMESPACE;

class Util
{

public:

	static quint32 messageLength(const QNdefMessage message);

	static QString currentLanguageCode(void);

	static QStringList availableLanguageCodes(void);

	static QString languageCodeToString(const QString &code);

	static QString phoneNumberSubtypeToString(const QString &type);

	static QString contactDetailContextToString(const QString &type);

};

#endif /* UTIL_H */
