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
	  m_state(FoursquareVenueSelectionPageListModel::Idle),
	  m_source(0),
	  m_venues(),
	  m_buckets(),
	  m_search(0),
	  m_auth("")
{
	setGrouped(true);
}

bool FoursquareVenueSelectionPageListModel::fetch(const QString auth)
{
	m_auth = auth;

	if (m_source == NULL) {
		m_source = QGeoPositionInfoSource::createDefaultSource(this);
		if (m_source == NULL) {
			mDebug(__func__) << "Cannot set up position source. ";
			return false;
		}
	}

	if (m_search != NULL) {
		delete m_search;
		m_search = NULL;
	}

	mDebug(__func__) << "Starting position lookup. ";
	connect(m_source, SIGNAL(positionUpdated(const QGeoPositionInfo &)),
		this, SLOT(positionUpdated(const QGeoPositionInfo &)));
	connect(m_source, SIGNAL(updateTimeout()),
		this, SLOT(positionUpdateTimeout()));
	m_source->requestUpdate(60*1000);
	m_state = Locating;

	return true;
}

void FoursquareVenueSelectionPageListModel::positionUpdated(const QGeoPositionInfo &update)
{
	mDebug(__func__) 
		<< "Position lookup done, we're at "
		<< update.coordinate().latitude() << "," 
		<< update.coordinate().longitude() << ". ";
	Q_EMIT(positionFound());

	m_search = new FoursquareVenueSearch(m_auth, this);
	connect(m_search, SIGNAL(searchComplete()),
		this, SLOT(venueSearchCompleted()));
	m_state = Fetching;
	if (m_search->venuesByCoordinates(update.coordinate().latitude(),
					  update.coordinate().longitude()) == false) {
		mDebug(__func__) << "Can't lookup venues. ";
		QList<FoursquareVenue> empty;
		updateResults(empty);
		m_state = Idle;
		Q_EMIT(ready());
	}
}

void FoursquareVenueSelectionPageListModel::positionUpdateTimeout(void)
{
	mDebug(__func__) << "Position lookup timeout. ";

	QList<FoursquareVenue> empty;
	updateResults(empty);
	m_state = Idle;
	Q_EMIT(ready());
}

void FoursquareVenueSelectionPageListModel::
venueSearchCompleted(void)
{
	mDebug(__func__) << "Venue search complete. ";
	updateResults(m_search->results());
	delete m_search;
	m_search = NULL;
	m_state = Idle;
	Q_EMIT(ready());
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
		parameters 
			<< venue.displayLabel()
			<< "icon-m-content-avatar-placeholder" /* TODO */;
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
