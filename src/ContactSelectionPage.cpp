/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactSelectionPage.h"
#include "ContactSelectionPageListCellCreator.h"
#include "ContactSelectionPageListModel.h"
#include "ContactDetailPicker.h"
#include "LabelOrList.h"

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new ContactSelectionPageListCellCreator;
}

ContactSelectionPage::ContactSelectionPage(QContactManager *manager,
					   QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_model(new ContactSelectionPageListModel(manager, this))
{
}

ContactSelectionPage::~ContactSelectionPage(void)
{
}

void ContactSelectionPage::createContent(void)
{
	createCommonContent(m_model,
			    _getCreator,
			    tr("<h1>No contact to select</h1>"),
			    tr("<big>Select the contact to use</big>"),
			    true,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(contactSelected(const QModelIndex &)));
}

void ContactSelectionPage::contactSelected(const QModelIndex &which)
{
	QContact contact = m_model->contact(which);
	ContactDetailPicker *picker = new ContactDetailPicker(contact);
	connect(picker, SIGNAL(ready(const QContact)),
		this, SLOT(contactPicked(const QContact)));
	connect(picker, SIGNAL(dismiss()),
		this, SLOT(dismiss()));
	picker->appear(MSceneWindow::DestroyWhenDismissed);
}

void ContactSelectionPage::contactPicked(const QContact which)
{
	dismiss();
	Q_EMIT(selected(which));
}
