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

#include "ContactSelectionPage.h"
#include "ContactSelectionPageListCellCreator.h"
#include "ContactSelectionPageListModel.h"
#include "LabelOrList.h"

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new ContactSelectionPageListCellCreator;
}

ContactSelectionPage::ContactSelectionPage(QContactManager *manager,
					   Util::ContactDetails 
					   requiredDetails,
					   QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_model(new ContactSelectionPageListModel(manager, 
						    requiredDetails, 
						    this))
{
}

ContactSelectionPage::~ContactSelectionPage(void)
{
}

void ContactSelectionPage::createContent(void)
{
	createCommonContent(m_model,
			    _getCreator,
			    tr("Fetching contacts"),
			    tr("Select contact"),
			    true,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(contactSelected(const QModelIndex &)));

	connect(m_model, SIGNAL(ready()), this, SLOT(itemsReady()));
	if (m_model->fetch() == true) {
		setBusy();
	} else {
		m_list->setLabel(tr("Cannot retrieve contacts"));
	}
}

void ContactSelectionPage::itemsReady(void)
{
	clearBusy();
	m_list->setLabel(tr("No contacts to select from"));
}

void ContactSelectionPage::contactSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->contact(which)));
}
