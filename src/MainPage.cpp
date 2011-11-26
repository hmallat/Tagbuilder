/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "MainPage.h"
#include "CreatePage.h"
#include "TextPage.h"
#include "TagStorage.h"
#include "Tag.h"
#include "TagListModel.h"
#include "TagListCellCreator.h"

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MList>
#include <MContentItem>
#include <MPannableViewport>

MainPage::MainPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_tagButtons(NULL)
{
}

MainPage::~MainPage(void)
{
}

void MainPage::createContent(void)
{
	setPannable(false);

	MAction *newAction = new MAction("icon-m-toolbar-add", 
					 tr("Create a new tag"),
					 this);
	newAction->setLocation(MAction::ToolBarLocation | 
			       MAction::ApplicationMenuLocation);
 	connect(newAction, SIGNAL(triggered()),
 		this, SLOT(createTag()));
	addAction(newAction);

	MAction *aboutAction = new MAction(tr("About..."), this);
	aboutAction->setLocation(MAction::ApplicationMenuLocation);
 	connect(aboutAction, SIGNAL(triggered()),
 		this, SLOT(showAbout()));
	addAction(aboutAction);

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
	centralWidget()->setLayout(layout);

	refreshList();
}

void MainPage::createTagButtons(QGraphicsAnchorLayout *layout)
{
	MLabel *label = new MLabel(tr("<big>Stored tags</big>"));
	label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	label->setAlignment(Qt::AlignLeft);
	layout->addCornerAnchors(label, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);

	MList *list = new MList();
	TagListCellCreator *creator = new TagListCellCreator;
	list->setCellCreator(creator);
	TagListModel *model = new TagListModel;
	list->setItemModel(model);

	MPannableViewport *view = new MPannableViewport();
	view->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	view->setPanDirection(Qt::Vertical);
	view->setMinimumSize(100, 100);
	view->setWidget(list);
	layout->addCornerAnchors(view, Qt::TopLeftCorner,
				 label, Qt::BottomLeftCorner);
	layout->addCornerAnchors(view, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);
}

void MainPage::refreshList(void)
{
	QGraphicsAnchorLayout *layout = 
		static_cast<QGraphicsAnchorLayout *>
		(centralWidget()->layout());

	while (layout->count() > 0) {
		QGraphicsLayoutItem *item = layout->itemAt(0);
		layout->removeAt(0);
		delete item;
	}

	if (TagStorage::storedTags().length() == 0) {
		MLabel *nothing = new MLabel(tr("<h1>You don't have any "
						"stored tags currently"
						"</h1>"));
		nothing->setAlignment(Qt::AlignCenter);
		nothing->setWordWrap(true);
		layout->addAnchor(nothing, Qt::AnchorTop,
				  layout, Qt::AnchorTop);
		layout->addAnchor(nothing, Qt::AnchorBottom,
				  layout, Qt::AnchorBottom);
		layout->addAnchor(nothing, Qt::AnchorLeft,
				  layout, Qt::AnchorLeft);
		layout->addAnchor(nothing, Qt::AnchorRight,
				  layout, Qt::AnchorRight);
		
	} else {
		createTagButtons(layout);
	}
}

void MainPage::createTag(void)
{
	CreatePage *page = new CreatePage();
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(disappeared(void)),
		this, SLOT(refreshList(void)));
}

void MainPage::showAbout(void)
{
}

void MainPage::tagSelected(int which)
{
	Tag *tag = TagStorage::storedTags().at(which);
	TextPage *page = new TextPage(tag);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(disappeared(void)),
		this, SLOT(refreshList(void)));
}
