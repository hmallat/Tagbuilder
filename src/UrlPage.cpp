/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "UrlPage.h"
#include "TextRecordEdit.h"
#include "LabeledTextEdit.h"

#include <QUrl>
#include <QGraphicsLinearLayout>
#include <QNdefNfcUriRecord>

UrlPage::UrlPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_url(0),
	  m_title(0)
{
}

UrlPage::~UrlPage(void)
{
}

void UrlPage::createPageSpecificContent(void)
{
	m_url = new LabeledTextEdit(MTextEditModel::SingleLine,
				    tr("URL"),
				    tr("Enter URL"));
	m_url->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout()->addItem(m_url);
	layout()->setAlignment(m_url, Qt::AlignCenter);

	connect(m_url, SIGNAL(textChanged()),
		this, SLOT(urlChanged(void)));
}

void UrlPage::setupNewData(void)
{
}

bool UrlPage::setupData(const QNdefMessage message)
{
	QNdefNfcUriRecord U(message[0]);
	m_url->setText(U.uri().toString());
	return true;
}

QNdefMessage UrlPage::prepareDataForStorage(void)
{
	QNdefMessage message;
	QNdefNfcUriRecord U;
	U.setUri(m_url->text());
	message << U;
	return message;
}

void UrlPage::urlChanged(void)
{
	/* TODO: how about checking also that the URL is a valid one */
	setContentValidity(m_url->text() != "" ? true : false);
}

