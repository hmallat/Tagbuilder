/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CreatePage.h"
#include "TextPage.h"
#include "TagTypeListModel.h"
#include "TagTypeListCellCreator.h"

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MList>
#include <MContentItem>
#include <MPannableViewport>

/* TODO: own buttons for SMS and CALLTO? What about other URI types? 
   Codewise could use the same page anyway...
 */

CreatePage::CreatePage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_model(NULL)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

CreatePage::~CreatePage(void)
{
}

void CreatePage::createContent(void)
{
	MAction *cancelAction = new MAction(tr("Cancel"), this);
	cancelAction->setLocation(MAction::ToolBarLocation);
	connect(cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(cancelAction);

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
	centralWidget()->setLayout(layout);

	MWidgetController *header = new MWidgetController();
	header->setStyleName("CommonHeaderPanel");
	header->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	layout->addCornerAnchors(header, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);

	MLabel *label = new MLabel(tr("Create a new tag"), header);
	label->setStyleName("CommonHeader");
	label->setAlignment(Qt::AlignLeft);

	MList *list = new MList();
	TagTypeListCellCreator *creator = new TagTypeListCellCreator;
	list->setCellCreator(creator);
	m_model = new TagTypeListModel(list);
	list->setItemModel(m_model);

	MPannableViewport *view = new MPannableViewport();
	view->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	view->setPanDirection(Qt::Vertical);
	view->setMinimumSize(100, 100);
	view->setWidget(list);
	layout->addCornerAnchors(view, Qt::TopLeftCorner,
				 header, Qt::BottomLeftCorner);
	layout->addCornerAnchors(view, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);

	connect(list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(tagTypeSelected(const QModelIndex &)));
}

void CreatePage::tagTypeSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model != 0 ? m_model->name(which) : ""));
}

