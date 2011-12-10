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
#include "BluezSupplicant.h"
#include "BtSelectionPage.h"

#include <MLabel>
#include <MButton>
#include <MMessageBox>
#include <MContentItem>
#include <QGraphicsLinearLayout>
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

BtPage::BtPage(BluezSupplicant *bluez, int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_device(0),
	  m_info(),
	  m_bluez(bluez)
{
}

BtPage::~BtPage(void)
{
}

void BtPage::createPageSpecificContent(void)
{
	MLabel *selected = new MLabel(tr("Selected device"));
	selected->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	selected->setAlignment(Qt::AlignLeft);
	layout()->addItem(selected);
	layout()->setAlignment(selected, Qt::AlignLeft);

	m_device = new MContentItem();
	m_device->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout()->addItem(m_device);
	layout()->setAlignment(m_device, Qt::AlignCenter);

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
			new MButton(tr("Choose a known device"));
		sub_layout->addItem(pick_button);
		connect(pick_button, SIGNAL(clicked()),
			this, SLOT(chooseExistingBT()));
	
		MButton *scan_button = new MButton(tr("Scan for a device"));
		sub_layout->addItem(scan_button);
		connect(scan_button, SIGNAL(clicked()),
			this, SLOT(chooseScannedBT()));

		layout()->addItem(sub_layout);
		layout()->setAlignment(sub_layout, Qt::AlignCenter);
	}
	
}

void BtPage::setupNewData(void) 
{
	setDevice(QBluetoothDeviceInfo());
}

bool BtPage::setupData(const QNdefMessage message)
{
	BtNdefRecord bt(message[0]);
	setDevice(QBluetoothDeviceInfo(bt.address(), 
				       bt.name(), 
				       bt.classOfDevice()));
	return true;
}

QNdefMessage BtPage::prepareDataForStorage(void)
{
	QNdefMessage message;

	quint32 cod =
		((m_info.serviceClasses() & 0x7ff) << 13) | 
		((m_info.majorDeviceClass() & 0x1f) << 8) |
		((m_info.minorDeviceClass() & 0x3f) << 2);

	BtNdefRecord bt;
	bt.setAddress(m_info.address());
	bt.setClassOfDevice(cod);
	bt.setName(m_info.name());
	message << bt;

	return message;
}

void BtPage::noBluetoothAlert(void)
{
	MMessageBox *box = new MMessageBox(tr("Bluetooth is not available. "));
	box->appear();
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

	noBluetoothAlert();
}

void BtPage::chooseExistingBT(void)
{
	if (m_bluez->isAvailable() == false) {
		noBluetoothAlert();
		return;
	}

	BtSelectionPage *page = 
		new BtSelectionPage(m_bluez, 
				    BtSelectionPage::SelectFromExisting);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const QBluetoothDeviceInfo)),
		this, SLOT(setDevice(const QBluetoothDeviceInfo)));
}

void BtPage::chooseScannedBT(void)
{
	if (m_bluez->isAvailable() == false) {
		noBluetoothAlert();
		return;
	}

	BtSelectionPage *page = 
		new BtSelectionPage(m_bluez, 
				    BtSelectionPage::SelectFromScanned);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const QBluetoothDeviceInfo)),
		this, SLOT(setDevice(const QBluetoothDeviceInfo)));
}

void BtPage::setDevice(const QBluetoothDeviceInfo info)
{
	m_info = info;

	m_device->setImageID("icon-m-content-bluetooth"); /* TODO */
	m_device->setTitle(m_info.isValid()
			   ? m_info.name()
			   : tr("No device selected"));
	m_device->setSubtitle(m_info.isValid()
			      ? m_info.address().toString()
			      : "00:00:00:00:00:00");
	setContentValidity(m_info.isValid());
	updateSize();
}

void BtPage::updateSize(void)
{
	QString name = m_info.name();

	quint32 payloadLength = 
		13 + /* length, bdaddr, cod */
		(name != "" ? (2 + name.toUtf8().length()) : 0);

	quint32 ndefLength = 
		1 + /* NDEF header */
		1 + /* type length */
		(payloadLength < 256 ? 1 : 4) + /* payload length */
		32 + /* type (application/vnd.bluetooth.ep.oob) */
		payloadLength; /* payload */

	setContentSize(ndefLength);
}
