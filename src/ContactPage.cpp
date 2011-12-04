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
#include "LabeledTextEdit.h"
#include "TagStorage.h"
#include "Tag.h"

#include <MAction>
#include <MLabel>
#include <MButton>
#include <MMessageBox>
#include <MContentItem>
#include <QGraphicsAnchorLayout>
#include <QGraphicsLinearLayout>
#include <QContactLocalIdFilter>
#include <QVersitContactExporter>
#include <QVersitContactImporter>
#include <QVersitReader>
#include <QVersitWriter>

#include <MDebug>

ContactPage::ContactPage(int tag, QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_tag(tag),
	  m_name(0),
	  m_contact(0),
	  m_cancelAction(0),
	  m_storeAction(0),
	  m_fetch(0),
	  m_info()
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

ContactPage::~ContactPage(void)
{
}

void ContactPage::createContent(void)
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

	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical);

	MLabel *label = new MLabel(m_tag == -1 
				   ? tr("<big>Create tag contents</big>")
				   : tr("<big>Edit tag contents</big>"));
	label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	label->setAlignment(Qt::AlignCenter);
	layout->addItem(label);
	layout->setAlignment(label, Qt::AlignCenter);

	m_name = new LabeledTextEdit(MTextEditModel::SingleLine,
				     tr("Tag name"),
				     tr("Enter tag name"));
	if (m_tag != -1) {
		m_name->setText(TagStorage::tag(m_tag)->name());
	}
	m_name->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout->addItem(m_name);
	layout->setAlignment(m_name, Qt::AlignLeft);
	connect(m_name, SIGNAL(textChanged(void)),
		this, SLOT(nameChanged(void)));

	MLabel *selected = new MLabel(tr("Selected contact"));
	selected->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	selected->setAlignment(Qt::AlignLeft);
	layout->addItem(selected);
	layout->setAlignment(selected, Qt::AlignLeft);

	m_contact = new MContentItem(MContentItem::IconAndSingleTextLabel);
	m_contact->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	if (m_tag != -1) {
		importContact();
	} else {
		setContact(QContact());
	}
	layout->addItem(m_contact);
	layout->setAlignment(m_contact, Qt::AlignCenter);

	{
		QGraphicsLinearLayout *sub_layout = 
			new QGraphicsLinearLayout(Qt::Vertical);

#if 0 /* does not seem to work too well for now */
		if (m_contactManager.selfContactId() != 0) {
			mDebug(__func__) << "Nonzero self ID is " 
					 << m_contactManager.selfContactId();
			MButton *this_button = 
				new MButton(tr("Use my information"));
			this_button->setSizePolicy(QSizePolicy::Minimum, 
						   QSizePolicy::Fixed);
			sub_layout->addItem(this_button);
			connect(this_button, SIGNAL(clicked()),
				this, SLOT(useMyInformation()));
		}
#endif

		MButton *pick_button = 
			new MButton(tr("Choose from contacts"));
		sub_layout->addItem(pick_button);
		connect(pick_button, SIGNAL(clicked()),
			this, SLOT(chooseFromAddressbook()));
	
		layout->addItem(sub_layout);
		layout->setAlignment(sub_layout, Qt::AlignCenter);
	}
	
	centralWidget()->setLayout(layout);
}

void ContactPage::nameChanged(void)
{
	if (m_name->text() == "") {
		/* cannot store without a name */
		m_storeAction->setEnabled(false);
	} else {
		m_storeAction->setEnabled(true);
	}
}

void ContactPage::useMyInformation(void)
{
	/* This doesn't seem to work too well */

	if (m_fetch != 0) { /* cancel old request */
		delete m_fetch;
		m_fetch = 0;
	}

	QContactLocalIdFilter selfFilter;
	selfFilter.add(m_contactManager.selfContactId());

	m_fetch = new QContactFetchRequest(this);
	m_fetch->setManager(&m_contactManager);
	m_fetch->setFilter(selfFilter);
	connect(m_fetch, SIGNAL(resultsAvailable()),
		this, SLOT(singleContactFetched()));

	mDebug(__func__) << "Starting self request. ";

	if (m_fetch->start() == false) {
		mDebug(__func__) << "Cannot fetch self contact. ";
		delete m_fetch;
		m_fetch = 0;
	}
}

void ContactPage::chooseFromAddressbook(void)
{
	ContactSelectionPage *page = 
		new ContactSelectionPage(&m_contactManager);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const QContact)),
		this, SLOT(setContact(const QContact)));
}

void ContactPage::singleContactFetched(void)
{
	mDebug(__func__) << "Got response to self request. ";

	QList<QContact> results = m_fetch->contacts();
	if (results.length() == 0) {
		MMessageBox *box = 
			new MMessageBox(tr("Cannot get contact information."));
		box->appear();
	} else {
		setContact(results[0]);
	}
	delete m_fetch;
	m_fetch = 0;
}

void ContactPage::setContact(const QContact contact)
{
	mDebug(__func__) << contact.displayLabel();
	m_info = contact;
	m_contact->setImageID("icon-m-content-avatar-placeholder"); /* TODO */
	m_contact->setTitle(m_info.isEmpty() 
			    ? "No one selected"
			    : m_info.displayLabel());
}

void ContactPage::storeTag(void)
{
	bool success;
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

	if (m_tag == -1) {
		success = TagStorage::append(m_name->text(), message);
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

void ContactPage::importContact(void)
{
	QNdefRecord r = TagStorage::tag(m_tag)->message().at(0);
	VCardNdefRecord vc(TagStorage::tag(m_tag)->message().at(0));
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
	return;

fail:
	MMessageBox *box = new MMessageBox(tr("Cannot read the tag. "));
	box->appear();
}
