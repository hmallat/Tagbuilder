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
#include <QSystemInfo>
#include <MContainer>
#include <MLabel>
#include <MButton>

UrlPage::UrlPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_sysinfo(new QSystemInfo(this)),
	  m_url(0),
	  m_titleButton(0),
	  m_title(0)
{
}

UrlPage::~UrlPage(void)
{
}

void UrlPage::createPageSpecificContent(void)
{
	m_url = new LabeledTextEdit(MTextEditModel::SingleLine,
				    tr("Bookmark URL"),
				    tr("Enter bookmark URL"));
	m_url->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout()->addItem(m_url);
	layout()->setAlignment(m_url, Qt::AlignCenter);
	connect(m_url, SIGNAL(textChanged()),
		this, SLOT(urlChanged(void)));

	{
		MContainer *container = new MContainer(tr("Bookmark title"));

		QGraphicsLinearLayout *sub =
			new QGraphicsLinearLayout(Qt::Vertical,
						  container->centralWidget());
		sub->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);


		{
			QGraphicsLinearLayout *pack =
				new QGraphicsLinearLayout(Qt::Horizontal);
			pack->setSizePolicy(QSizePolicy::Minimum, 
					    QSizePolicy::Fixed);

			MLabel *label = new MLabel(tr("Include title"));
			label->setSizePolicy(QSizePolicy::Minimum, 
					     QSizePolicy::Fixed);
			pack->addItem(label);
			pack->setAlignment(label, Qt::AlignCenter);
			
			m_titleButton = new MButton();
			m_titleButton->setCheckable(true);
			m_titleButton->setViewType(MButton::switchType);
			m_titleButton->setSizePolicy(QSizePolicy::Minimum, 
						     QSizePolicy::Fixed);
			pack->addItem(m_titleButton);
			pack->setAlignment(m_titleButton, Qt::AlignCenter);
			pack->setStretchFactor(m_titleButton, 0);
			connect(m_titleButton, SIGNAL(clicked(bool)),
				this, SLOT(titleIncludedChanged(bool)));

			sub->addItem(pack);
		}

		m_title = new TextRecordEdit("",
					     tr("Enter title"),
					     m_sysinfo->availableLanguages(),
					     m_sysinfo->currentLanguage());
		sub->addItem(m_title);
		sub->setAlignment(m_title, Qt::AlignHCenter);
		connect(m_title, SIGNAL(contentsChanged()),
			this, SLOT(titleChanged(void)));
		
		layout()->addItem(container);
	}

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

void UrlPage::titleIncludedChanged(bool checked)
{
	/* TODO */
	(void) checked;
}

void UrlPage::titleChanged(void)
{
	/* TODO */
}

void UrlPage::urlChanged(void)
{
	/* TODO: how about checking also that the URL is a valid one */
	setContentValidity(m_url->text() != "" ? true : false);
}

