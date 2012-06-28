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

#ifndef _FOURSQUARE_VENUE_SEARCH_H_
#define _FOURSQUARE_VENUE_SEARCH_H_

#include <QObject>

#include "FoursquareVenue.h"

#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVariant>

class FoursquareVenueSearch : public QObject
{

	Q_OBJECT;

public:

	enum SearchStatus { Ok, Incomplete, Error, AuthenticationError };

	FoursquareVenueSearch(QString auth, QObject *parent = 0);

	~FoursquareVenueSearch(void);

	bool venuesByCoordinates(double lat, double lon);

	bool venuesByLocation(const QString loc);

	const QList<FoursquareVenue> results(void);

	enum SearchStatus status(void);

Q_SIGNALS:

	void searchComplete(void);

private Q_SLOTS:

	void requestFinished(void);

private:

	Q_DISABLE_COPY(FoursquareVenueSearch);

	bool search(QUrl query);

	void processResponse(QVariantMap &response);

	QString m_auth;

	QNetworkAccessManager *m_nam;

	QNetworkReply *m_reply;

	QList<FoursquareVenue> m_venues;

	enum SearchStatus m_status;

};

#endif /* _FOURSQUARE_VENUE_SEARCH_H_ */
