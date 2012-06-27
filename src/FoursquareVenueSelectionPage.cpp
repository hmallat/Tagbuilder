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

#include "FoursquareVenueSelectionPage.h"
#include "FoursquareVenueSelectionPageListCellCreator.h"
#include "FoursquareVenueSelectionPageListModel.h"
#include "FoursquareVenue.h"
#include "FoursquareAuthStorage.h"
#include "FoursquareAuthPage.h"
#include "LabelOrList.h"
#include "LabeledTextEdit.h"

#include <QGraphicsLinearLayout>
#include <MButton>
#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new FoursquareVenueSelectionPageListCellCreator;
}

FoursquareVenueSelectionPage::FoursquareVenueSelectionPage(QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_geosource(QGeoPositionInfoSource::createDefaultSource(this)),
	  m_model(new FoursquareVenueSelectionPageListModel(this)),
	  m_storage(FoursquareAuthStorage::storage())
{
	if (m_geosource == NULL) {
		mDebug(__func__) << "Cannot set up geoposition source. ";
	} else {
		mDebug(__func__) << "Starting geoposition lookup. ";
		connect(m_geosource, 
			SIGNAL(positionUpdated(const QGeoPositionInfo &)),
			this, 
			SLOT(positionUpdated(const QGeoPositionInfo &)));
		connect(m_geosource, 
			SIGNAL(updateTimeout()),
			this, 
			SLOT(positionUpdateTimeout()));
		m_geosource->startUpdates();
	}

	connect(m_model, SIGNAL(ready()), this, SLOT(itemsReady()));

	connect(this, SIGNAL(created()), 
		this, SLOT(activate()),
		Qt::QueuedConnection);
}

FoursquareVenueSelectionPage::~FoursquareVenueSelectionPage(void)
{
}

void FoursquareVenueSelectionPage::createContent(void)
{
	m_location = new LabeledTextEdit(tr("Search"),
					 LabeledTextEdit::SingleLineEditOnly);
	m_location->setPrompt(tr("Enter location"));
	connect(m_location, SIGNAL(editCompleted()),
		this, SLOT(searchLocation(void)));

	m_here = new MButton(tr("Nearby venues"));
	m_here->setEnabled(false);
	connect(m_here, SIGNAL(clicked()),
		this, SLOT(searchNearby(void)));

	QGraphicsLinearLayout *sub_layout = 
		new QGraphicsLinearLayout(Qt::Vertical);
	sub_layout->setSizePolicy(QSizePolicy::Preferred, 
				  QSizePolicy::Preferred);
	sub_layout->addItem(m_location);
	sub_layout->setAlignment(m_location, Qt::AlignCenter);
	sub_layout->addItem(m_here);
	sub_layout->setAlignment(m_here, Qt::AlignCenter);

	createCommonContent(m_model,
			    _getCreator,
			    tr(""),
			    tr("Select venue"),
			    true,
			    false,
			    sub_layout);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(venueSelected(const QModelIndex &)));

	Q_EMIT(created());
}

void FoursquareVenueSelectionPage::positionUpdated(const QGeoPositionInfo &update)
{
	mDebug(__func__) 
		<< "Position lookup done, we're at "
		<< update.coordinate().latitude() << "," 
		<< update.coordinate().longitude() << ". ";
	m_geoinfo = update;
	m_here->setEnabled(true);
}

void FoursquareVenueSelectionPage::positionUpdateTimeout(void)
{
	mDebug(__func__) << "Position lookup timeout. ";
}

void FoursquareVenueSelectionPage::activate(void)
{
	if (m_storage->isEmpty()) { 
		/* No authentication token present, get it */
		m_list->setLabel(tr("Not authenticated"));
		authenticate();
	} else {
		m_list->setLabel(tr("Search for venues"));
	}
}

void FoursquareVenueSelectionPage::authenticate(void)
{
	FoursquareAuthPage *page =
		new FoursquareAuthPage();
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(authenticationComplete(const QString)),
		this, SLOT(authenticationComplete(const QString)));
	connect(page, SIGNAL(authenticationFailed()),
		this, SLOT(authenticationFailed()));
}

void FoursquareVenueSelectionPage::authenticationComplete(const QString token)
{
	m_storage->set(token);
	m_list->setLabel(tr("Search for venues"));
}

void FoursquareVenueSelectionPage::authenticationFailed(void)
{
	/* TODO */
}

void FoursquareVenueSelectionPage::searchNearby(void)
{
	setBusy();
	m_list->setLabel(tr("Searching nearby venues"));
	m_model->fetch(m_storage->get(),
		       m_geoinfo.coordinate().latitude(),
		       m_geoinfo.coordinate().longitude());
}

void FoursquareVenueSelectionPage::searchLocation(void)
{
	QString loc = m_location->contents();
	if (loc == "")
		return;

	setBusy();
	m_list->setLabel(tr("Searching venues near %1").arg(loc));
	m_model->fetch(m_storage->get(), loc);
}

void FoursquareVenueSelectionPage::itemsReady(void)
{
	clearBusy();
	m_list->setLabel(tr("No venues to select from"));
}

void FoursquareVenueSelectionPage::venueSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->venue(which)));
}

