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
			    tr("No contacts to select from"),
			    tr("Select contact"),
			    true,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(contactSelected(const QModelIndex &)));

	connect(m_model, SIGNAL(ready()), this, SLOT(clearBusy()));
	if (m_model->fetch() == true) {
		setBusy();
	} else {
		/* TODO: message box or some other indication */
	}
}

void ContactSelectionPage::contactSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->contact(which)));
}
