/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CreatePage.h"
#include "CreateTextPage.h"

#include <QGraphicsLinearLayout>
#include <MButton>
#include <MLabel>

CreatePage::CreatePage(QGraphicsItem *parent)
	: MApplicationPage(parent)
{
}

CreatePage::~CreatePage(void)
{
}

void CreatePage::createContent(void)
{
	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget()->setLayout(layout);

	MLabel *label = new MLabel(tr("<big>Select the type of tag "
				      "to create</big>"));
	label->setAlignment(Qt::AlignCenter);
	layout->addItem(label);
	layout->setAlignment(label, Qt::AlignHCenter);

	MButton *btButton = new MButton("icon-m-content-bluetooth", 
					tr("Bluetooth device"));
	layout->addItem(btButton);
	layout->setAlignment(btButton, Qt::AlignHCenter);

	MButton *urlButton = new MButton("icon-m-content-url", 
					 tr("Bookmark"));
	layout->addItem(urlButton);
	layout->setAlignment(urlButton, Qt::AlignHCenter);

	MButton *calButton = new MButton("icon-m-content-calendar", 
					 tr("Calendar entry"));
	layout->addItem(calButton);
	layout->setAlignment(calButton, Qt::AlignHCenter);

	MButton *conButton = new MButton("icon-m-content-avatar-placeholder", 
					 tr("Contact"));
	layout->addItem(conButton);
	layout->setAlignment(conButton, Qt::AlignHCenter);

	MButton *txtButton = new MButton("icon-m-content-text", 
					 tr("Text"));
	layout->addItem(txtButton);
	layout->setAlignment(txtButton, Qt::AlignHCenter);
	connect(txtButton, SIGNAL(clicked(bool)),
		this, SLOT(createText(void)));

	MButton *wlanButton = new MButton("icon-m-content-wlan", 
					  tr("Wireless network"));
	layout->addItem(wlanButton);
	layout->setAlignment(wlanButton, Qt::AlignHCenter);

	MLabel *bottomSpacer = new MLabel("");
	bottomSpacer->setAlignment(Qt::AlignCenter);
	layout->addItem(bottomSpacer);
	layout->setAlignment(bottomSpacer, Qt::AlignHCenter);
}

void CreatePage::createText(void)
{
	CreateTextPage *page = new CreateTextPage();
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
}

