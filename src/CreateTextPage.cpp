/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CreateTextPage.h"
#include "TextRecordEdit.h"
#include "Tag.h"
#include "TagStorage.h"

#include <QGraphicsLinearLayout>
#include <MAction>
#include <MTextEdit>
#include <MLabel>

#include <QNdefNfcTextRecord>
#include <QNdefMessage>

QTM_USE_NAMESPACE;

/* TODO: show a locale specific human readable language name 
   instead of two-letter code (study QLocale) */

/* TODO: subclass text edit to show placeholder text */

/* TODO: dim placeholder text */

CreateTextPage::CreateTextPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_name(NULL),
	  m_nameFocused(false),
	  m_edit(NULL),
	  m_sysinfo(new QSystemInfo(this)),
	  m_saveAction(NULL)
{
}

CreateTextPage::~CreateTextPage(void)
{
}

void CreateTextPage::createContent(void)
{
	m_saveAction = new MAction(tr("Save"), this);
	m_saveAction->setEnabled(false);
	m_saveAction->setLocation(MAction::ToolBarLocation);
 	connect(m_saveAction, SIGNAL(triggered()),
 		this, SLOT(saveTag()));
	addAction(m_saveAction);

	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical);

	MLabel *label = new MLabel(tr("<big>Create tag contents</big>"));
	label->setAlignment(Qt::AlignCenter);
	layout->addItem(label);
	layout->setAlignment(label, Qt::AlignHCenter);

	m_name = new MTextEdit(MTextEditModel::SingleLine);
	m_name->setPrompt(tr("Enter tag name"));
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
		/* cannot save without a name */
		m_saveAction->setEnabled(false);
	} else {
		m_saveAction->setEnabled(true);
	}
}

void CreateTextPage::saveTag(void)
{
	QNdefMessage message;

	QNdefNfcTextRecord T;
	T.setEncoding(QNdefNfcTextRecord::Utf8);
	T.setLocale(m_edit->language());
	T.setText(m_edit->contents());
	message << T;
	
	Tag *tag = new Tag(m_name->text(), message);
	if (TagStorage::append(tag) == false) {
		/* TODO: show banner */
		delete tag;
	}

	/* TODO: go back to main? */
}
