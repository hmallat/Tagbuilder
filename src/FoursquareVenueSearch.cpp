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

#include "FoursquareVenueSearch.h"

#include <MDebug>

FoursquareVenueSearch::FoursquareVenueSearch(const QString auth, 
					     QObject *parent) :
	QObject(parent),
	m_auth(auth),
	m_nam(new QNetworkAccessManager(this)),
	m_request(0)
{
        connect(m_nam, SIGNAL(finished(QNetworkReply *)),
                this, SLOT(requestFinished(QNetworkReply *)));
}

FoursquareVenueSearch::~FoursquareVenueSearch(void)
{
}

bool FoursquareVenueSearch::venuesByCoordinates(double lat, double lon)
{
	if (m_request != 0) {
		delete m_request;
		m_request = 0;
	}

	m_venues.clear();

        QUrl query("https://api.foursquare.com/v2/venues/search");

	query.addQueryItem("ll", QString("%1,%2").arg(lat).arg(lon));
	query.addQueryItem("oauth_token", m_auth);

        m_request = new QNetworkRequest(query);
	m_nam->get(*m_request);

	return true;
}

void FoursquareVenueSearch::requestFinished(QNetworkReply *reply)
{
        if (reply->error() == QNetworkReply::NoError) {
                QString contents(reply->readAll());
                mDebug(__func__) << "Success: " << contents;
		/* TODO: JSON handling */
		m_venues << FoursquareVenue("1234", "Dummy one");
		m_venues << FoursquareVenue("5678", "Made up second");
		m_venues << FoursquareVenue("9999", "Third fake");
	} else {
		/* TODO: handle stale/invalid auth token error somehow,
		   eg add parameter to signal */
                QString contents(reply->readAll());
		mDebug(__func__) << "Error: " << reply->error();
                mDebug(__func__) << "Error: " << contents;
	}

	Q_EMIT(searchComplete());
}

const QList<FoursquareVenue> FoursquareVenueSearch::results(void)
{
	return m_venues;
}
