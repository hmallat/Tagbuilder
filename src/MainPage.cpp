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

#include <QGraphicsLinearLayout>
#include <MAction>
#include <MLabel>
#include <MButton>
#include <MButtonGroup>

static const QString _messageTypeToIcon(const QNdefMessage &message)
{
	QNdefRecord record = message.at(0);
	if (record.typeNameFormat() == QNdefRecord::NfcRtd) {
		if (record.type() == "T") {
			return "icon-m-content-text";
		} else if (record.type() == "U" ||
			   record.type() == "Sp") {
			return "icon-m-content-uri";
		} else {
			return "";
		}
	} else {
		return "";
	}
}

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
	const QList<Tag*> storedTags = TagStorage::storedTags();

	delete m_tagButtons;
	m_tagButtons = new MButtonGroup(this);

	for (int i = 0; i < storedTags.length(); i++) {
		Tag *tag = storedTags.at(i);
		QString iconId = _messageTypeToIcon(tag->message());
		MButton *button = new MButton(iconId, tag->name());
		m_tagButtons->addButton(button, i);
		layout->addItem(button);
	}

	connect(m_tagButtons, SIGNAL(buttonClicked(int)),
		this, SLOT(tagSelected(int)));
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
