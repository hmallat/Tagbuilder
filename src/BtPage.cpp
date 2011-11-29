/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

/* TODO: check what happens if BT is off */

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
#include <QGraphicsLinearLayout>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>

#include <MDebug>

/* NB: looks like Qt hostinfo doesn't return the class of
   device. Could ask from BlueZ, but that would be just as unportable
   as hardcoding it (no BlueZ in Symbian!), so here goes. */
#define N9_COD 0x5a020c

/* NB: looks like Qt doesn't support retrieving a list of already
   known devices. So implement that using BlueZ D-Bus interface. That
   cannot be hardcoded... */

BtPage::BtPage(int tag, QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_tag(tag),
	  m_name(0),
	  m_device(0),
	  m_cancelAction(0),
	  m_storeAction(0),
	  m_discovery(new QBluetoothDeviceDiscoveryAgent(this))
				  
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
	connect(m_discovery, 
		SIGNAL(deviceDiscovered(const QBluetoothDeviceInfo &)),
		this, 
		SLOT(deviceDiscovered(const QBluetoothDeviceInfo &)));

	connect(m_discovery, 
		SIGNAL(error(QBluetoothDeviceDiscoveryAgent::Error)),
		this, 
		SLOT(discoveryError(QBluetoothDeviceDiscoveryAgent::Error)));

	connect(m_discovery, SIGNAL(finished()),
		this, SLOT(discoveryFinished()));

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

	MLabel *selected = new MLabel(tr("Selected device"));
	selected->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	selected->setAlignment(Qt::AlignLeft);
	layout->addItem(selected);
	layout->setAlignment(selected, Qt::AlignLeft);

	m_device = new MContentItem();
	m_device->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	if (m_tag != -1) {
		/* TODO */
	} else {
		setDevice(QBluetoothDeviceInfo());
	}
	layout->addItem(m_device);
	layout->setAlignment(m_device, Qt::AlignCenter);

	{
		QGraphicsLinearLayout *sub_layout = 
			new QGraphicsLinearLayout(Qt::Vertical);

		MButton *this_button = 
			new MButton(tr("Choose this phone"));
		this_button->setSizePolicy(QSizePolicy::Minimum, 
					   QSizePolicy::Fixed);
		sub_layout->addItem(this_button);
		connect(this_button, SIGNAL(clicked()),
			this, SLOT(choosePhoneBT()));

		MButton *pick_button = 
			new MButton(tr("Choose a paired device"));
		sub_layout->addItem(pick_button);
		connect(pick_button, SIGNAL(clicked()),
			this, SLOT(chooseExistingBT()));
	
		MButton *scan_button = new MButton(tr("Scan for a device"));
		sub_layout->addItem(scan_button);
		connect(scan_button, SIGNAL(clicked()),
			this, SLOT(chooseScannedBT()));

		layout->addItem(sub_layout);
		layout->setAlignment(sub_layout, Qt::AlignCenter);
	}
	
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

void BtPage::choosePhoneBT(void)
{
	QList<QBluetoothHostInfo> locals = QBluetoothLocalDevice::allDevices();
	if (locals.length() > 0) {
		QBluetoothDeviceInfo info(locals[0].getAddress(),
					  locals[0].getName(),
					  N9_COD);
		setDevice(info);
		return;
	}

	MMessageBox *box = 
		new MMessageBox(tr("Cannot get phone Bluetooth "
				   "information. "));
	box->appear();
}

void BtPage::chooseExistingBT(void)
{
	/* TODO: should bring up a list popup here */
}

void BtPage::chooseScannedBT(void)
{
	mDebug(__func__) << "Starting discovery...";
	/* TODO: should bring up a list popup here */
	m_discovery->start();
}

void BtPage::deviceDiscovered(const QBluetoothDeviceInfo &info)
{
	mDebug(__func__) << "Found " << info.address().toString();
}

void BtPage::discoveryError(QBluetoothDeviceDiscoveryAgent::Error err)
{
	mDebug(__func__) << "Error " << err;
}

void BtPage::discoveryFinished(void)
{
	mDebug(__func__) << "done. ";
}

void BtPage::setDevice(const QBluetoothDeviceInfo &info)
{
	m_info = info;

	m_device->setImageID("icon-m-content-bluetooth"); /* TODO */
	m_device->setTitle(m_info.isValid()
			   ? m_info.name()
			   : tr("No device selected"));
	m_device->setSubtitle(m_info.isValid()
			      ? m_info.address().toString()
			      : "00:00:00:00:00:00");
}

