/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TextPage.h"
#include "LabeledTextEdit.h"
#include "TextRecordEdit.h"
#include "Tag.h"
#include "TagStorage.h"

#include <QGraphicsLinearLayout>
#include <MAction>
#include <MLabel>
#include <MMessageBox>

#include <QNdefNfcTextRecord>
#include <QNdefMessage>

QTM_USE_NAMESPACE;

/* TODO: show a locale specific human readable language name 
   instead of two-letter code (study QLocale) */

TextPage::TextPage(int tag, QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_tag(tag),
	  m_name(NULL),
	  m_nameFocused(false),
	  m_edit(NULL),
	  m_sysinfo(new QSystemInfo(this)),
	  m_storeAction(NULL)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

TextPage::~TextPage(void)
{
}

void TextPage::createContent(void)
{
	m_cancelAction = new MAction(tr("Cancel"), this);
	m_cancelAction->setLocation(MAction::ToolBarLocation);
	connect(m_cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(m_cancelAction);

	m_storeAction = new MAction(tr("Store"), this);
	m_storeAction->setEnabled(false);
	m_storeAction->setLocation(MAction::ToolBarLocation);
	connect(m_storeAction, SIGNAL(triggered()),
		this, SLOT(storeTag()));
	addAction(m_storeAction);

	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical);

	MLabel *label = new MLabel(m_tag == -1 
				   ? tr("<big>Create tag contents</big>")
				   : tr("<big>Edit tag contents</big>"));
	label->setAlignment(Qt::AlignCenter);
	layout->addItem(label);
	layout->setAlignment(label, Qt::AlignHCenter);

	m_name = new LabeledTextEdit(MTextEditModel::SingleLine,
				     tr("Tag name"),
				     tr("Enter tag name"));
	if (m_tag != -1) {
		m_name->setText(TagStorage::tag(m_tag)->name());
	}
	layout->addItem(m_name);
	layout->setAlignment(m_name, Qt::AlignHCenter);
	connect(m_name, SIGNAL(textChanged(void)),
		this, SLOT(nameChanged(void)));

	if (m_tag != -1) {
		QNdefNfcTextRecord T(TagStorage::tag(m_tag)->message().at(0));
		m_edit = new TextRecordEdit(m_sysinfo->availableLanguages(),
					    T.locale());
		m_edit->setContents(T.text());
	} else {
		m_edit = new TextRecordEdit(m_sysinfo->availableLanguages(),
					    m_sysinfo->currentLanguage());
	}
	layout->addItem(m_edit);
	layout->setAlignment(m_edit, Qt::AlignHCenter);
	layout->setStretchFactor(m_edit, 999);

	centralWidget()->setLayout(layout);
}

void TextPage::nameChanged(void)
{
	if (m_name->text() == "") {
		/* cannot store without a name */
		m_storeAction->setEnabled(false);
	} else {
		m_storeAction->setEnabled(true);
	}
}

void TextPage::storeTag(void)
{
	QNdefMessage message;

	QNdefNfcTextRecord T;
	T.setEncoding(QNdefNfcTextRecord::Utf8);
	T.setLocale(m_edit->language());
	T.setText(m_edit->contents());
	message << T;
	
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
