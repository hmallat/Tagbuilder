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
#include <QSignalMapper>
#include <MContainer>
#include <MLabel>
#include <MButton>

#include <MDebug>

UrlPage::UrlPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_sysinfo(new QSystemInfo(this)),
	  m_titleRemoveMapper(new QSignalMapper(this)),
	  m_titleChangeMapper(new QSignalMapper(this)),
	  m_url(0),
	  m_addTitle(0),
	  m_titleLayout(0),
	  m_titles()
{
}

UrlPage::~UrlPage(void)
{
}

void UrlPage::createPageSpecificContent(void)
{
	m_url = new LabeledTextEdit(MTextEditModel::SingleLine,
				    tr("Bookmark URL"),
				    tr("Enter bookmark URL"),
				    "http://");
	m_url->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout()->addItem(m_url);
	layout()->setAlignment(m_url, Qt::AlignCenter);
	connect(m_url->textEdit(), SIGNAL(textChanged()),
		this, SLOT(urlChanged(void)));

	{
		MContainer *container = new MContainer(tr("Titles"));
		container->setSizePolicy(QSizePolicy::Minimum, 
					 QSizePolicy::Minimum);
		
		m_titleLayout =
			new QGraphicsLinearLayout(Qt::Vertical,
						  container->centralWidget());
		m_titleLayout->setSizePolicy(QSizePolicy::Minimum, 
					     QSizePolicy::Minimum);

		m_addTitle = new MButton();
		m_addTitle->setText("Add a title");
		m_addTitle->setSizePolicy(QSizePolicy::Minimum, 
					  QSizePolicy::Fixed);
		m_titleLayout->addItem(m_addTitle);
		m_titleLayout->setAlignment(m_addTitle, Qt::AlignCenter);
		connect(m_addTitle, SIGNAL(clicked()),
			this, SLOT(addTitle()));

		layout()->addItem(container);
		layout()->setAlignment(container, Qt::AlignCenter);
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
		m_url->textEdit()->setText(U.uri().toString());
		r = true;

	} else {
		SmartPosterRecord Sp(record);

		QNdefNfcUriRecord U;
		if (Sp.uri(U) == false) {
			goto exit;
		}
		m_url->textEdit()->setText(U.uri().toString());

		QList<QNdefNfcTextRecord> T = Sp.titles();
#if 0 /* TODO for reals */
		if (T.length() != 0) {
			mDebug(__func__) << "Setting locale to " << T[0].locale();
			m_title->setContents(T[0].text());
			m_title->setLanguage(T[0].locale());
			m_titleButton->setChecked(true);
		} else {
			m_titleButton->setChecked(true);
		}
#endif

		r = true;
	}

exit:
	updateSize();
	return r;
}

QNdefMessage UrlPage::prepareDataForStorage(void)
{
	QNdefMessage message;

	if (m_titles.length() == 0) {
		mDebug(__func__) << "Creating U, no title set. ";
		QNdefNfcUriRecord U;
		U.setUri(m_url->textEdit()->text());
		message << U;
	} else {
		mDebug(__func__) << "Creating Sp, title set. ";
#if 0 /* TODO for reals */
		SmartPosterRecord Sp;

		QNdefNfcUriRecord U;
		U.setUri(m_url->textEdit()->text());
		Sp.setUri(U);

		QList<QNdefNfcTextRecord> titles;
		QNdefNfcTextRecord title;
		title.setText(m_title->contents());
		title.setLocale(m_title->language());
		titles << title;
		Sp.setTitles(titles);

		message << Sp;
#endif

	}
	return message;
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

void UrlPage::urlChanged(void)
{
	/* TODO: how about checking also that the URL is a valid one */
	setContentValidity(m_url->textEdit()->text() != "" ? true : false);
	updateSize();
}

void UrlPage::titleChanged(QObject *which)
{
	/* TODO for reals */
	(void) which;
	updateSize();
}

void UrlPage::addTitle(void)
{
	/* Add the new title edit just before the add button */

	int pos = m_titleLayout->count() - 1;

	QGraphicsLinearLayout *pack =
		new QGraphicsLinearLayout(Qt::Horizontal);

	TextRecordEdit *title = 
		new TextRecordEdit("",
				   tr("Enter title"),
				   m_sysinfo->availableLanguages(),
				   m_sysinfo->currentLanguage(),
				   "",
				   false);
	title->setSizePolicy(QSizePolicy::Minimum, 
			     QSizePolicy::Fixed);
	pack->addItem(title);
	pack->setAlignment(title, Qt::AlignLeft | Qt::AlignVCenter);
	pack->setStretchFactor(title, 999);
	connect(title, SIGNAL(contentsChanged()),
		m_titleChangeMapper, SLOT(map()));
	connect(title, SIGNAL(languageChanged()),
		m_titleChangeMapper, SLOT(map()));
	m_titleChangeMapper->setMapping(title, title);

	MButton *xButton = new MButton("icon-s-cancel", "");
	xButton->setViewType(MButton::iconType);
	xButton->setSizePolicy(QSizePolicy::Fixed, 
			       QSizePolicy::Fixed);
	pack->addItem(xButton);
	pack->setAlignment(xButton, Qt::AlignRight | Qt::AlignVCenter);
	pack->setStretchFactor(xButton, 0);
	connect(xButton, SIGNAL(clicked()),
		m_titleRemoveMapper, SLOT(map()));
	m_titleRemoveMapper->setMapping(xButton, title);

	m_titleLayout->insertItem(pos, pack);
	m_titleLayout->setAlignment(pack, Qt::AlignHCenter);

	m_titles << title;
}

void UrlPage::removeTitle(QObject *which)
{
	/* TODO for reals */
	(void) which;
}
