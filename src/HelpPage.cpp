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

#include "HelpPage.h"
#include "ViewHeader.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MAction>
#include <MImageWidget>
#include <MPannableViewport>

#include <contentaction.h>

#include <MDebug>

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
		tr("<h2>Application home page</h2>"

		   "Please see <a href=\"http://hannu.mallat.fi/n9/nfctagbuilder\">http://hannu.mallat.fi/n9/nfctagbuilder</a>"

		   "<h2>Creating tag contents</h2>"
		   
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
	
	connect(info, SIGNAL(linkActivated(const QString &)),
		this, SLOT(linkActivated(const QString &)));

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

void HelpPage::linkActivated(const QString &link)
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

