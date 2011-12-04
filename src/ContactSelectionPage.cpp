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

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MList>
#include <MContentItem>
#include <MPannableViewport>

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
	createCommonContent(tr("<big>Select the contact to use</big>"));

	ContactSelectionPageListCellCreator *creator = 
		new ContactSelectionPageListCellCreator;
	m_list->setCellCreator(creator);
	m_list->setItemModel(m_model);
	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(contactSelected(const QModelIndex &)));
}

void ContactSelectionPage::contactSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->contact(which)));
}
