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

#include "WritePage.h"
#include "TagWriter.h"
#include "NfcdMonitor.h"
#include "ViewHeader.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <MAction>
#include <MLabel>
#include <MButton>
#include <MImageWidget>
#include <MProgressIndicator>
#include <MMessageBox>

#define TAP_IMAGE INSTALLPREFIX "/share/images/tap.png"

/* Mifare ultralights have 48 writable bytes, but two bytes
   are taken by NDEF TLV overhead */
#define MAX_UL_DATALEN 46

WritePage::WritePage(QNdefMessage message, 
		     NfcdMonitor *monitor,
		     QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_writer(NULL),
	  m_indicator(NULL),
	  m_monitor(monitor),
	  m_info(NULL)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);

	QByteArray data = message.toByteArray();
	m_datalen = data.length();

	m_writer = new TagWriter(this);
	
	connect(m_writer, SIGNAL(started(void)),
		this, SLOT(writeStarted(void)));

	connect(m_writer, SIGNAL(complete(bool)),
		this, SLOT(writeFinished(bool)));

	m_writer->setMessage(message);

	connect(m_monitor, SIGNAL(nfcStatusChanged()),
		this, SLOT(updateInfo()));

}

WritePage::~WritePage(void)
{
}

void WritePage::createActions(void)
{
	m_done = new MAction(tr("Done"), this);
	m_done->setLocation(MAction::ToolBarLocation);
	connect(m_done, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(m_done);

}

void WritePage::createContent(void)
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
		new ViewHeader(tr("Write tags"));
	anchor->addCornerAnchors(header, Qt::TopLeftCorner,
				 anchor, Qt::TopLeftCorner);

	{
		QGraphicsLinearLayout *sub_layout = 
			new QGraphicsLinearLayout(Qt::Vertical);
		sub_layout->setSizePolicy(QSizePolicy::Preferred, 
					  QSizePolicy::Preferred);

		MLabel *topSpacing = new MLabel("");
		sub_layout->addItem(topSpacing);
		sub_layout->setStretchFactor(topSpacing, 999);

		MImageWidget *image = new MImageWidget(new QImage(TAP_IMAGE));
		image->setSizePolicy(QSizePolicy::Fixed, 
				     QSizePolicy::Fixed);
		sub_layout->addItem(image);
		sub_layout->setAlignment(image, Qt::AlignHCenter);
	
		m_info = new MLabel();
		m_info->setWordWrap(true);
		m_info->setAlignment(Qt::AlignHCenter);
		m_info->setSizePolicy(QSizePolicy::Preferred, 
				      QSizePolicy::Preferred);
		sub_layout->addItem(m_info);
		sub_layout->setAlignment(m_info, Qt::AlignHCenter);

		m_indicator = 
			new MProgressIndicator(NULL, MProgressIndicator::barType);
		m_indicator->setUnknownDuration(true);
		m_indicator->setVisible(false);
		sub_layout->addItem(m_indicator);
		sub_layout->setAlignment(m_indicator, Qt::AlignHCenter);

		MLabel *botSpacing = new MLabel("");
		sub_layout->addItem(botSpacing);
		sub_layout->setStretchFactor(botSpacing, 999);

		anchor->addCornerAnchors(sub_layout, Qt::TopLeftCorner,
					 header, Qt::BottomLeftCorner);
		anchor->addCornerAnchors(sub_layout, Qt::BottomRightCorner,
					 anchor, Qt::BottomRightCorner);
	}

	centralWidget()->setLayout(anchor);

	updateInfo();

	m_writer->writeMessage();
}

void WritePage::updateInfo(void)
{
	if (m_info == 0) {
		return;
	}

	if (m_monitor->status() == NfcdMonitor::NfcStatusOn) {
		QString first = tr("<p>Touch a tag to write it. "
				   "You can write multiple tags, "
				   "one after another.</p>");
		QString second = tr("<p>Note that tag must have capacity for "
				    "at least %1 bytes of NDEF data.<p>")
			.arg(m_datalen);
		m_info->setText(m_datalen > MAX_UL_DATALEN 
				? first + second : first);
	} else {
		m_info->setText(tr("<p>NFC is not enabled. To write "
				   "tags, please enable NFC from the "
				   "control panel. </p>"));
	}
}

void WritePage::writeStarted(void)
{
	m_done->setEnabled(false);
	m_indicator->setVisible(true);
}

void WritePage::writeFinished(bool success)
{
	m_done->setEnabled(true);
	m_indicator->setVisible(false);

	if (success == true) {
		/* Nice, simply keep going until the user dismisses the page */
	} else {
		/* Not much error case handling -- just show a dialog */
		MMessageBox *box = 
			new MMessageBox(tr("Cannot write the tag. "
					   "The data may be too large "
					   "for the tag, or the tag "
					   "might be unwritable. "));
		box->appear(MSceneWindow::DestroyWhenDismissed);
	}
}
