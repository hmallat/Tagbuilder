/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "SelectionPage.h"
#include "LabelOrList.h"
#include "ViewHeader.h"

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MContentItem>

SelectionPage::SelectionPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_list(0)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

SelectionPage::~SelectionPage(void)
{
}

void SelectionPage::createContent(void)
{
}

void SelectionPage::createCommonContent(QAbstractItemModel *itemModel,
					MAbstractCellCreator<MWidgetController> *(*getCreator)(void),
					const QString label,
					const QString title, 
					bool groupedList,
					bool multiSelect)
{
	MApplicationPage::createContent();
	setStyleName("CommonApplicationPage");
	setPannable(false);

	MAction *cancelAction = new MAction(tr("Cancel"), this);
	cancelAction->setLocation(MAction::ToolBarLocation);
	connect(cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(cancelAction);

	if (multiSelect == true) {
		MAction *doneAction = new MAction(tr("Done"), this);
		doneAction->setLocation(MAction::ToolBarLocation);
		connect(doneAction, SIGNAL(triggered()),
			this, SIGNAL(done()));
		addAction(doneAction);
	}

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
	layout->setContentsMargins(0, 0, 0, 0);

	m_header = new ViewHeader(title);

	layout->addCornerAnchors(m_header, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);
	layout->addCornerAnchors(m_header, Qt::TopRightCorner,
				 layout, Qt::TopRightCorner);

	m_list = new LabelOrList(itemModel,
				 getCreator,
				 label,
				 groupedList,
				 multiSelect);
	if (multiSelect == false) {
		connect(m_list, SIGNAL(itemClicked(const QModelIndex)),
			this, SIGNAL(clicked(const QModelIndex)));
	}

	layout->addCornerAnchors(m_list, Qt::TopLeftCorner,
				 m_header, Qt::BottomLeftCorner);
	layout->addCornerAnchors(m_list, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);


	centralWidget()->setLayout(layout);
}

void SelectionPage::setBusyStatus(bool busy)
{
	if (busy == true) {
		m_header->setBusy();
	} else {
		m_header->clearBusy();
	}
}

