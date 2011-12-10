/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BluezSupplicant.h"
#include "BtPage.h"
#include "CalendarPage.h"
#include "ContactPage.h"
#include "CreatePage.h"
#include "MainPage.h"
#include "TextPage.h"
#include "TagStorage.h"
#include "Tag.h"
#include "TagTypeListModel.h"
#include "TagListCellCreator.h"
#include "UrlPage.h"
#include "TagReader.h"
#include "LabelOrList.h"
#include "VCardNdefRecord.h"

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MList>
#include <MContentItem>
#include <MPannableViewport>
#include <MObjectMenu>
#include <MSceneManager>
#include <MMessageBox>

#include <MDebug>

static MAbstractCellCreator<MContentItem> *_getTagListCellCreator(void)
{
	return new TagListCellCreator();
}

MainPage::MainPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_objectMenu(NULL),
	  m_longTapIndex(QModelIndex())
{
	m_bluez = new BluezSupplicant(this);
	m_reader = new TagReader(this);

	connect(this, SIGNAL(appeared()), 
		this, SLOT(pageAppeared()));

	connect(this, SIGNAL(disappearing()), 
		this, SLOT(pageDisappearing()));

	connect(m_reader, SIGNAL(messageRead(const QNdefMessage)),
		this, SLOT(messageRead(const QNdefMessage)));

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
	newAction->setLocation(MAction::ToolBarLocation);
 	connect(newAction, SIGNAL(triggered()),
 		this, SLOT(createTag()));
	addAction(newAction);

	MAction *killallAction = new MAction(tr("Remove all tags"),
					     this);
	killallAction->setLocation(MAction::ApplicationMenuLocation);
 	connect(killallAction, SIGNAL(triggered()),
 		this, SLOT(removeAllTags()));
	addAction(killallAction);

	MAction *aboutAction = new MAction(tr("About..."), this);
	aboutAction->setLocation(MAction::ApplicationMenuLocation);
 	connect(aboutAction, SIGNAL(triggered()),
 		this, SLOT(showAbout()));
	addAction(aboutAction);

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();

	MLabel *label = new MLabel(tr("<big>Stored tags</big>"));
	label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	label->setAlignment(Qt::AlignLeft);
	layout->addCornerAnchors(label, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);

	LabelOrList *list = new LabelOrList(TagStorage::storage(),
					    _getTagListCellCreator,
					    tr("<h1>You don't have any "
					       "stored tags currently. "
					       "Create some or harvest "
					       "existing ones by touching "
					       "them. "
					       "</h1>"));
	list->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	layout->addCornerAnchors(list, Qt::TopLeftCorner,
				 label, Qt::BottomLeftCorner);
	layout->addCornerAnchors(list, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);
	connect(list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(tagSelected(const QModelIndex &)));
	connect(list, SIGNAL(itemLongTapped(const QModelIndex &,
					    const QPointF &)),
		this, SLOT(tagLongSelected(const QModelIndex &,
					   const QPointF &)));

	centralWidget()->setLayout(layout);
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
	} else if (which == Tag::CALENDAR_TAG) {
		page = new CalendarPage();
	} else if (which == Tag::CONTACT_TAG) {
		page = new ContactPage();
	} else if (which == Tag::TEXT_TAG) {
		page = new TextPage();
	} else if (which == Tag::URL_TAG) {
		page = new UrlPage();
	}

	if (page != 0) {
		page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	}
}

void MainPage::editTag(void)
{
	if (m_longTapIndex.isValid() &&
	    m_longTapIndex.row() < TagStorage::storage()->count()) {
		MApplicationPage *page = 0;
		const Tag *tag = 
			TagStorage::storage()->tag(m_longTapIndex.row());
		if (tag->type() == Tag::BLUETOOTH_TAG) {
			page = new BtPage(m_bluez, m_longTapIndex.row());
		} else if (tag->type() == Tag::CALENDAR_TAG) {
			page = new CalendarPage(m_longTapIndex.row());
		} else if (tag->type() == Tag::CONTACT_TAG) {
			page = new ContactPage(m_longTapIndex.row());
		} else if (tag->type() == Tag::TEXT_TAG) {
			page = new TextPage(m_longTapIndex.row());
		} else if (tag->type() == Tag::URL_TAG) {
			page = new UrlPage(m_longTapIndex.row());
		}
		if (page != 0) {
			page->appear(scene(), 
				     MSceneWindow::DestroyWhenDismissed);
		}
		m_longTapIndex = QModelIndex();
	}
}

void MainPage::removeTag(void)
{
	if (m_longTapIndex.isValid() &&
	    m_longTapIndex.row() < TagStorage::storage()->count()) {
		TagStorage::storage()->remove(m_longTapIndex.row());
		m_longTapIndex = QModelIndex();
	}
}

void MainPage::removeAllTags(void)
{
	while (TagStorage::storage()->count() != 0) {
		TagStorage::storage()->remove(0);
	}
}

void MainPage::showAbout(void)
{
}

void MainPage::tagSelected(const QModelIndex &which)
{
	(void) which;
	/* TODO: write it */
	mDebug(__func__) << "Hello!";
}

void MainPage::tagLongSelected(const QModelIndex &which,
			       const QPointF &position)
{
	if (which.isValid() &&
	    which.row() < TagStorage::storage()->count()) {
		m_longTapIndex = which;
		m_objectMenu->setCursorPosition(position);
		m_objectMenu->setTitle(which.data().toString());
		sceneManager()->appearSceneWindow(m_objectMenu);
	}
}

void MainPage::pageAppeared(void)
{
	m_reader->start();
}

void MainPage::pageDisappearing(void)
{
	m_reader->stop();
}

void MainPage::messageRead(const QNdefMessage contents)
{
	bool success;
	QNdefMessage in;

	/* Convert incoming data so we don't have to deal with this
	   mess later on -- Qt API could have had some consideration
	   for this */
	if (VCardNdefRecord::hasSupportedMimeType(contents[0]) &&
	    contents.length() == 1) {
		in << VCardNdefRecord::fromSupportedMimeType(contents[0]);
	} else {
		in = contents;
	}

	QString name = "Harvested tag";
	success = TagStorage::storage()->append(name, in);
	if (success == false) {
		MMessageBox *box = 
			new MMessageBox(tr("Cannot store the tag. "));
		box->appear();
	}
}
