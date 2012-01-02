/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "WritePage.h"
#include "TagWriter.h"

#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <MAction>
#include <MLabel>
#include <MButton>
#include <MImageWidget>
#include <MProgressIndicator>
#include <MMessageBox>

#define TAP_IMAGE INSTALLPREFIX "/share/images/tap.png"

WritePage::WritePage(QNdefMessage message, 
		     QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_writer(NULL),
	  m_indicator(NULL)
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
}

WritePage::~WritePage(void)
{
}

void WritePage::createContent(void)
{
	MApplicationPage::createContent();

	setPannable(false);

	m_done = new MAction(tr("Done"), this);
	m_done->setLocation(MAction::ToolBarLocation);
	connect(m_done, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(m_done);

	QGraphicsAnchorLayout *anchor = new QGraphicsAnchorLayout();
	anchor->setSizePolicy(QSizePolicy::Preferred, 
			      QSizePolicy::Preferred);

	MWidgetController *header = new MWidgetController();
	header->setStyleName("CommonHeaderPanel");
	header->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	anchor->addCornerAnchors(header, Qt::TopLeftCorner,
				 anchor, Qt::TopLeftCorner);

	MLabel *label = new MLabel(tr("Write tags"),
				   header);
	label->setStyleName("CommonHeader");
	label->setAlignment(Qt::AlignLeft);

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
	
		MLabel *info = new MLabel();
		QString first = tr("<p>Touch a tag to write it. "
				   "You can write multiple tags, "
				   "one after another.</p>");
		QString second = tr("<p>Note that tag capacity must be "
				    "at least %1 bytes.<p>")
			.arg(m_datalen);

		info->setText(m_datalen > 48 ? first + second : first);
		info->setWordWrap(true);
		info->setAlignment(Qt::AlignHCenter);
		info->setSizePolicy(QSizePolicy::Preferred, 
				    QSizePolicy::Preferred);
		sub_layout->addItem(info);
		sub_layout->setAlignment(info, Qt::AlignHCenter);

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

	m_writer->writeMessage();
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
		/* Nice, now write more or dismiss by "done" action */
		m_writer->writeMessage();
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
