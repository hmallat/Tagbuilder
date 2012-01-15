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
#include "ViewHeader.h"

#include <MAction>
#include <MLabel>
#include <MMessageBox>
#include <MPannableViewport>
#include <MSeparator>
#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>

#include <MDebug>

CreateEditPage::CreateEditPage(int tag, QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_tag(tag),
	  m_cancelAction(0),
	  m_storeAction(0),
	  m_layout(0),
#ifdef LABEL_SIZE
	  m_size(0),
#endif
	  m_name(0),
	  m_contentValidity(false),
	  m_nameValidity(false),
	  m_nameEdited(false)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

CreateEditPage::~CreateEditPage(void)
{
}

void CreateEditPage::createActions(void)
{
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

	createPageSpecificActions();

}

void CreateEditPage::createContent(void)
{
	MApplicationPage::createContent();
	setStyleName("CommonApplicationPage");
	setPannable(false);

	createActions();

	QGraphicsAnchorLayout *anchor = new QGraphicsAnchorLayout();
	anchor->setContentsMargins(0, 0, 0, 0);
	anchor->setSizePolicy(QSizePolicy::Minimum, 
			      QSizePolicy::Minimum);

	ViewHeader *header = 
		new ViewHeader(m_tag == TagStorage::NULL_TAG 
			       ? tr("Create tag contents")
			       : tr("Edit tag contents"));
	anchor->addCornerAnchors(header, Qt::TopLeftCorner,
				 anchor, Qt::TopLeftCorner);

#ifdef LABEL_SIZE
	m_size = new MLabel();
	m_size->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	m_size->setAlignment(Qt::AlignRight);
	anchor->addAnchor(m_size, Qt::AnchorBottom, anchor, Qt::AnchorBottom);
	anchor->addAnchor(m_size, Qt::AnchorRight, anchor, Qt::AnchorRight);
#endif

	MWidget *layoutContainer = new MWidget();
	layoutContainer->setSizePolicy(QSizePolicy::Preferred, 
				       QSizePolicy::Minimum);
	m_layout = new QGraphicsLinearLayout(Qt::Vertical, layoutContainer);
	m_layout->setSizePolicy(QSizePolicy::Preferred, 
				QSizePolicy::Minimum);

	m_name = new LabeledTextEdit(tr("Ok"),
				     LabeledTextEdit::SingleLineEditAndLabel);
	m_name->setLabel(tr("Tag name"));
	m_name->setPrompt(tr("Enter tag name"));
	m_name->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	m_layout->addItem(m_name);
	m_layout->setAlignment(m_name, Qt::AlignLeft);
	connect(m_name, SIGNAL(contentsChanged(void)),
		this, SLOT(nameChanged(void)));

	MSeparator *sep = new MSeparator;
	sep->setStyleName("CommonHorizontalSeparator");
	sep->setOrientation(Qt::Horizontal);
	layout()->addItem(sep);

	createPageSpecificContent();

	MPannableViewport *view = new MPannableViewport();
	view->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	view->setPanDirection(Qt::Vertical);
	view->setMinimumSize(100, 100);
	view->setWidget(layoutContainer);

	anchor->addAnchor(view, Qt::AnchorTop, header, Qt::AnchorBottom);
#ifdef LABEL_SIZE
	anchor->addAnchor(view, Qt::AnchorBottom, m_size, Qt::AnchorTop);
#else
	anchor->addAnchor(view, Qt::AnchorBottom, anchor, Qt::AnchorBottom);
#endif
	anchor->addAnchor(view, Qt::AnchorLeft, anchor, Qt::AnchorLeft);
	anchor->addAnchor(view, Qt::AnchorRight, anchor, Qt::AnchorRight);

	centralWidget()->setLayout(anchor);

	if (m_tag == TagStorage::NULL_TAG) {
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
	mDebug(__func__) << "Name changed. ";
	m_nameEdited = true;
	setNameValidity(m_name->contents() != "" ? true : false);
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

#ifdef LABEL_SIZE
void CreateEditPage::setContentSize(quint32 bytes)
{
	m_size->setText(tr("%1 bytes").arg(bytes));
}
#endif

void CreateEditPage::load(void)
{
	const Tag *tag = TagStorage::storage()->tag(m_tag);	

	m_name->setContents(tag->name());
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
	box->appear(MSceneWindow::DestroyWhenDismissed);	
}

void CreateEditPage::storeAndExit(void)
{
	bool success;

	QNdefMessage message = prepareDataForStorage();
	if (message.isEmpty() == true) {
		goto fail;
	}

	if (m_tag == TagStorage::NULL_TAG) {
		success = TagStorage::storage()->append(m_name->contents(), 
							message);
	} else if (m_tag == TagStorage::TEMPORARY_TAG) {
		TagStorage::storage()->remove(TagStorage::TEMPORARY_TAG);
		success = TagStorage::storage()->append(m_name->contents(), 
							message);
	} else {
		success = TagStorage::storage()->update(m_tag,
							m_name->contents(),
							message);
	}

	if (success == false) {
		goto fail;
	}

	dismiss();
	return;

fail:
	MMessageBox *box = new MMessageBox(tr("Cannot store the tag. "));
	box->appear(MSceneWindow::DestroyWhenDismissed);
}

void CreateEditPage::setDefaultName(const QString name)
{
	mDebug(__func__) << "Call to set default name to " << name;
	mDebug(__func__) << (m_tag == TagStorage::NULL_TAG ? "" : "non-")
			 << "null tag. ";
	mDebug(__func__) << (m_nameEdited == true ? "" : "not ")
			 << "edited. ";

	if (m_tag == TagStorage::NULL_TAG && m_nameEdited == false) {
		m_name->setContents(name);
		m_nameEdited = false;
		mDebug(__func__) << "Default name set!";
	}
}
