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

#include "FoursquareVenueSelectionPageListModel.h"
#include "FoursquareVenue.h"

#include <QStringList>

#include <MDebug>

FoursquareVenueSelectionPageListModel::
FoursquareVenueSelectionPageListModel(QObject *parent)
	: MAbstractItemModel(parent),
	  m_venues(),
	  m_buckets(),
	  m_search(0),
	  m_auth("")
{
	setGrouped(true);
}

bool FoursquareVenueSelectionPageListModel::fetch(const QString auth,
						  double lat,
						  double lon)
{
	m_auth = auth;

	if (m_search != 0) {
		delete m_search;
		m_search = NULL;
	}

	m_search = new FoursquareVenueSearch(m_auth, this);
	connect(m_search, SIGNAL(searchComplete()),
		this, SLOT(venueSearchCompleted()));
	if (m_search->venuesByCoordinates(lat, lon) == false) {
		mDebug(__func__) << "Can't lookup venues. ";
		return false;
	}

	return true;
}

bool FoursquareVenueSelectionPageListModel::fetch(const QString auth,
						  const QString loc)
{
	m_auth = auth;

	if (m_search != 0) {
		delete m_search;
		m_search = NULL;
	}

	m_search = new FoursquareVenueSearch(m_auth, this);
	connect(m_search, SIGNAL(searchComplete()),
		this, SLOT(venueSearchCompleted()));
	if (m_search->venuesByLocation(loc) == false) {
		mDebug(__func__) << "Can't lookup venues. ";
		return false;
	}

	return true;
}

void FoursquareVenueSelectionPageListModel::
venueSearchCompleted(void)
{
	mDebug(__func__) << "Venue search complete. ";
	updateResults(m_search->results());
	enum FoursquareVenueSearch::SearchStatus status = m_search->status();
	m_search->deleteLater();
	m_search = 0;
	Q_EMIT(ready(status));
}

void FoursquareVenueSelectionPageListModel::
updateResults(const QList<FoursquareVenue> &results)
{
	QStringList bucketEntries;
	QMap<QString, FoursquareVenue> insertions;

	for (int i = 0; i < results.length(); i++) {
		FoursquareVenue venue = results[i];
		if (venue.displayLabel() == "") {
			continue;
		}

		if (insertions.contains(venue.displayLabel())) {
			mDebug(__func__) << venue.displayLabel() 
					 << " is duplicate, skipping. ";
			continue;
		}

		insertions[venue.displayLabel()] = venue;
		bucketEntries << venue.displayLabel();

	}
	if (bucketEntries.length() == 0) {
		mDebug(__func__) << "Nothing to be inserted. ";
		/* TODO: indicate in UI */
		goto done;
	}

done:
	Q_EMIT(layoutAboutToBeChanged());

	if (m_buckets.bucketCount() > 0) {
		beginRemoveRows(QModelIndex(), 0, m_buckets.bucketCount() - 1,
				Util::animateLists);
		m_buckets.clear();
		m_venues.clear();
		endRemoveRows();
	}

	if (bucketEntries.length() > 0) {
		beginInsertRows(QModelIndex(), 0, bucketEntries.length() - 1,
				Util::animateLists);
		m_buckets.setItems(bucketEntries);
		m_venues = insertions;
		endInsertRows();
	}

	Q_EMIT(layoutChanged());
}

int FoursquareVenueSelectionPageListModel::groupCount(void) const
{
	return m_buckets.bucketCount();
}

int FoursquareVenueSelectionPageListModel::rowCountInGroup(int group) const
{
	return m_buckets.bucketSize(group);
}

QString FoursquareVenueSelectionPageListModel::groupTitle(int group) const
{
	return m_buckets.bucketName(group);
}

QVariant FoursquareVenueSelectionPageListModel::itemData(int row, 
							 int group, 
							 int role) const
{
	(void)role;

	QStringList parameters;
	QString label = m_buckets.bucketContent(group)[row];
	if (m_venues.contains(label)) {
		FoursquareVenue venue = m_venues[label];
		QString dist = "";
		if (venue.distance() != FoursquareVenue::DISTANCE_UNKNOWN)
			dist = QString("%1 m").arg(venue.distance());
		parameters 
			<< venue.displayLabel()
			<< dist;
	} else {
		parameters 
			<< ""
			<< "";
	}

	return qVariantFromValue(parameters);
}

const FoursquareVenue FoursquareVenueSelectionPageListModel::
venue(const QModelIndex &index) const
{
	int row = index.row();
	int group = index.parent().row();

	QString label = m_buckets.bucketContent(group)[row];
	if (m_venues.contains(label)) {
		return m_venues[label];
	} else {
		return FoursquareVenue();
	}
}
