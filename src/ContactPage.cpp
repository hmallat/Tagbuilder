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
#include "VCardNdefRecord.h"
#include "Util.h"
#include "Tag.h"

#include <MLabel>
#include <MContainer>
#include <MAction>
#include <QGraphicsLinearLayout>
#include <QContactLocalIdFilter>
#include <QVersitContactExporter>
#include <QVersitContactImporter>
#include <QVersitReader>
#include <QVersitWriter>

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new ContactDetailPickerListCellCreator;
}

ContactPage::ContactPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_info(),
	  m_contactDetails(0),
	  m_model(new ContactDetailPickerListModel(Util::AllDetails, this))
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
	m_contactDetails = new LabelOrList(m_model,
					   _getCreator,
					   tr("Select a contact"),
					   true,
					   false);
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
	VCardNdefRecord vc(message[0]);
	QVersitReader reader(vc.payload());
	QVersitContactImporter importer;

	mDebug(__func__) << message[0].typeNameFormat();
	mDebug(__func__) << QString::fromAscii(message[0].type());
	mDebug(__func__) << QString::fromAscii(message[0].payload());
	mDebug(__func__) << Tag::type(message);

	if (reader.startReading() == false ||
	    reader.waitForFinished() == false) {
		mDebug(__func__) << "Reader fail, " << reader.error() << ". ";
		goto fail;
	}

	if (reader.results().length() == 0) {
		mDebug(__func__) << "No results. ";
		goto fail;
	}

	if (importer.importDocuments(reader.results()) == false ||
	    importer.contacts().length() == 0) {
		mDebug(__func__) << "Importer fail. ";
		goto fail;
	}

	setContact(importer.contacts()[0]);
	return true;

fail:
	return false;
}

QNdefMessage ContactPage::prepareDataForStorage(void)
{
	QNdefMessage message;
	VCardNdefRecord vc;
	QByteArray data;
	QVersitWriter writer(&data);
	QVersitContactExporter exporter;

	if (exporter.exportContacts(QList<QContact>() << m_info,
				    QVersitDocument::VCard30Type) == false) {
		goto fail;
	}

	if (writer.startWriting(exporter.documents()) == false ||
	    writer.waitForFinished() == false) {
		goto fail;
	}

	vc.setPayload(data);
	message << vc;
	return message;
	
fail:
	return QNdefMessage();
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
	updateSize();
}

void ContactPage::setContactThroughAction(const QContact contact)
{
	setContact(contact);
	setDefaultName(contact.displayLabel());
}

void ContactPage::updateSize(void)
{
	/* No easy way to calculate the size of the exported VCARD file,
	   so make a message out of it... */
	QNdefMessage message = prepareDataForStorage();
	quint32 ndefLength = Util::messageLength(message);
	setContentSize(ndefLength);
}

