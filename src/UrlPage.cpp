/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "UrlPage.h"
#include "LabeledTextEdit.h"
#include "TextRecordEdit.h"
#include "TagStorage.h"
#include "Tag.h"

#include <MAction>
#include <MLabel>
#include <MMessageBox>
#include <QGraphicsAnchorLayout>
#include <QUrl>
#include <QNdefNfcUriRecord>

UrlPage::UrlPage(int tag, QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_tag(tag),
	  m_name(NULL),
	  m_url(NULL),
	  m_titleEdits(),
	  m_cancelAction(NULL),
	  m_storeAction(NULL)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

UrlPage::~UrlPage(void)
{
}

void UrlPage::createContent(void)
{
	m_cancelAction = new MAction(tr("Cancel"), this);
	m_cancelAction->setLocation(MAction::ToolBarLocation);
	connect(m_cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(m_cancelAction);

	m_storeAction = new MAction(tr("Store"), this);
	if (m_tag == -1) {
		m_storeAction->setEnabled(false);
	}
	m_storeAction->setLocation(MAction::ToolBarLocation);
	connect(m_storeAction, SIGNAL(triggered()),
		this, SLOT(storeTag()));
	addAction(m_storeAction);

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();

	MLabel *label = new MLabel(m_tag == -1 
				   ? tr("<big>Create tag contents</big>")
				   : tr("<big>Edit tag contents</big>"));
	label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	label->setAlignment(Qt::AlignCenter);
	layout->addCornerAnchors(label, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);
	layout->addAnchor(label, Qt::AnchorRight,
			  layout, Qt::AnchorRight);

	m_name = new LabeledTextEdit(MTextEditModel::SingleLine,
				     tr("Tag name"),
				     tr("Enter tag name"));
	if (m_tag != -1) {
		m_name->setText(TagStorage::tag(m_tag)->name());
	}
	m_name->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout->addCornerAnchors(m_name, Qt::TopLeftCorner,
				 label, Qt::BottomLeftCorner);
	layout->addAnchor(m_name, Qt::AnchorRight,
			  layout, Qt::AnchorRight);
	connect(m_name, SIGNAL(textChanged(void)),
		this, SLOT(nameChanged(void)));

	m_url = new LabeledTextEdit(MTextEditModel::SingleLine,
				    tr("URL"),
				    tr("Enter URL"));
	if (m_tag != -1) {
		QNdefNfcUriRecord U(TagStorage::tag(m_tag)->message().at(0));
		m_url->setText(U.uri().toString());
	}
	m_url->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout->addCornerAnchors(m_url, Qt::TopLeftCorner,
				 m_name, Qt::BottomLeftCorner);
	layout->addAnchor(m_url, Qt::AnchorRight,
			  layout, Qt::AnchorRight);

	centralWidget()->setLayout(layout);
}

void UrlPage::nameChanged(void)
{
	if (m_name->text() == "") {
		/* cannot store without a name */
		m_storeAction->setEnabled(false);
	} else {
		m_storeAction->setEnabled(true);
	}
}

void UrlPage::storeTag(void)
{
	QNdefMessage message;

	QNdefNfcUriRecord U;
	U.setUri(m_url->text());
	message << U;
	
	bool success;
	if (m_tag == -1) {
		success = TagStorage::append(m_name->text(), message);
	} else {
		success = TagStorage::update(m_tag,
					     m_name->text(),
					     message);
	}
	
	if (success == false) {
		MMessageBox *box = 
			new MMessageBox(tr("Cannot store the tag. "));
		box->appear();
	} else {
		dismiss();
	}

}
