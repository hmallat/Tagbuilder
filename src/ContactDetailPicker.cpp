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

#include "ContactDetailPicker.h"
#include "ContactDetailPickerListCellCreator.h"
#include "ContactDetailPickerListModel.h"
#include "LabelOrList.h"
#include "Util.h"

#include <QContactDetailDefinition>
#include <QContactManager>
#include <QItemSelectionModel>

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MList>

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new ContactDetailPickerListCellCreator;
}

ContactDetailPicker::ContactDetailPicker(const QContact contact,
					 Util::ContactDetails details,
					 bool singleDetailOnly,
					 QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_contact(contact),
	  m_model(new ContactDetailPickerListModel(details,
						   this)),
	  m_single(singleDetailOnly)
{
	m_model->setContact(m_contact);
}

ContactDetailPicker::~ContactDetailPicker(void)
{
}

void ContactDetailPicker::createContent(void)
{
	createCommonContent(m_model,
			    _getCreator,
			    tr("No contact details to select from"),
			    tr("Select contact details"),
			    true,
			    m_single == true ? false : true);

	if (m_single == true) {
		connect(this, SIGNAL(clicked(const QModelIndex)),
			this, SLOT(detailClicked(const QModelIndex)));
	} else {
		connect(this, SIGNAL(done()), this, SLOT(pickingDone()));
		selectAll(m_model);
	}
}

void ContactDetailPicker::pickingDone(void)
{
	QContactManager manager;

	QItemSelectionModel *selection = m_list->selectionModel();
	if (selection == NULL) {
		mDebug(__func__) << "No selection model. ";
		return;
	}

	/* Construct a new contact out of the selected details 
	   and give that as the result of the pick. */
	QContact picked;
	QModelIndexList list = selection->selectedIndexes();
	for (int i = 0; i < list.length(); i++) {
		QContactDetail detail = m_model->detail(list[i]);
		/* TODO: will this cause problems, does the 
		   damn qcontact make a copy or not? */
		picked.saveDetail(&detail);
	}
	manager.synthesizeContactDisplayLabel(&picked);

	dismiss();
	Q_EMIT(contactPicked(picked));
}

void ContactDetailPicker::detailClicked(const QModelIndex which)
{
	QContactManager manager;

	QContact picked;
	QContactDetail detail = m_model->detail(which);
	/* TODO: will this cause problems, does the 
	   damn qcontact make a copy or not? */
	picked.saveDetail(&detail);

	manager.synthesizeContactDisplayLabel(&picked);

	dismiss();
	Q_EMIT(contactPicked(picked));
}
