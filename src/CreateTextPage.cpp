/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CreateTextPage.h"
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

CreateTextPage::CreateTextPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_name(NULL),
	  m_nameFocused(false),
	  m_edit(NULL),
	  m_sysinfo(new QSystemInfo(this)),
	  m_storeAction(NULL)
{
}

CreateTextPage::~CreateTextPage(void)
{
}

void CreateTextPage::createContent(void)
{
	m_storeAction = new MAction(tr("Store"), this);
	m_storeAction->setEnabled(false);
	m_storeAction->setLocation(MAction::ToolBarLocation);
 	connect(m_storeAction, SIGNAL(triggered()),
 		this, SLOT(storeTag()));
	addAction(m_storeAction);

	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical);

	MLabel *label = new MLabel(tr("<big>Create tag contents</big>"));
	label->setAlignment(Qt::AlignCenter);
	layout->addItem(label);
	layout->setAlignment(label, Qt::AlignHCenter);

	m_name = new LabeledTextEdit(MTextEditModel::SingleLine,
				     tr("Tag name"),
				     tr("Enter tag name"));
	layout->addItem(m_name);
	layout->setAlignment(m_name, Qt::AlignHCenter);
	connect(m_name, SIGNAL(textChanged(void)),
		this, SLOT(nameChanged(void)));

	m_edit = new TextRecordEdit(m_sysinfo->availableLanguages(),
				    m_sysinfo->currentLanguage());
	layout->addItem(m_edit);
	layout->setAlignment(m_edit, Qt::AlignHCenter);
	layout->setStretchFactor(m_edit, 999);

	centralWidget()->setLayout(layout);
}

void CreateTextPage::nameChanged(void)
{
	if (m_name->text() == "") {
		/* cannot store without a name */
		m_storeAction->setEnabled(false);
	} else {
		m_storeAction->setEnabled(true);
	}
}

void CreateTextPage::storeTag(void)
{
	QNdefMessage message;

	QNdefNfcTextRecord T;
	T.setEncoding(QNdefNfcTextRecord::Utf8);
	T.setLocale(m_edit->language());
	T.setText(m_edit->contents());
	message << T;
	
	Tag *tag = new Tag(m_name->text(), message);
	if (TagStorage::append(tag) == false) {
		MMessageBox *box = 
			new MMessageBox(tr("Cannot store the tag. "));
		box->appear();
		delete tag;
	}

	/* TODO: go back to main? */
}
