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

#ifndef _UTIL_H_
#define _UTIL_H_

#include <QNdefMessage>
#include <QContact>
#include <QOrganizerItem>

class QDateTime;

QTM_USE_NAMESPACE;

#define CONTACT_DETAILS 6

#define CALENDAR_DETAILS 8

class Util
{

public:

	enum ContactDetail {
		NoContactDetails = 0x0,
		Name = 0x1,
		PhoneNumber = 0x2,
		EmailAddress = 0x4, 
		PhysicalAddress = 0x8,
		WebAddress = 0x10,
		Organization = 0x20,
		AllContactDetails = 0x3f
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
		TodoProgress = 0x80,
		AllCalendarDetails = 0xff		
	};

	Q_DECLARE_FLAGS(CalendarDetails, CalendarDetail);

	static const bool animateLists;

	static quint32 messageLength(const QNdefMessage message);

	static QString currentLanguageCode(void);

	static QStringList availableLanguageCodes(void);

	static QString languageCodeToString(const QString &code);

	static QString phoneNumberSubtypeToString(const QString &type);

	static QString urlSubtypeToString(const QString &type);

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
