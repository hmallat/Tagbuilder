/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BluezSupplicant.h"
#include "BtPage.h"
#include "CreatePage.h"
#include "MainPage.h"
#include "TextPage.h"
#include "TagStorage.h"
#include "Tag.h"
#include "TagListModel.h"
#include "TagTypeListModel.h"
#include "TagListCellCreator.h"
#include "UrlPage.h"

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MList>
#include <MContentItem>
#include <MPannableViewport>
#include <MObjectMenu>
#include <MSceneManager>

#include <MDebug>

MainPage::MainPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_objectMenu(NULL),
	  m_longTapIndex(QModelIndex())
{
	m_bluez = new BluezSupplicant(this);
}

MainPage::~MainPage(void)
{
}

void MainPage::createObjectMenuActions(void)
{
	m_objectMenu = new MObjectMenu(this);
	
	MAction *editAction = new MAction(tr("Edit tag"), this);
	m_objectMenu->addAction(editAction);
  	connect(editAction, SIGNAL(triggered()), this, SLOT(editTag()));

	MAction *removeAction = new MAction(tr("Remove tag"), this);
	m_objectMenu->addAction(removeAction);
  	connect(removeAction, SIGNAL(triggered()), this, SLOT(removeTag()));
}

void MainPage::createContent(void)
{
	m_bluez->start();

	setPannable(false);

	createObjectMenuActions();

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
	TagListCellCreator *creator = new TagListCellCreator(this);
	list->setCellCreator(creator);
	TagListModel *model = new TagListModel(list);
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

	connect(list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(tagSelected(const QModelIndex &)));
	connect(list, SIGNAL(itemLongTapped(const QModelIndex &,
					    const QPointF &)),
		this, SLOT(tagLongSelected(const QModelIndex &,
					   const QPointF &)));
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

	if (TagStorage::count() == 0) {
		MLabel *nothing = new MLabel(tr("<h1>You don't have any "
						"stored tags currently"
						"</h1>"));
		nothing->setAlignment(Qt::AlignCenter);
		nothing->setWordWrap(true);
		layout->addCornerAnchors(nothing, Qt::TopLeftCorner,
					 layout, Qt::TopLeftCorner);
		layout->addCornerAnchors(nothing, Qt::BottomRightCorner,
					 layout, Qt::BottomRightCorner);
		
	} else {
		createTagButtons(layout);
	}
}

void MainPage::createTag(void)
{
	CreatePage *page = new CreatePage();
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(QString)),
		this, SLOT(createSelectedTag(QString)));
}

void MainPage::createSelectedTag(QString which)
{
	MApplicationPage *page = 0;

	if (which == Tag::BLUETOOTH_TAG) {
		page = new BtPage(m_bluez);
	} else if (which == Tag::TEXT_TAG) {
		page = new TextPage();
	} else if (which == Tag::URL_TAG) {
		page = new UrlPage();
	}

	if (page != 0) {
		page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
		connect(page, SIGNAL(disappeared(void)),
			this, SLOT(refreshList(void)));
	}
}

void MainPage::editTag(void)
{
	if (m_longTapIndex.isValid() &&
	    m_longTapIndex.row() < TagStorage::count()) {
		MApplicationPage *page = 0;
		const Tag *tag = TagStorage::tag(m_longTapIndex.row());
		if (tag->type() == Tag::BLUETOOTH_TAG) {
			page = new BtPage(m_bluez, m_longTapIndex.row());
		} else if (tag->type() == Tag::TEXT_TAG) {
			page = new TextPage(m_longTapIndex.row());
		} else if (tag->type() == Tag::URL_TAG) {
			page = new UrlPage(m_longTapIndex.row());
		}
		if (page != 0) {
			page->appear(scene(), 
				     MSceneWindow::DestroyWhenDismissed);
			connect(page, SIGNAL(disappeared(void)),
				this, SLOT(refreshList(void)));
		}
		m_longTapIndex = QModelIndex();
	}
}

void MainPage::removeTag(void)
{
	if (m_longTapIndex.isValid() &&
	    m_longTapIndex.row() < TagStorage::count()) {
		TagStorage::remove(m_longTapIndex.row());
		refreshList();
		m_longTapIndex = QModelIndex();
	}
}

void MainPage::showAbout(void)
{
}

void MainPage::tagSelected(const QModelIndex &which)
{
	(void) which;
	/* TODO: write it */
}

void MainPage::tagLongSelected(const QModelIndex &which,
			       const QPointF &position)
{
	if (which.isValid() &&
	    which.row() < TagStorage::count()) {
		m_longTapIndex = which;
		m_objectMenu->setCursorPosition(position);
		m_objectMenu->setTitle(which.data().toString());
		sceneManager()->appearSceneWindow(m_objectMenu);
	}
}
