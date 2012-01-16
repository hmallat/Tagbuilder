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
#include <QContact>

class QDateTime;

QTM_USE_NAMESPACE;

#define CONTACT_DETAILS 4

class Util
{

public:

	enum ContactDetail {
		NoDetails = 0x0,
		Name = 0x1,
		PhoneNumber = 0x2,
		EmailAddress = 0x4, 
		PhysicalAddress = 0x8,
		AllDetails = 0xf
	};

	Q_DECLARE_FLAGS(ContactDetails, ContactDetail);

	static quint32 messageLength(const QNdefMessage message);

	static QString currentLanguageCode(void);

	static QStringList availableLanguageCodes(void);

	static QString languageCodeToString(const QString &code);

	static QString phoneNumberSubtypeToString(const QString &type);

	static QString contactDetailContextToString(const QString &type);

	static QString eventTimeToString(const QDateTime &begin,
					 const QDateTime &end,
					 bool forceDate = false);

	static QString eventDurationToString(const QDateTime &begin,
					     const QDateTime &end);
	
	static int imAttributeExtensionId(void);

	static const QString contactDetailName(Util::ContactDetail detail);

	static QContact contactFromNdef(const QNdefMessage &message,
					enum ContactDetail filter 
					= AllDetails);

	static QNdefMessage ndefFromContact(const QContact &contact);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Util::ContactDetails);

#endif /* UTIL_H */
