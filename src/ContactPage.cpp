/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactPage.h"
#include "ContactSelectionPage.h"
#include "VCardNdefRecord.h"
#include "Util.h"

#include <MLabel>
#include <MButton>
#include <MContentItem>
#include <QGraphicsLinearLayout>
#include <QContactLocalIdFilter>
#include <QVersitContactExporter>
#include <QVersitContactImporter>
#include <QVersitReader>
#include <QVersitWriter>

#include <MDebug>

ContactPage::ContactPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_contact(0),
	  m_info()
{
}

ContactPage::~ContactPage(void)
{
}

void ContactPage::createPageSpecificContent(void)
{
	MLabel *selected = new MLabel(tr("Selected contact"));
	selected->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	selected->setAlignment(Qt::AlignLeft);
	layout()->addItem(selected);
	layout()->setAlignment(selected, Qt::AlignLeft);

	m_contact = new MContentItem(MContentItem::IconAndSingleTextLabel);
	m_contact->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout()->addItem(m_contact);
	layout()->setAlignment(m_contact, Qt::AlignCenter);

	{
		QGraphicsLinearLayout *sub_layout = 
			new QGraphicsLinearLayout(Qt::Vertical);

		MButton *pick_button = 
			new MButton(tr("Choose from contacts"));
		sub_layout->addItem(pick_button);
		connect(pick_button, SIGNAL(clicked()),
			this, SLOT(chooseFromAddressbook()));
	
		layout()->addItem(sub_layout);
		layout()->setAlignment(sub_layout, Qt::AlignCenter);
	}
	
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
		this, SLOT(setContact(const QContact)));
}

void ContactPage::setContact(const QContact contact)
{
	mDebug(__func__) << contact.displayLabel();
	m_info = contact;
	m_contact->setImageID("icon-m-content-avatar-placeholder"); /* TODO */
	m_contact->setTitle(m_info.isEmpty() 
			    ? "No one selected"
			    : m_info.displayLabel());
	setContentValidity(m_info.isEmpty() ? false : true);
	updateSize();
}

void ContactPage::updateSize(void)
{
	/* No easy way to calculate the size of the exported VCARD file,
	   so make a message out of it... */
	QNdefMessage message = prepareDataForStorage();
	quint32 ndefLength = Util::messageLength(message);
	setContentSize(ndefLength);
}
