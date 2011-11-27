/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtPage.h"
#include "BtNdefRecord.h"
#include "LabeledTextEdit.h"
#include "TagStorage.h"
#include "Tag.h"

#include <MAction>
#include <MLabel>
#include <MButton>
#include <MMessageBox>
#include <MContentItem>
#include <QGraphicsAnchorLayout>

BtPage::BtPage(int tag, QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_tag(tag),
	  m_name(NULL),
	  m_device(NULL),
	  m_cancelAction(NULL),
	  m_storeAction(NULL)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

BtPage::~BtPage(void)
{
}

void BtPage::createContent(void)
{
	m_cancelAction = new MAction(tr("Cancel"), this);
	m_cancelAction->setLocation(MAction::ToolBarLocation);
	connect(m_cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(m_cancelAction);

	m_storeAction = new MAction(tr("Store"), this);
	if (m_tag == -1) {
		m_storeAction->setEnabled(false);
	}
	m_storeAction->setLocation(MAction::ToolBarLocation);
	connect(m_storeAction, SIGNAL(triggered()),
		this, SLOT(storeTag()));
	addAction(m_storeAction);

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();

	MLabel *label = new MLabel(m_tag == -1 
				   ? tr("<big>Create tag contents</big>")
				   : tr("<big>Edit tag contents</big>"));
	label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	label->setAlignment(Qt::AlignCenter);
	layout->addCornerAnchors(label, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);
	layout->addAnchor(label, Qt::AnchorRight,
			  layout, Qt::AnchorRight);

	m_name = new LabeledTextEdit(MTextEditModel::SingleLine,
				     tr("Tag name"),
				     tr("Enter tag name"));
	if (m_tag != -1) {
		m_name->setText(TagStorage::tag(m_tag)->name());
	}
	m_name->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout->addCornerAnchors(m_name, Qt::TopLeftCorner,
				 label, Qt::BottomLeftCorner);
	layout->addAnchor(m_name, Qt::AnchorRight,
			  layout, Qt::AnchorRight);
	connect(m_name, SIGNAL(textChanged(void)),
		this, SLOT(nameChanged(void)));

	MLabel *selected = new MLabel(tr("Selected device"));
	selected->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	selected->setAlignment(Qt::AlignLeft);
	layout->addCornerAnchors(selected, Qt::TopLeftCorner,
				 m_name, Qt::BottomLeftCorner);
	layout->addAnchor(selected, Qt::AnchorRight,
			  layout, Qt::AnchorRight);

	m_device = new MContentItem();
	if (m_tag != -1) {
		/* TODO */
	} else {
		m_device->setImageID("icon-m-content-bluetooth");
		m_device->setTitle(tr("No device selected"));
		m_device->setSubtitle("00:00:00:00:00:00");
	}
	layout->addCornerAnchors(m_device, Qt::TopLeftCorner,
				 selected, Qt::BottomLeftCorner);
	layout->addAnchor(m_device, Qt::AnchorRight,
			  layout, Qt::AnchorRight);

	MButton *this_button = new MButton(tr("Use this device"));
	layout->addAnchor(this_button, Qt::AnchorTop,
			  m_device, Qt::AnchorBottom);
	
	MButton *pick_button = new MButton(tr("Choose a paired device"));
	layout->addAnchor(pick_button, Qt::AnchorTop,
			  this_button, Qt::AnchorBottom);
	
	MButton *scan_button = new MButton(tr("Scan for a device"));
	layout->addAnchor(scan_button, Qt::AnchorTop,
			  pick_button, Qt::AnchorBottom);
	
	centralWidget()->setLayout(layout);
}

void BtPage::nameChanged(void)
{
	if (m_name->text() == "") {
		/* cannot store without a name */
		m_storeAction->setEnabled(false);
	} else {
		m_storeAction->setEnabled(true);
	}
}

void BtPage::storeTag(void)
{
	/* TODO */
}
