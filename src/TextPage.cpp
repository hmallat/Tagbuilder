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

TextPage::TextPage(Tag *tag, QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_tag(tag),
	  m_name(NULL),
	  m_nameFocused(false),
	  m_edit(NULL),
	  m_sysinfo(new QSystemInfo(this)),
	  m_storeAction(NULL)
{
}

TextPage::~TextPage(void)
{
}

void TextPage::createContent(void)
{
	m_storeAction = new MAction(tr("Store"), this);
	m_storeAction->setEnabled(false);
	m_storeAction->setLocation(MAction::ToolBarLocation);
	connect(m_storeAction, SIGNAL(triggered()),
		this, SLOT(storeTag()));
	addAction(m_storeAction);

	if (m_tag == 0) { /* create new */
		/* TODO */
	} else { /* edit existing */
		m_removeAction = new MAction(tr("Remove"), this);
		m_removeAction->setEnabled(false);
		m_removeAction->setLocation(MAction::ToolBarLocation);
		connect(m_removeAction, SIGNAL(triggered()),
			this, SLOT(removeTag()));
		addAction(m_removeAction);
	}

	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical);

	MLabel *label = new MLabel(m_tag == 0 
				   ? tr("<big>Create tag contents</big>")
				   : tr("<big>Edit tag contents</big>"));
	label->setAlignment(Qt::AlignCenter);
	layout->addItem(label);
	layout->setAlignment(label, Qt::AlignHCenter);

	m_name = new LabeledTextEdit(MTextEditModel::SingleLine,
				     tr("Tag name"),
				     tr("Enter tag name"));
	if (m_tag != 0) {
		m_name->setText(m_tag->name());
	}
	layout->addItem(m_name);
	layout->setAlignment(m_name, Qt::AlignHCenter);
	connect(m_name, SIGNAL(textChanged(void)),
		this, SLOT(nameChanged(void)));

	if (m_tag != 0) {
		QNdefNfcTextRecord T(m_tag->message().at(0));
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
	if (m_tag == 0) {
		Tag *new_tag = new Tag(m_name->text(), message);
		success = TagStorage::append(new_tag);
		if (success == false) {
			delete new_tag;
		}
	} else {
		m_tag->setName(m_name->text());
		m_tag->setMessage(message);
		success = TagStorage::update(m_tag);
	}
	
	if (success == false) {
		MMessageBox *box = 
			new MMessageBox(tr("Cannot store the tag. "));
		box->appear();
	} else {
		/* Go back to main page, but how? */
	}
}

void TextPage::removeTag(void)
{
	/* TODO */
}