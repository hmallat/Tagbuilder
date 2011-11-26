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
}

CreatePage::~CreatePage(void)
{
}

void CreatePage::createContent(void)
{
	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
	centralWidget()->setLayout(layout);

	MLabel *label = new MLabel(tr("<big>Select the type of tag "
				      "to create</big>"));
	label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	label->setAlignment(Qt::AlignLeft);
	layout->addCornerAnchors(label, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);

	MList *list = new MList();
	TagTypeListCellCreator *creator = new TagTypeListCellCreator;
	list->setCellCreator(creator);
	m_model = new TagTypeListModel;
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
		this, SLOT(tagTypeSelected(const QModelIndex &)));
}

void CreatePage::tagTypeSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model != 0 ? m_model->name(which) : ""));
}

