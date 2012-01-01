/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "SelectionPage.h"
#include "LabelOrList.h"

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
	centralWidget()->setLayout(layout);

	MWidgetController *header = new MWidgetController();
	header->setStyleName("CommonHeaderPanel");
	header->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	layout->addCornerAnchors(header, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);

	MLabel *titleLabel = new MLabel(title, header);
	titleLabel->setStyleName("CommonHeader");
	titleLabel->setAlignment(Qt::AlignLeft);

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
				 header, Qt::BottomLeftCorner);
	layout->addCornerAnchors(m_list, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);

}
