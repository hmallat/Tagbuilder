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

#ifndef _FOURSQUARE_VENUE_SELECTION_PAGE_H_
#define _FOURSQUARE_VENUE_SELECTION_PAGE_H_

#include "SelectionPage.h"

#include "Util.h"

QTM_USE_NAMESPACE;

class FoursquareAuthStorage;
class FoursquareVenue;
class FoursquareVenueSelectionPageListModel;
class LabeledTextEdit;
class MButton;

#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>

class FoursquareVenueSelectionPage : public SelectionPage
{
	Q_OBJECT;

public:

	FoursquareVenueSelectionPage(QGraphicsItem *parent = 0);

	virtual ~FoursquareVenueSelectionPage(void);

        virtual void createContent(void);

Q_SIGNALS:

	void created(void);

	void selected(const FoursquareVenue &which);

private Q_SLOTS:
	
	void activate(void);

	void positionUpdated(const QGeoPositionInfo &update);

	void positionUpdateTimeout(void);

	void authenticationComplete(const QString token);

	void authenticationFailed(void);

	void searchNearby(void);

	void searchLocation(void);

	void itemsReady(void);

	void venueSelected(const QModelIndex &which);

private:

	Q_DISABLE_COPY(FoursquareVenueSelectionPage);

	void authenticate(void);

	QGeoPositionInfoSource *m_geosource;

	QGeoPositionInfo m_geoinfo;

	FoursquareVenueSelectionPageListModel *m_model;

	FoursquareAuthStorage *m_storage;

	LabeledTextEdit *m_location;

	MButton *m_here;

};

#endif /* _FOURSQUARE_VENUE_SELECTION_PAGE_H_ */
