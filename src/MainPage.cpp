/*

    NFC Tag Builder for Nokia N9
    Copyright (C) 2011,2012  Hannu Mallat <hmallat@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
#include "UnknownPage.h"
#include "UrlPage.h"
#include "TagReader.h"
#include "LabelOrList.h"
#include "BtNdefRecord.h"
#include "VCardNdefRecord.h"
#include "NfcHandoverSelectNdefRecord.h"
#include "WritePage.h"
#include "ViewHeader.h"
#include "NfcdMonitor.h"
#include "HelpPage.h"

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MList>
#include <MContentItem>
#include <MPannableViewport>
#include <MObjectMenu>
#include <MSceneManager>
#include <MMessageBox>

#include <contentaction.h>

#include <MDebug>

#ifndef VERSION
#define VERSION "0.0.0"
#endif

static MAbstractCellCreator<MWidgetController> *_getTagListCellCreator(void)
{
	return new TagListCellCreator();
}

MainPage::MainPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_objectMenu(NULL),
	  m_unknownObjectMenu(NULL),
	  m_longTapIndex(QModelIndex())
{
	m_bluez = new BluezSupplicant(this);
	m_reader = new TagReader(this);
	m_monitor = new NfcdMonitor(this);

	connect(this, SIGNAL(appeared()), 
		this, SLOT(pageAppeared()));

	connect(this, SIGNAL(disappearing()), 
		this, SLOT(pageDisappearing()));

	connect(m_reader, SIGNAL(messageRead(const QNdefMessage)),
		this, SLOT(messageRead(const QNdefMessage)));

	m_monitor->start();
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

	m_unknownObjectMenu = new MObjectMenu(this);
	
	MAction *showAction = new MAction(tr("Show tag"), this);
	m_unknownObjectMenu->addAction(showAction);
  	connect(showAction, SIGNAL(triggered()), this, SLOT(editTag()));

	MAction *removeUnknownAction = new MAction(tr("Remove tag"), this);
	m_unknownObjectMenu->addAction(removeUnknownAction);
  	connect(removeUnknownAction, SIGNAL(triggered()), 
		this, SLOT(removeTag()));
}

void MainPage::createActions(void)
{
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

	MAction *helpAction = new MAction(tr("Instructions..."), this);
	helpAction->setLocation(MAction::ApplicationMenuLocation);
 	connect(helpAction, SIGNAL(triggered()),
 		this, SLOT(showHelp()));
	addAction(helpAction);
}

void MainPage::createContent(void)
{
	m_bluez->start();

	MApplicationPage::createContent();
	setStyleName("CommonApplicationPage");
	setPannable(false);
	
	createObjectMenuActions();
	createActions();

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
	layout->setContentsMargins(0, 0, 0, 0);

	ViewHeader *header = new ViewHeader(tr("Stored tags"));
	layout->addCornerAnchors(header, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);

	LabelOrList *list = new LabelOrList(TagStorage::storage(),
					    _getTagListCellCreator,
					    tr("You don't have any "
					       "stored tags currently. "
					       "Create some or harvest "
					       "existing ones by touching "
					       "them."),
					    false,
					    false);
	list->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	layout->addCornerAnchors(list, Qt::TopLeftCorner,
				 header, Qt::BottomLeftCorner);
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
		editTag(m_longTapIndex.row());
		m_longTapIndex = QModelIndex();
	}
}

void MainPage::editTag(int index)
{
	MApplicationPage *page = 0;

	mDebug(__func__) << "Getting tag " << index;
	const Tag *tag = TagStorage::storage()->tag(index);
	mDebug(__func__) << "Got tag " << tag;

	if (tag->type() == Tag::BLUETOOTH_TAG) {
		page = new BtPage(m_bluez, index);
	} else if (tag->type() == Tag::CALENDAR_TAG) {
		page = new CalendarPage(index);
	} else if (tag->type() == Tag::CONTACT_TAG) {
		page = new ContactPage(index);
	} else if (tag->type() == Tag::TEXT_TAG) {
		page = new TextPage(index);
	} else if (tag->type() == Tag::URL_TAG) {
		page = new UrlPage(index);
	} else {
		page = new UnknownPage(index);
	}
	
	if (page != 0) {
		page->appear(scene(), 
			     MSceneWindow::DestroyWhenDismissed);
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
	MLabel *label = 
		new MLabel(tr("<big>NFC Tag Builder</big><br>"
			      "<br>"
			      "<br>v%1"
			      "<br>"
			      "Copyright (c) 2012 Hannu Mallat<br>"
			      "<a href=\"http://hannu.mallat.fi/n9/nfctagbuilder\">http://hannu.mallat.fi/n9/nfctagbuilder</a><br>")
			   .arg(VERSION));
	label->setWordWrap(true);
        label->setAlignment(Qt::AlignHCenter);
        label->setStyleName("CommonQueryText");
	connect(label, SIGNAL(linkActivated(const QString &)),
		this, SLOT(linkActivated(const QString &)));

	MMessageBox *box = new MMessageBox();
	box->setCentralWidget(label);

	box->appear();
}

void MainPage::showHelp(void)
{
	HelpPage *page = new HelpPage();
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}

void MainPage::linkActivated(const QString &link)
{
        mDebug(__func__) << "link=" << link;
        ContentAction::Action uriAction = 
                ContentAction::Action::defaultActionForScheme(link);
        if (uriAction.isValid()) {
                mDebug(__func__) << "Triggering...";
                uriAction.trigger();
        } else {
                mDebug(__func__) << "Invalid action. ";
        }
}

void MainPage::tagSelected(const QModelIndex &which)
{
	const Tag *tag = TagStorage::storage()->tag(which.row());
	WritePage *page = new WritePage(tag->message(), m_monitor);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}

void MainPage::tagLongSelected(const QModelIndex &which,
			       const QPointF &position)
{
	if (which.isValid() &&
	    which.row() < TagStorage::storage()->count()) {
		m_longTapIndex = which;
		const Tag *tag = TagStorage::storage()->tag(which.row());
		if (tag->type() == Tag::UNKNOWN_TAG) {
			m_unknownObjectMenu->setCursorPosition(position);
			m_unknownObjectMenu->setTitle(which.data().toString());
			sceneManager()->appearSceneWindow(m_unknownObjectMenu);
		} else {
			m_objectMenu->setCursorPosition(position);
			m_objectMenu->setTitle(which.data().toString());
			sceneManager()->appearSceneWindow(m_objectMenu);
		}
	}
}

void MainPage::pageAppeared(void)
{
	mDebug(__func__) << "Enabling NFC read. ";
	m_reader->start();
}

void MainPage::pageDisappearing(void)
{
	mDebug(__func__) << "Disabling NFC read. ";
	m_reader->stop();
}

void MainPage::messageRead(const QNdefMessage contents)
{
	bool success;
	QNdefMessage in;
	QString name;

	if (contents.length() == 0) {
		success = false;
		goto done;
	}

	/* TODO: UI notification if conversion drops data? */

	/* Convert incoming data so we don't have to deal with this
	   mess later on -- Qt API could have had some consideration
	   for this */
	if (VCardNdefRecord::hasSupportedMimeType(contents[0]) &&
	    contents.length() == 1) {
		in << VCardNdefRecord::fromSupportedMimeType(contents[0]);
	} else if (BtNdefRecord::hasSupportedMimeType(contents[0]) &&
		   contents.length() == 1) {
		in << BtNdefRecord::fromSupportedMimeType(contents[0]);
	} else if (contents[0].isRecordType<NfcHandoverSelectNdefRecord>() &&
		   contents.length() == 2 &&
		   BtNdefRecord::hasSupportedMimeType(contents[1])) {
		in << contents[0];
		in << BtNdefRecord::fromSupportedMimeType(contents[1]);
	} else {
		in = contents;
	}

	mDebug(__func__) << "Saving temp";
	name = "Harvested tag";
	success = TagStorage::storage()->update(TagStorage::TEMPORARY_TAG,
						name, 
						in);
done:
	if (success == false) {
		MMessageBox *box = 
			new MMessageBox(tr("Cannot store the tag. "));
		box->appear(MSceneWindow::DestroyWhenDismissed);
	} else {
		mDebug(__func__) << "Editing temp";
		editTag(TagStorage::TEMPORARY_TAG);
	}
}
