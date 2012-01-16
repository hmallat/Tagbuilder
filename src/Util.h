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
#include <QOrganizerItem>

class QDateTime;

QTM_USE_NAMESPACE;

#define CONTACT_DETAILS 4

#define CALENDAR_DETAILS 7

class Util
{

public:

	enum ContactDetail {
		NoContactDetails = 0x0,
		Name = 0x1,
		PhoneNumber = 0x2,
		EmailAddress = 0x4, 
		PhysicalAddress = 0x8,
		AllContactDetails = 0xf
	};

	Q_DECLARE_FLAGS(ContactDetails, ContactDetail);

	enum CalendarDetail {
		NoCalendarDetails = 0x0,
		Label = 0x1,
		Location = 0x2,
		EventTime = 0x4,
		JournalTime = 0x8,
		TodoTime = 0x10,
		Description = 0x20,
		Comment = 0x40,
		AllCalendarDetails = 0x7f		
	};

	Q_DECLARE_FLAGS(CalendarDetails, CalendarDetail);

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

	static const QString contactDetailName(ContactDetail detail);

	static QContact 
		contactFromNdef(const QNdefMessage &message,
				enum ContactDetail filter = AllContactDetails);

	static QNdefMessage ndefFromContact(const QContact &contact);

	static const QString calendarDetailName(CalendarDetail detail);

	static QOrganizerItem 
		organizerItemFromNdef(const QNdefMessage &message);

	static QNdefMessage 
		ndefFromOrganizerItem(const QOrganizerItem &contact);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Util::ContactDetails);

Q_DECLARE_OPERATORS_FOR_FLAGS(Util::CalendarDetails);

#endif /* UTIL_H */
