/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CreateEditPage.h"
#include "LabeledTextEdit.h"
#include "TagStorage.h"
#include "Tag.h"

#include <MAction>
#include <MLabel>
#include <MMessageBox>
#include <MPannableViewport>
#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>

#include <MDebug>

CreateEditPage::CreateEditPage(int tag, QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_tag(tag),
	  m_cancelAction(0),
	  m_storeAction(0),
	  m_layout(0),
	  m_size(0),
	  m_name(0),
	  m_contentValidity(false),
	  m_nameValidity(false)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

CreateEditPage::~CreateEditPage(void)
{
}

void CreateEditPage::createContent(void)
{
	setPannable(false);

	m_cancelAction = new MAction(tr("Cancel"), this);
	m_cancelAction->setLocation(MAction::ToolBarLocation);
	connect(m_cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(m_cancelAction);

	m_storeAction = new MAction(tr("Store"), this);
	m_storeAction->setLocation(MAction::ToolBarLocation);
	connect(m_storeAction, SIGNAL(triggered()),
		this, SLOT(storeAndExit()));
	addAction(m_storeAction);

	QGraphicsAnchorLayout *anchor = new QGraphicsAnchorLayout();
	anchor->setSizePolicy(QSizePolicy::Minimum, 
			      QSizePolicy::Minimum);

	m_size = new MLabel();
	m_size->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	m_size->setAlignment(Qt::AlignRight);
	anchor->addAnchor(m_size, Qt::AnchorBottom, anchor, Qt::AnchorBottom);
	anchor->addAnchor(m_size, Qt::AnchorRight, anchor, Qt::AnchorRight);

	MWidget *layoutContainer = new MWidget();
	layoutContainer->setSizePolicy(QSizePolicy::Minimum, 
				       QSizePolicy::Minimum);
	m_layout = new QGraphicsLinearLayout(Qt::Vertical, layoutContainer);
	m_layout->setSizePolicy(QSizePolicy::Minimum, 
				QSizePolicy::Minimum);

	MLabel *label = new MLabel(m_tag == -1 
				   ? tr("<big>Create tag contents</big>")
				   : tr("<big>Edit tag contents</big>"));
	label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	label->setAlignment(Qt::AlignCenter);
	m_layout->addItem(label);
	m_layout->setAlignment(label, Qt::AlignCenter);

	m_name = new LabeledTextEdit(MTextEditModel::SingleLine,
				     tr("Tag name"),
				     tr("Enter tag name"));
	m_name->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	m_layout->addItem(m_name);
	m_layout->setAlignment(m_name, Qt::AlignLeft);
	connect(m_name, SIGNAL(textChanged(void)),
		this, SLOT(nameChanged(void)));

	createPageSpecificContent();

	MPannableViewport *view = new MPannableViewport();
	view->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	view->setPanDirection(Qt::Vertical);
	view->setMinimumSize(100, 100);
	view->setWidget(layoutContainer);

	anchor->addAnchor(view, Qt::AnchorTop, anchor, Qt::AnchorTop);
	anchor->addAnchor(view, Qt::AnchorBottom, m_size, Qt::AnchorTop);
	anchor->addAnchor(view, Qt::AnchorLeft, anchor, Qt::AnchorLeft);
	anchor->addAnchor(view, Qt::AnchorRight, anchor, Qt::AnchorRight);

	centralWidget()->setLayout(anchor);

	if (m_tag == -1) {
		setContentValidity(false);
		setNameValidity(false);
		setupNewData();
	} else {
		load();
	}
}

QGraphicsLinearLayout *CreateEditPage::layout(void)
{
	return m_layout;
}

void CreateEditPage::updateUI(void)
{
	if (m_nameValidity == false || m_contentValidity == false) {
		m_storeAction->setEnabled(false);
	} else {
		m_storeAction->setEnabled(true);
	}
}

void CreateEditPage::nameChanged(void)
{
	setNameValidity(m_name->text() != "" ? true : false);
}

void CreateEditPage::setNameValidity(bool valid)
{
	m_nameValidity = valid;
	updateUI();
}

void CreateEditPage::setContentValidity(bool valid)
{
	m_contentValidity = valid;
	updateUI();
}

void CreateEditPage::setContentSize(quint32 bytes)
{
	m_size->setText(tr("%1 bytes").arg(bytes));
}

void CreateEditPage::load(void)
{
	const Tag *tag = TagStorage::tag(m_tag);	

	m_name->setText(tag->name());
	setNameValidity(true);

	if (setupData(tag->message()) == false) {
		goto fail;
	} else {
		setContentValidity(true);
	}

	return;

fail:
	setupNewData();
	MMessageBox *box = new MMessageBox(tr("Cannot read the tag. "));
	box->appear();	
}

void CreateEditPage::storeAndExit(void)
{
	bool success;

	QNdefMessage message = prepareDataForStorage();
	if (message.isEmpty() == true) {
		goto fail;
	}

	if (m_tag == -1) {
		success = TagStorage::append(m_name->text(), 
					     message);
	} else {
		success = TagStorage::update(m_tag,
					     m_name->text(),
					     message);
	}

	if (success == false) {
		goto fail;
	}

	dismiss();
	return;

fail:
	MMessageBox *box = new MMessageBox(tr("Cannot store the tag. "));
	box->appear();
}
