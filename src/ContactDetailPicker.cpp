/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactDetailPicker.h"
#include "ContactDetailPickerListCellCreator.h"
#include "ContactDetailPickerListModel.h"
#include "LabelOrList.h"
#include "Util.h"

#include <QContactDetailDefinition>
#include <QContactManager>
#include <QContactName>
#include <QContactPhoneNumber>
#include <QContactAddress>
#include <QContactEmailAddress>
#include <QContactOnlineAccount>

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MList>

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new ContactDetailPickerListCellCreator;
}

ContactDetailPicker::ContactDetailPicker(const QContact contact,
					 QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_contact(contact),
	  m_model(new ContactDetailPickerListModel(m_contact, this))
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

ContactDetailPicker::~ContactDetailPicker(void)
{
}

void ContactDetailPicker::createContent(void)
{
	m_cancelAction = new MAction(tr("Cancel"), this);
	m_cancelAction->setLocation(MAction::ToolBarLocation);
	connect(m_cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(m_cancelAction);

	m_doneAction = new MAction(tr("Done"), this);
	m_doneAction->setLocation(MAction::ToolBarLocation);
	connect(m_doneAction, SIGNAL(triggered()),
		this, SLOT(picked()));
	addAction(m_doneAction);

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
	centralWidget()->setLayout(layout);

	MLabel *titleLabel = new MLabel(tr("Select details"));
	titleLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	titleLabel->setAlignment(Qt::AlignLeft);
	layout->addCornerAnchors(titleLabel, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);

	m_list = new LabelOrList(m_model,
				 _getCreator,
				 tr("<h1>No details to select</h1>"),
				 true,
				 true);
	
	layout->addCornerAnchors(m_list, Qt::TopLeftCorner,
				 titleLabel, Qt::BottomLeftCorner);
	layout->addCornerAnchors(m_list, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);
}