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

#include <QGraphicsLinearLayout>
#include <MAction>
#include <MLabel>

#include <QNdefNfcTextRecord>
#include <QNdefMessage>

QTM_USE_NAMESPACE;

/* TODO: show a locale specific human readable language name 
   instead of two-letter code (study QLocale) */

TextPage::TextPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_edit(NULL),
	  m_sysinfo(new QSystemInfo(this))
{
}

TextPage::~TextPage(void)
{
}

void TextPage::createPageSpecificContent(void)
{
	m_edit = new TextRecordEdit(tr("Text"),
				    tr("Enter text"),
				    m_sysinfo->availableLanguages(),
				    m_sysinfo->currentLanguage());
	layout()->addItem(m_edit);
	layout()->setAlignment(m_edit, Qt::AlignHCenter);

	connect(m_edit, SIGNAL(contentsChanged()),
		this, SLOT(textChanged(void)));
}

void TextPage::setupNewData(void)
{
}

bool TextPage::setupData(QNdefMessage message)
{
	QNdefNfcTextRecord T(message[0]);
	m_edit->setLanguage(T.locale());
	m_edit->setContents(T.text());
	return true;
}

QNdefMessage TextPage::prepareDataForStorage(void)
{
	QNdefMessage message;
	QNdefNfcTextRecord T;
	T.setEncoding(QNdefNfcTextRecord::Utf8);
	T.setLocale(m_edit->language());
	T.setText(m_edit->contents());
	message << T;
	return message;
}

void TextPage::textChanged(void)
{
	setContentValidity(m_edit->contents() != "" ? true : false);
}
