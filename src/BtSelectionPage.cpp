/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtSelectionPage.h"
#include "BtSelectionPageListCellCreator.h"
#include "BtSelectionPageExistingListModel.h"
#include "BtSelectionPageScanListModel.h"
#include "BluezSupplicant.h"
#include "LabelOrList.h"

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new BtSelectionPageListCellCreator;
}

BtSelectionPage::BtSelectionPage(BluezSupplicant *bluez,
				 enum BtSelectionPage::Type type,
				 QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_model(0)
{
	m_model = (type == SelectFromExisting)
		? static_cast<BtSelectionPageListModel *>(new BtSelectionPageExistingListModel(bluez, this))
		: static_cast<BtSelectionPageListModel *>(new BtSelectionPageScanListModel(bluez, this));
}

BtSelectionPage::~BtSelectionPage(void)
{
}

void BtSelectionPage::createContent(void)
{
	createCommonContent(m_model,
			    _getCreator,
			    tr("<h1>No Bluetooth devices to select</h1>"),
			    tr("<big>Select the device to use</big>"),
			    false,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(deviceSelected(const QModelIndex &)));
}

void BtSelectionPage::deviceSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->device(which)));
}

