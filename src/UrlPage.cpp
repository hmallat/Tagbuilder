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
#include <QSignalMapper>
#include <MContainer>
#include <MLabel>
#include <MButton>
#include <MButtonGroup>

#include <MDebug>

UrlPage::UrlPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_titleRemoveMapper(new QSignalMapper(this)),
	  m_titleChangeMapper(new QSignalMapper(this)),
	  m_url(0),
	  m_addTitle(0),
	  m_titleLayout(0),
	  m_titles()
{
	connect(m_titleRemoveMapper, SIGNAL(mapped(QObject *)),
		this, SLOT(removeTitle(QObject *)));
	connect(m_titleChangeMapper, SIGNAL(mapped(QObject *)),
		this, SLOT(titleChanged(QObject *)));
}

UrlPage::~UrlPage(void)
{
}

void UrlPage::createPageSpecificContent(void)
{
	m_url = new LabeledTextEdit(LabeledTextEdit::SingleLineEditAndLabel);
	m_url->setLabel(tr("Bookmark URL"));
	m_url->setPrompt(tr("Enter bookmark URL"));
	m_url->setContents("http://");
	m_url->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout()->addItem(m_url);
	layout()->setAlignment(m_url, Qt::AlignCenter);
	connect(m_url, SIGNAL(contentsChanged()),
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

	{
		MContainer *container = new MContainer(tr("Action"));
		container->setSizePolicy(QSizePolicy::Minimum, 
					 QSizePolicy::Minimum);
		
		QGraphicsLinearLayout *sub_layout =
			new QGraphicsLinearLayout(Qt::Vertical,
						  container->centralWidget());
		sub_layout->setSizePolicy(QSizePolicy::Minimum, 
				      QSizePolicy::Minimum);
		sub_layout->setSpacing(0);

		actButton[NoAction] = new MButton();
		actButton[NoAction]->setText(tr("No action"));
		actButton[NoAction]->setStyleName("CommonTopSplitButton");
		actButton[NoAction]->setCheckable(true);
		actButton[NoAction]->setViewType(MButton::groupType);
		connect(actButton[NoAction], SIGNAL(clicked()),
			this, SLOT(actChanged()));

		actButton[DoAction] = new MButton();
		actButton[DoAction]->setText(tr("Execute"));
		actButton[DoAction]->setStyleName("CommonVerticalCenterSplitButton");
		actButton[DoAction]->setCheckable(true);
		actButton[DoAction]->setViewType(MButton::groupType);
		connect(actButton[DoAction], SIGNAL(clicked()),
			this, SLOT(actChanged()));

		actButton[SaveAction] = new MButton();
		actButton[SaveAction]->setText(tr("Save for later"));
		actButton[SaveAction]->setStyleName("CommonVerticalCenterSplitButton");
		actButton[SaveAction]->setCheckable(true);
		actButton[SaveAction]->setViewType(MButton::groupType);
		connect(actButton[SaveAction], SIGNAL(clicked()),
			this, SLOT(actChanged()));

		actButton[EditAction] = new MButton();
		actButton[EditAction]->setText(tr("Open for editing"));
		actButton[EditAction]->setStyleName("CommonBottomSplitButton");
		actButton[EditAction]->setCheckable(true);
		actButton[EditAction]->setViewType(MButton::groupType);
		connect(actButton[EditAction], SIGNAL(clicked()),
			this, SLOT(actChanged()));

		sub_layout->addItem(actButton[NoAction]);
		sub_layout->addItem(actButton[DoAction]);
		sub_layout->addItem(actButton[SaveAction]);
		sub_layout->addItem(actButton[EditAction]);

		MButtonGroup* group = new MButtonGroup(this);
		group->addButton(actButton[NoAction]);
		group->addButton(actButton[DoAction]);
		group->addButton(actButton[SaveAction]);
		group->addButton(actButton[EditAction]);
		actButton[NoAction]->setChecked(true);

		layout()->addItem(container);
		layout()->setAlignment(container, Qt::AlignCenter);
	}

}

enum UrlPage::Action UrlPage::checkedAction(void)
{
	return 
		(actButton[DoAction]->isChecked() == true) ? DoAction :
		(actButton[SaveAction]->isChecked() == true) ? SaveAction :
		(actButton[EditAction]->isChecked() == true) ? EditAction :
		NoAction;
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
		m_url->setContents(U.uri().toString());
		r = true;

	} else {
		SmartPosterRecord Sp(record);

		QNdefNfcUriRecord U;
		if (Sp.uri(U) == false) {
			goto exit;
		}
		m_url->setContents(U.uri().toString());

		QList<QNdefNfcTextRecord> T = Sp.titles();
		for (int i = 0; i < T.length(); i++) {
			addTitle();
			m_titles[i]->setContents(T[i].text());
			m_titles[i]->setLanguageCode(T[i].locale());
		}

		SmartPosterRecord::ActionRecord act;

		if (Sp.act(act) == true) {
			actButton[(act.action() == SmartPosterRecord::ActionRecord::ActionDo)
				  ? DoAction :
				  (act.action() == SmartPosterRecord::ActionRecord::ActionSave)
				  ? SaveAction :
				  (act.action() == SmartPosterRecord::ActionRecord::ActionEdit)
				  ? EditAction :
				  NoAction]->setChecked(true);
		} else {
			actButton[NoAction]->setChecked(true);
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
	enum Action action = checkedAction();

	if (m_titles.length() == 0 && action == NoAction) {
		mDebug(__func__) << "Creating U, no title or action set. ";
		QNdefNfcUriRecord U;
		U.setUri(m_url->contents());
		message << U;
	} else {
		mDebug(__func__) << "Creating Sp, title or action set. ";
		SmartPosterRecord Sp;

		QNdefNfcUriRecord U;
		U.setUri(m_url->contents());
		Sp.setUri(U);

		QList<QNdefNfcTextRecord> titles;
		for (int i = 0; i < m_titles.length(); i++) {
			QNdefNfcTextRecord title;
			title.setText(m_titles[i]->contents());
			title.setLocale(m_titles[i]->languageCode());
			titles << title;
		}
		Sp.setTitles(titles);

		if (action != NoAction) {
			SmartPosterRecord::ActionRecord act;
			act.setAction
				((action == DoAction) ? 
				 SmartPosterRecord::ActionRecord::ActionDo :
				 (action == SaveAction) ? 
				 SmartPosterRecord::ActionRecord::ActionSave :
				 SmartPosterRecord::ActionRecord::ActionEdit);
			Sp.setAct(act);
		}

		message << Sp;

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
	setContentValidity(m_url->contents() != "" ? true : false);
	updateSize();
}

void UrlPage::actChanged(void)
{
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
		new TextRecordEdit(LabeledTextEdit::SingleLineEditOnly);
	title->setPrompt(tr("Enter title"));
	title->setSizePolicy(QSizePolicy::Minimum, 
			     QSizePolicy::Fixed);
	pack->addItem(title);
	pack->setAlignment(title, Qt::AlignLeft | Qt::AlignVCenter);
	pack->setStretchFactor(title, 999);
	connect(title, SIGNAL(contentsChanged()),
		m_titleChangeMapper, SLOT(map()));
	connect(title, SIGNAL(languageCodeChanged()),
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
	int pos;

	for (pos = 0; pos < m_titles.length(); pos++) {
		if (m_titles[pos] == static_cast<TextRecordEdit *>(which)) {
			mDebug(__func__) << "Removing title from position " 
					 << pos;
			m_titles.removeAt(pos);

			QGraphicsLinearLayout *pack = 
				static_cast<QGraphicsLinearLayout *>
				(m_titleLayout->itemAt(pos));
			while (pack->count() != 0) {
				mDebug(__func__) << "Remove packed item. ";
				QGraphicsLayoutItem *item = pack->itemAt(0);
				pack->removeAt(0);
				delete item;
			}
			m_titleLayout->removeAt(pos);
			delete pack;

			break;
		}
	}
}
