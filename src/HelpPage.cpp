/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "HelpPage.h"
#include "ViewHeader.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MAction>
#include <MImageWidget>
#include <MPannableViewport>

HelpPage::HelpPage(QGraphicsItem *parent)
	: MApplicationPage(parent)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

HelpPage::~HelpPage(void)
{
}

void HelpPage::createActions(void)
{
	MAction *done = new MAction(tr("Done"), this);
	done->setLocation(MAction::ToolBarLocation);
	connect(done, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(done);
}

void HelpPage::createContent(void)
{
	MApplicationPage::createContent();
	setStyleName("CommonApplicationPage");
	setPannable(false);

	createActions();

	QGraphicsAnchorLayout *anchor = new QGraphicsAnchorLayout();
	anchor->setContentsMargins(0, 0, 0, 0);
	anchor->setSizePolicy(QSizePolicy::Preferred, 
			      QSizePolicy::Preferred);

	ViewHeader *header = 
		new ViewHeader(tr("Instructions"));
	anchor->addCornerAnchors(header, Qt::TopLeftCorner,
				 anchor, Qt::TopLeftCorner);

	MWidget *layoutContainer = new MWidget();
	layoutContainer->setSizePolicy(QSizePolicy::Preferred, 
				       QSizePolicy::Minimum);
	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical, layoutContainer);
	layout->setSizePolicy(QSizePolicy::Preferred, 
				QSizePolicy::Minimum);

	MLabel *info = new MLabel();
	info->setWordWrap(true);
	info->setAlignment(Qt::AlignLeft);
	info->setSizePolicy(QSizePolicy::Preferred, 
			    QSizePolicy::Preferred);
	info->setText(
		tr("<h2>Creating tag contents</h2>"
		   
		   "<p>The main page shows a list of tag content you have, "
		   "and it is initially empty. </p>"
		   
		   "<p>Start by pressing the '<b>+</b>' icon on the "
		   "bottom left corner of the tag content list, and "
		   "select what kind of tag you want to create. The "
		   "following types are available: <br>"
		   "<ul>"

		   "<li><b>Bluetooth</b> tags can be used to connect "
		   "to and share content with Bluetooth devices that "
		   "do not have built-in NFC functionality. "

		   "<li><b>Bookmark</b> tags can be used to share "
		   "web links, email addresses, phone numbers or "
		   "any other type of URL. "

		   "<li><b>Calendar</b> tags can contain calendar "
		   "events or to-do items that can be read into "
		   "other peoples' calendars. "

		   "<li><b>Contact</b> tags can be used to share "
		   "your phone number, email address, and other "
		   "contact information. "

		   "<li><b>Text</b> tags contain free-form text. "

		   "</ul></p>"

		   "<p>Each tag type has its own edit page for "
		   "filling in the details by hand, or picking "
		   "the information from other applications on "
		   "the phone. For instance, you can type in the "
		   "Bluetooth details by hand, or select "
		   "a known device's information. Check the menu "
		   "of each page for available options. </p>"

		   "<p>Once you finish editing and store the tag contents, "
		   "an entry will appear on the tag content list. </p>"

		   "<h2>Writing tag contents</h2>"

		   "<p>Tap on an entry on the list; you can then write "
		   "the selected tag contents by touching an NFC tag. </p>"

		   "<p>Note that you need to have large enough NFC "
		   "tags to hold the content you're writing. Also "
		   "note that writing can take a little while, so "
		   "do not lift your phone off the tag until the "
		   "writing progress indicator disappears. </p>"

		   "<h2>Editing and removing tag contents</h2>"
		   
		   "<p>Long tap on an entry on the tag content list to "
		   "bring up the object menu. The menu contains options "
		   "to edit or remove the entry. "

		   "<h2>Harvesting existing tags</h2>"

		   "<p> When in the main screen, simply tap NFC tags to "
		   "harvest their contents into the application. Note that "
		   "unsupported tag contents can be written as-is to "
		   "other tags, but cannot be edited. </p>"
			));
	
	layout->addItem(info);
	layout->setAlignment(info, Qt::AlignCenter);

	MPannableViewport *view = new MPannableViewport();
	view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	view->setPanDirection(Qt::Vertical);
	view->setMinimumSize(100, 100);
	view->setWidget(layoutContainer);

	anchor->addCornerAnchors(view, Qt::TopLeftCorner,
				 header, Qt::BottomLeftCorner);
	anchor->addCornerAnchors(view, Qt::BottomRightCorner,
				 anchor, Qt::BottomRightCorner);

	centralWidget()->setLayout(anchor);
}
