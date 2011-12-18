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

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
	centralWidget()->setLayout(layout);

	MLabel *titleLabel = new MLabel(title);
	titleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	titleLabel->setAlignment(Qt::AlignLeft);
	layout->addCornerAnchors(titleLabel, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);

	m_list = new LabelOrList(itemModel,
				 getCreator,
				 label,
				 groupedList,
				 multiSelect);

	layout->addCornerAnchors(m_list, Qt::TopLeftCorner,
				 titleLabel, Qt::BottomLeftCorner);
	layout->addCornerAnchors(m_list, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);

}
