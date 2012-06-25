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
#include "LabelOrList.h"
#include "FoursquareAuthStorage.h"
#include "FoursquareAuthPage.h"

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new FoursquareVenueSelectionPageListCellCreator;
}

FoursquareVenueSelectionPage::FoursquareVenueSelectionPage(QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_model(new FoursquareVenueSelectionPageListModel(this)),
	  m_storage(FoursquareAuthStorage::storage())
{
	connect(m_model, SIGNAL(positionFound()), this, SLOT(positionFound()));
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
	createCommonContent(m_model,
			    _getCreator,
			    tr("Not authenticated"),
			    tr("Select venue"),
			    true,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(venueSelected(const QModelIndex &)));

	Q_EMIT(created());
}

void FoursquareVenueSelectionPage::activate(void)
{
	if (m_storage->isEmpty()) { 
		/* No authentication token present, get it */
		authenticate();
	} else {
		fetch();
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
	fetch();
}

void FoursquareVenueSelectionPage::authenticationFailed(void)
{
}

void FoursquareVenueSelectionPage::fetch(void)
{
	m_list->setLabel(tr("Locating current position"));
	if (m_model->fetch(m_storage->get()) == true) {
		setBusy();
	} else {
		m_list->setLabel(tr("Cannot retrieve venues"));
	}
}

void FoursquareVenueSelectionPage::positionFound(void)
{
	m_list->setLabel(tr("Retrieving venues"));
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
