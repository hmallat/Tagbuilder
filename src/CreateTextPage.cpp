/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CreateTextPage.h"
#include "TextRecordEdit.h"

#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MAction>

#include <MDebug>

/* TODO: show a locale specific human readable language name 
   instead of two-letter code (study QLocale) */

/* TODO: figure out a way to make a component out of the text input, 
   language selection, and byte counter fields that can be added to
   other layouts. Smarmyposter editor needs that. */

CreateTextPage::CreateTextPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_edit(NULL),
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

	m_edit = new TextRecordEdit(m_sysinfo->availableLanguages(),
				    m_sysinfo->currentLanguage());
	layout->addItem(m_edit);
	layout->setAlignment(m_edit, Qt::AlignHCenter);
	layout->setStretchFactor(m_edit, 999);

	centralWidget()->setLayout(layout);
}

void CreateTextPage::saveTag(void)
{
	/* TODO: write to tag storage and go back to main screen */
}
