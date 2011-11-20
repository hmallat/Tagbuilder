/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CreateTextPage.h"

#include <QGraphicsLinearLayout>
#include <MTextEdit>
#include <MLabel>
#include <MComboBox>
#include <MAction>

#include <MDebug>

/* TODO: show a locale specific human readable language name 
   instead of two-letter code (study QLocale) */

CreateTextPage::CreateTextPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_text(NULL),
	  m_sizeLabel(NULL),
	  m_langCombo(NULL),
	  m_sysinfo(new QSystemInfo(this))
{
}

CreateTextPage::~CreateTextPage(void)
{
}

void CreateTextPage::createContent(void)
{
	MAction *saveAction = new MAction(tr("Save the tag"),
					  this);
	saveAction->setLocation(MAction::ToolBarLocation);
 	connect(saveAction, SIGNAL(triggered()),
 		this, SLOT(saveTag()));
	addAction(saveAction);

	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical);

	MLabel *label = new MLabel(tr("Fill in text"));
	label->setAlignment(Qt::AlignCenter);
	layout->addItem(label);
	layout->setAlignment(label, Qt::AlignHCenter);

	m_text = new MTextEdit(MTextEditModel::MultiLine);
	layout->addItem(m_text);
	layout->setAlignment(m_text, Qt::AlignHCenter);
	layout->setStretchFactor(m_text, 999);

	connect(m_text, SIGNAL(textChanged(void)),
		this, SLOT(textChanged(void)));

	{
		QGraphicsLinearLayout *sublayout = 
			new QGraphicsLinearLayout(Qt::Horizontal);

		m_langCombo = new MComboBox();
		m_langCombo->setTitle(tr("Text language"));
		m_langCombo->addItems(m_sysinfo->availableLanguages());
		for (int i = 0; i < m_langCombo->count(); i++) {
			if (m_sysinfo->currentLanguage() == 
			    m_langCombo->itemText(i)) {
				m_langCombo->setCurrentIndex(i);
				break;
			}
		}
		if (m_langCombo->currentIndex() == -1) {
			m_langCombo->setCurrentIndex(0);
		}
		sublayout->addItem(m_langCombo);
		sublayout->setAlignment(m_langCombo, 
					Qt::AlignLeft | Qt::AlignVCenter);
		connect(m_langCombo, 
			SIGNAL(currentIndexChanged(int)),
			this,
			SLOT(languageChanged(void)));

		m_sizeLabel = new MLabel();
		m_sizeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		sublayout->addItem(m_sizeLabel);
		sublayout->setAlignment(m_sizeLabel, 
					Qt::AlignRight | Qt::AlignVCenter);
		sublayout->setStretchFactor(m_sizeLabel, 999);

		layout->addItem(sublayout);
	}

	centralWidget()->setLayout(layout);
	updateSize();
}

void CreateTextPage::updateSize(void)
{
	/* This is an awful lot of work just to print a number... */

	QString lang = m_langCombo->currentText();

	int payloadLength = 
		1 + /* status byte */
		lang.toUtf8().length() + /* language code */
		m_text->text().toUtf8().length(); /* text itself */

	int ndefLength = 
		1 + /* NDEF header */
		1 + /* type length */
		(payloadLength < 256 ? 1 : 4) + /* payload length */
		1 + /* type (T) */
		payloadLength; /* payload */

	m_sizeLabel->setText(tr("%1 bytes").arg(ndefLength));
}

void CreateTextPage::textChanged(void)
{
	updateSize();
}

void CreateTextPage::languageChanged(void)
{
	updateSize();
}
