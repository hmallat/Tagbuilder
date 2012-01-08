/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "UnknownPage.h"
#include "UnknownRecordListModel.h"
#include "UnknownRecordListCellCreator.h"
#include "LabelOrList.h"
#include "Util.h"

#include <MLabel>
#include <MButton>
#include <MMessageBox>
#include <MContentItem>
#include <QGraphicsLinearLayout>

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new UnknownRecordListCellCreator;
}

UnknownPage::UnknownPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_model(new UnknownRecordListModel(this))
{
}

UnknownPage::~UnknownPage(void)
{
}

void UnknownPage::createPageSpecificActions(void)
{
}

void UnknownPage::createPageSpecificContent(void)
{
	MLabel *title = new MLabel(tr("This tag's contents are unsupported "
				      "and cannot be edited. "
				      "However you can still write the tag "
				      "from the main screen."));
	title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	title->setAlignment(Qt::AlignCenter);
	title->setWordWrap(true);
	layout()->addItem(title);
	layout()->setAlignment(title, Qt::AlignCenter);

	m_unknownDetails = new LabelOrList(m_model,
					   _getCreator,
					   tr("Empty message"),
					   false,
					   false);
	m_unknownDetails->setSizePolicy(QSizePolicy::Preferred, 
					QSizePolicy::Preferred);
	layout()->addItem(m_unknownDetails);
	layout()->setAlignment(m_unknownDetails, Qt::AlignCenter);
}

void UnknownPage::setupNewData(void) 
{
	m_message = QNdefMessage();
	m_model->setMessage(m_message);
	updateSize();
}

bool UnknownPage::setupData(const QNdefMessage message)
{
	m_message = message;
	m_model->setMessage(m_message);
	updateSize();
	return true;
}

QNdefMessage UnknownPage::prepareDataForStorage(void)
{
	return m_message;
}

void UnknownPage::updateSize(void)
{
	setContentSize(Util::messageLength(m_message));
}
