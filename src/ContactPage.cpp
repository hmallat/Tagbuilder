/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactPage.h"
#include "ContactSelectionPage.h"
#include "ContactDetailPicker.h"
#include "ContactDetailPickerListCellCreator.h"
#include "ContactDetailPickerListModel.h"
#include "LabelOrList.h"
#include "Util.h"
#include "Tag.h"

#include <MLabel>
#include <MAction>
#include <MButton>
#include <QGraphicsLinearLayout>

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new ContactDetailPickerListCellCreator("UnselectableContentItem");
}

ContactPage::ContactPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_info(),
	  m_contactDetails(0),
	  m_model(new ContactDetailPickerListModel(Util::AllContactDetails, 
						   this))
{
}

ContactPage::~ContactPage(void)
{
}

void ContactPage::createPageSpecificActions(void)
{
	MAction *pickAction = new MAction(tr("Contact from addressbook..."),
					  this);
	pickAction->setLocation(MAction::ApplicationMenuLocation);
	connect(pickAction, SIGNAL(triggered()),
		this, SLOT(chooseFromAddressbook()));
	addAction(pickAction);
}

void ContactPage::createPageSpecificContent(void)
{
	QList<MButton *> buttons;

	MButton *aButton = new MButton();
	aButton->setText(tr("Addressbook contact"));
	aButton->setSizePolicy(QSizePolicy::Preferred, 
			       QSizePolicy::Fixed);
	connect(aButton, SIGNAL(clicked()), 
		this, SLOT(chooseFromAddressbook()));
	buttons << aButton;

	m_contactDetails = new LabelOrList(m_model,
					   _getCreator,
					   tr("Select a contact"),
					   true,
					   false,
					   buttons);
	m_contactDetails->setSizePolicy(QSizePolicy::Preferred, 
					QSizePolicy::Preferred);
	layout()->addItem(m_contactDetails);
	layout()->setAlignment(m_contactDetails, Qt::AlignCenter);
}

void ContactPage::setupNewData(void) 
{
	setContact(QContact());
}

bool ContactPage::setupData(const QNdefMessage message)
{
	QContact c = Util::contactFromNdef(message);
	if (c.isEmpty() == true) {
		setContact(QContact());
		return false;
	}

	setContact(c);
	return true;
}

QNdefMessage ContactPage::prepareDataForStorage(void)
{
	return Util::ndefFromContact(m_info);
}

void ContactPage::chooseFromAddressbook(void)
{
	ContactSelectionPage *page = 
		new ContactSelectionPage(&m_contactManager);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const QContact)),
		this, SLOT(setContactDetails(const QContact)));
}

void ContactPage::setContactDetails(const QContact contact)
{
 	mDebug(__func__) << contact.displayLabel();
	ContactDetailPicker *picker = new ContactDetailPicker(contact);
	picker->appear(MSceneWindow::DestroyWhenDismissed);
	connect(picker, SIGNAL(contactPicked(const QContact)),
		this, SLOT(setContactThroughAction(const QContact)));
}

void ContactPage::setContact(const QContact contact)
{
	mDebug(__func__) << contact.displayLabel();
	m_info = contact;

	m_model->setContact(m_info);

	setContentValidity(m_info.isEmpty() ? false : true);
#ifdef LABEL_SIZE
	updateSize();
#endif
}

void ContactPage::setContactThroughAction(const QContact contact)
{
	setContact(contact);
	setDefaultName(contact.displayLabel());
}

#ifdef LABEL_SIZE
void ContactPage::updateSize(void)
{
	/* No easy way to calculate the size of the exported VCARD file,
	   so make a message out of it... */
	QNdefMessage message = prepareDataForStorage();
	quint32 ndefLength = Util::messageLength(message);
	setContentSize(ndefLength);
}
#endif

