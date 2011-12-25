/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
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
					 QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_contact(contact),
	  m_model(new ContactDetailPickerListModel(m_contact, this))
{
}

ContactDetailPicker::~ContactDetailPicker(void)
{
}

void ContactDetailPicker::createContent(void)
{
	createCommonContent(m_model,
			    _getCreator,
			    tr("<h1>No contact details to select</h1>"),
			    tr("Select contact details"),
			    true,
			    true);

	connect(this, SIGNAL(done()), this, SLOT(pickingDone()));
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
