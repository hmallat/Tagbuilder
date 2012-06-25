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

#ifndef _FOURSQUARE_VENUE_SELECTION_PAGE_LIST_MODEL_H_
#define _FOURSQUARE_VENUE_SELECTION_PAGE_LIST_MODEL_H_

#include <MAbstractItemModel>

#include "Util.h"
#include "FoursquareVenue.h"
#include "FoursquareVenueSearch.h"

#include <QMap>
#include <QGeoPositionInfoSource>
#include <QGeoPositionInfo>
#include <MLocaleBuckets>

QTM_USE_NAMESPACE;

class FoursquareVenueSelectionPageListModel : public MAbstractItemModel
{

	Q_OBJECT;

public:

	FoursquareVenueSelectionPageListModel(QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	const FoursquareVenue venue(const QModelIndex &index) const;

	bool fetch(const QString auth);

Q_SIGNALS:

	void positionFound(void);

	void ready(void);

protected Q_SLOTS:

	void positionUpdated(const QGeoPositionInfo &update);

	void positionUpdateTimeout(void);

	void venueSearchCompleted(void);

protected:

	void updateResults(const QList<FoursquareVenue> &results);

	enum { Idle, Locating, Fetching } m_state;
	
	QGeoPositionInfoSource *m_source;

	QMap<QString, FoursquareVenue> m_venues;

	MLocaleBuckets m_buckets;

	FoursquareVenueSearch *m_search;

	QString m_auth;

};

#endif /* _FOURSQUARE_VENUE_SELECTION_PAGE_LIST_MODEL_H_ */
