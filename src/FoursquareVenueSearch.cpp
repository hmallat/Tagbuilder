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

#include <qjson/parser.h>
#include <MDebug>

FoursquareVenueSearch::FoursquareVenueSearch(const QString auth, 
					     QObject *parent) :
	QObject(parent),
	m_auth(auth),
	m_nam(new QNetworkAccessManager(this)),
	m_reply(0),
	m_status(FoursquareVenueSearch::Incomplete)
{
}

FoursquareVenueSearch::~FoursquareVenueSearch(void)
{
	if (m_reply != 0) {
		delete m_reply;
		m_reply = 0;
		m_status = Incomplete;
	}

}

bool FoursquareVenueSearch::search(QUrl query)
{
	mDebug(__func__) << query;

	m_venues.clear();

	if (m_reply != 0) {
		delete m_reply;
		m_reply = 0;
		m_status = Incomplete;
	}

        m_reply = m_nam->get(QNetworkRequest(query));
        connect(m_reply, SIGNAL(finished(void)),
                this, SLOT(requestFinished(void)));

	return true;
}

bool FoursquareVenueSearch::venuesByCoordinates(double lat, double lon)
{
        QUrl query("https://api.foursquare.com/v2/venues/search");
	query.addQueryItem("ll", QString("%1,%2").arg(lat).arg(lon));
	query.addQueryItem("oauth_token", m_auth);
	query.addQueryItem("v", BUILDDATE);

	return search(query);
}

bool FoursquareVenueSearch::venuesByLocation(const QString loc)
{
        QUrl query("https://api.foursquare.com/v2/venues/search");
	query.addQueryItem("near", loc);
	query.addQueryItem("oauth_token", m_auth);
	query.addQueryItem("v", BUILDDATE);

	return search(query);
}

static void debugVariantMap(const QString name, const QVariantMap &map)
{
	mDebug(__func__) << name << ":::";

	for (QVariantMap::const_iterator i = map.constBegin();
	     i != map.constEnd();
	     i++) {
		QString k = i.key();
		QVariant v = i.value();
		mDebug(__func__) << "key " << k << ", value " << v;
	}

	mDebug(__func__) << ":::";
}

void FoursquareVenueSearch::processResponse(QVariantMap &response)
{
	debugVariantMap("response", response);
	QList<QVariant> venues = response["venues"].toList();
	foreach(QVariant venue, venues) {
		QVariantMap venueMap = venue.toMap();
		debugVariantMap("venue", venueMap);
		mDebug(__func__) << venueMap["id"];
		mDebug(__func__) << venueMap["name"];

		unsigned long long dist = FoursquareVenue::DISTANCE_UNKNOWN;
		QVariantMap locationMap = venueMap["location"].toMap();
		if (locationMap.contains("distance"))
			dist = locationMap["distance"].toULongLong();

		m_venues << 
			FoursquareVenue(venueMap["id"].toString(),
					venueMap["name"].toString(),
					dist);
	}

	mDebug(__func__) << m_venues.size() << " venues found. ";
}

void FoursquareVenueSearch::requestFinished(void)
{
        if (m_reply != 0 && m_reply->error() == QNetworkReply::NoError) {
		m_status = Ok;

		QByteArray contents = m_reply->readAll();
                mDebug(__func__) << "Success: " << contents;

		QJson::Parser parser;
		bool valid = false;

		QVariantMap results = parser.parse(contents, &valid).toMap();
		if (valid == true) {
			debugVariantMap("results", results);
			QVariantMap response = results["response"].toMap();
			processResponse(response);
		} else {
			mDebug(__func__) << "Invalid response. ";
		}

	} else {
		m_status = Error;

		QString contents(m_reply != 0 ? m_reply->readAll() : "");
		mDebug(__func__) << "Error: " << (m_reply != 0 
						  ? m_reply->error()
						  : QNetworkReply::UnknownNetworkError);
                mDebug(__func__) << "Error: " << contents;

		if (m_reply != 0 && 
		    m_reply->error() == QNetworkReply::AuthenticationRequiredError) {
			m_status = AuthenticationError;
		}
	}

	m_reply->deleteLater();
	m_reply = 0;

	Q_EMIT(searchComplete());
}

const QList<FoursquareVenue> FoursquareVenueSearch::results(void)
{
	mDebug(__func__) << m_venues.size() << " venues in results. ";
	return m_venues;
}

enum FoursquareVenueSearch::SearchStatus FoursquareVenueSearch::status(void)
{
	return m_status;
}

