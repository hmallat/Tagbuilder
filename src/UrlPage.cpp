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
#include "SmartPosterRecord.h"
#include "Util.h"

#include <QUrl>
#include <QGraphicsLinearLayout>
#include <QNdefNfcUriRecord>
#include <QSystemInfo>
#include <MContainer>
#include <MLabel>
#include <MButton>

#include <MDebug>

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
		connect(m_title, SIGNAL(languageChanged()),
			this, SLOT(titleLanguageChanged(void)));
		
		layout()->addItem(container);
	}

}

void UrlPage::setupNewData(void)
{
	updateSize();
}

bool UrlPage::setupData(const QNdefMessage message)
{
	bool r = false;

	QNdefRecord record = message[0];

	if (record.isRecordType<QNdefNfcUriRecord>()) {
		QNdefNfcUriRecord U(record);
		m_url->setText(U.uri().toString());
		m_titleButton->setChecked(false);
		r = true;

	} else {
		SmartPosterRecord Sp(record);

		QNdefNfcUriRecord U;
		if (Sp.uri(U) == false) {
			goto exit;
		}
		m_url->setText(U.uri().toString());

		QList<QNdefNfcTextRecord> T = Sp.titles();
		if (T.length() != 0) {
			m_title->setContents(T[0].text());
			m_title->setLanguage(T[0].locale());
			m_titleButton->setChecked(true);
		} else {
			m_titleButton->setChecked(true);
		}

		r = true;
	}

exit:
	updateSize();
	return r;
}

QNdefMessage UrlPage::prepareDataForStorage(void)
{
	QNdefMessage message;

	if (m_titleButton->isChecked() == false) {
		mDebug(__func__) << "Creating U, no title set. ";
		QNdefNfcUriRecord U;
		U.setUri(m_url->text());
		message << U;
	} else {
		mDebug(__func__) << "Creating Sp, title set. ";

		SmartPosterRecord Sp;

		QNdefNfcUriRecord U;
		U.setUri(m_url->text());
		Sp.setUri(U);

		QList<QNdefNfcTextRecord> titles;
		QNdefNfcTextRecord title;
		title.setText(m_title->contents());
		title.setLocale(m_title->language());
		titles << title;
		Sp.setTitles(titles);

		message << Sp;

	}
	return message;
}

void UrlPage::titleIncludedChanged(bool checked)
{
	(void) checked;
	updateSize();
}

void UrlPage::titleChanged(void)
{
	updateSize();
}

void UrlPage::titleLanguageChanged(void)
{
	updateSize();
}

void UrlPage::urlChanged(void)
{
	/* TODO: how about checking also that the URL is a valid one */
	setContentValidity(m_url->text() != "" ? true : false);
	updateSize();
}

void UrlPage::updateSize(void)
{
	/* A bunch of calculations could be done without creating the
	   NDEF record, but that'd include checking the URL prefixes
	   and all. Easier just to make a scratch message. */ 
	QNdefMessage message = prepareDataForStorage();
	quint32 ndefLength = Util::messageLength(message);
	setContentSize(ndefLength);
}
