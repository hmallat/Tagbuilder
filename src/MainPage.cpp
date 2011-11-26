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

#include <QGraphicsLinearLayout>
#include <MAction>
#include <MLabel>
#include <MList>
#include <MContentItem>

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

	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget()->setLayout(layout);

	refreshList();
}

void MainPage::createTagButtons(QGraphicsLinearLayout *layout)
{
	MList *list = new MList();
	TagListCellCreator *creator = new TagListCellCreator;
	list->setCellCreator(creator);
	TagListModel *model = new TagListModel;
	list->setItemModel(model);
	layout->addItem(list);
}

void MainPage::refreshList(void)
{
	QGraphicsLinearLayout *layout = 
		static_cast<QGraphicsLinearLayout *>
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
		layout->addItem(nothing);
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
