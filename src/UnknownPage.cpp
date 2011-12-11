/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "UnknownPage.h"
#include "Util.h"

#include <MLabel>
#include <MButton>
#include <MMessageBox>
#include <MContentItem>
#include <QGraphicsLinearLayout>

#include <MDebug>

UnknownPage::UnknownPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent)
{
}

UnknownPage::~UnknownPage(void)
{
}

void UnknownPage::createPageSpecificContent(void)
{
	MLabel *title = new MLabel(tr("<b>Unsupported tag</b><br><br>"
				      "The tag contents cannot be edited. "
				      "However you can still write the tag "
				      "from the main screen."));
	title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	title->setAlignment(Qt::AlignCenter);
	title->setWordWrap(true);
	layout()->addItem(title);
	layout()->setAlignment(title, Qt::AlignCenter);

	{
		QGraphicsLinearLayout *sub_layout = 
			new QGraphicsLinearLayout(Qt::Vertical);

		MButton *show_button = 
			new MButton(tr("Show tag contents"));
		show_button->setSizePolicy(QSizePolicy::Minimum, 
					   QSizePolicy::Fixed);
		sub_layout->addItem(show_button);
		connect(show_button, SIGNAL(clicked()),
			this, SLOT(showContents()));

		layout()->addItem(sub_layout);
		layout()->setAlignment(sub_layout, Qt::AlignCenter);
	}
	
}

void UnknownPage::setupNewData(void) 
{
	m_message = QNdefMessage();
	updateSize();
}

bool UnknownPage::setupData(const QNdefMessage message)
{
	m_message = message;
	updateSize();
	return true;
}

QNdefMessage UnknownPage::prepareDataForStorage(void)
{
	return m_message;
}

void UnknownPage::updateSize(void)
{
	setContentSize(Util::messageLength(m_message));
}

void UnknownPage::showContents(void)
{
	QString contents;

	contents.append("<b>Tag contents</b><br>");
	contents.append("<br>");
	contents.append("If you want this tag to be editable, "
			"please contact support with the "
			"information given below. <br>");
	contents.append("<br>");
	contents.append(QString("%1 %2<br>")
			.arg(m_message.length())
			.arg(m_message.length() == 1 ? "record" : "records"));
	for (int i = 0; i < m_message.length(); i++) {

		QString rtype;
		rtype.append(QString("Record %1 type: ").arg(i));

		QNdefRecord::TypeNameFormat tnf = 
			m_message[i].typeNameFormat();
		if (tnf == QNdefRecord::Empty) {
			rtype.append("empty");
		} else if (tnf == QNdefRecord::NfcRtd) {
			rtype.append("urn:nfc:wkt:");
			rtype.append(QString::fromAscii(m_message[i].type()));
		} else if (tnf == QNdefRecord::Mime) {
			rtype.append("mime:");
			rtype.append(QString::fromAscii(m_message[i].type()));
		} else if (tnf == QNdefRecord::Uri) {
			rtype.append("uri:");
			rtype.append(QString::fromAscii(m_message[i].type()));
		} else if (tnf == QNdefRecord::ExternalRtd) {
			rtype.append("urn:nfc:ext:");
			rtype.append(QString::fromAscii(m_message[i].type()));
		} else if (tnf == QNdefRecord::Unknown) {
			rtype.append("unknown");
		} else {
			rtype.append("unspecified");
		}

		contents.append(rtype);
		contents.append("<br>");
	}

	MMessageBox *box = new MMessageBox(contents);
	box->appear();
}
