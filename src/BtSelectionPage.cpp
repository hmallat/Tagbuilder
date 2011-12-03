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

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MList>
#include <MContentItem>
#include <MPannableViewport>

BtSelectionPage::BtSelectionPage(BluezSupplicant *bluez,
				 enum BtSelectionPage::Type type,
				 QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_model(0)
{
	m_model = (type == SelectFromExisting)
		? static_cast<BtSelectionPageListModel *>(new BtSelectionPageExistingListModel(bluez, this))
		: static_cast<BtSelectionPageListModel *>(new BtSelectionPageScanListModel(bluez, this));
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

BtSelectionPage::~BtSelectionPage(void)
{
}

void BtSelectionPage::createContent(void)
{
	MAction *cancelAction = new MAction(tr("Cancel"), this);
	cancelAction->setLocation(MAction::ToolBarLocation);
	connect(cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(cancelAction);

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
	centralWidget()->setLayout(layout);

	MLabel *label = new MLabel(tr("<big>Select the device "
				      "to use</big>"));
	label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	label->setAlignment(Qt::AlignLeft);
	layout->addCornerAnchors(label, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);

	MList *list = new MList();
	BtSelectionPageListCellCreator *creator = 
		new BtSelectionPageListCellCreator;
	list->setCellCreator(creator);
	list->setItemModel(m_model);

	MPannableViewport *view = new MPannableViewport();
	view->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	view->setPanDirection(Qt::Vertical);
	view->setMinimumSize(100, 100);
	view->setWidget(list);
	layout->addCornerAnchors(view, Qt::TopLeftCorner,
				 label, Qt::BottomLeftCorner);
	layout->addCornerAnchors(view, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);

	connect(list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(deviceSelected(const QModelIndex &)));
}

void BtSelectionPage::deviceSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->device(which)));
}



