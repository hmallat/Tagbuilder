/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

/* BT page needs to handle both standalone BT records as well as
   static handover tags that contain a handover record followed by a
   BT record. This is implemented in a slightly hacky manner
   currently. */ 

/* TODO: check what happens if BT is off */

#include "BtPage.h"
#include "BtNdefRecord.h"
#include "BluezSupplicant.h"
#include "BtSelectionPage.h"
#include "LabeledTextEdit.h"
#include "Util.h"

#include <MLabel>
#include <MAction>
#include <MMessageBox>
#include <QGraphicsLinearLayout>
#include <QBluetoothDeviceInfo>
#include <QBluetoothLocalDevice>
#include <QRegExpValidator>
#include <QRegExp>

#include <MDebug>

#define HEX "[0-9A-Fa-f]"
#define COL ":"

/* NB: looks like Qt hostinfo doesn't return the class of
   device. Could ask from BlueZ, but that would be just as unportable
   as hardcoding it (no BlueZ in Symbian!), so here goes. */
#define N9_COD 0x5a020c

/* NB: looks like Qt doesn't support retrieving a list of already
   known devices. So implement that using BlueZ D-Bus interface. That
   cannot be hardcoded... */

static quint32 _cod(const QBluetoothDeviceInfo &info)
{
	return
		((info.serviceClasses() & 0x7ff) << 13) | 
		((info.majorDeviceClass() & 0x1f) << 8) |
		((info.minorDeviceClass() & 0x3f) << 2);
}

static QString _bdaddr2str(const QBluetoothAddress address)
{
	static const char *hex = "0123456789ABCDEF";
	char addrstr[18] = { 0 };
	quint64 bdaddr = address.toUInt64();
	for (int i = 0; i < 6; i++) {
		addrstr[3*i + 0] = hex[(bdaddr >> (44 - i*8)) & 0xf];
		addrstr[3*i + 1] = hex[(bdaddr >> (40 - i*8)) & 0xf];
		addrstr[3*i + 2] = (i < 5) ? ':' : '\0';
	}
	return QString(addrstr);
}

static QString _cod2str(quint32 cod)
{
	static const char *hex = "0123456789ABCDEF";
	char codstr[7] = { 0 };
	for (int i = 0; i < 3; i++) {
		codstr[2*i + 0] = hex[(cod >> (20 - i*8)) & 0xf];
		codstr[2*i + 1] = hex[(cod >> (16 - i*8)) & 0xf];
	}
	return QString(codstr);
}

BtPage::BtPage(BluezSupplicant *bluez, int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_bluez(bluez),
	  m_info(),
	  m_name(0),
	  m_addr(0),
	  m_class(0),
	  m_deviceNameValidity(true),
	  m_deviceAddrValidity(true),
	  m_deviceClassValidity(true)
{
	m_bdaddrRegexp = QRegExp(HEX HEX COL
				 HEX HEX COL
				 HEX HEX COL
				 HEX HEX COL
				 HEX HEX COL
				 HEX HEX);
	m_codRegexp = QRegExp(HEX HEX 
			      HEX HEX 
			      HEX HEX);
}

BtPage::~BtPage(void)
{
}

void BtPage::createPageSpecificActions(void)
{
	MAction *chooseThisAction = new MAction(tr("This phone"),
						this);
	chooseThisAction->setLocation(MAction::ApplicationMenuLocation);
	connect(chooseThisAction, SIGNAL(triggered()),
		this, SLOT(choosePhoneBT()));
	addAction(chooseThisAction);
	
	MAction *pickAction = new MAction(tr("Known device..."),
					  this);
	pickAction->setLocation(MAction::ApplicationMenuLocation);
	connect(pickAction, SIGNAL(triggered()),
		this, SLOT(chooseExistingBT()));
	addAction(pickAction);

	MAction *scanAction = new MAction(tr("Scan for a device..."),
					  this);
	scanAction->setLocation(MAction::ApplicationMenuLocation);
	connect(scanAction, SIGNAL(triggered()),
		this, SLOT(chooseScannedBT()));
	addAction(scanAction);

}

void BtPage::createPageSpecificContent(void)
{
	m_name = new LabeledTextEdit(tr("Ok"),
				     LabeledTextEdit::SingleLineEditAndLabel);
	m_name->setLabel(tr("Device name"));
	m_name->setPrompt(tr("Enter device name"));
	m_name->setContents(m_info.name());
	m_name->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout()->addItem(m_name);
	layout()->setAlignment(m_name, Qt::AlignLeft);
	connect(m_name, SIGNAL(contentsChanged(void)),
		this, SLOT(deviceNameChanged(void)));

	QRegExpValidator *addrValidator =
		new QRegExpValidator(m_bdaddrRegexp, this);
	m_addr = new LabeledTextEdit(tr("Ok"),
				     LabeledTextEdit::SingleLineEditAndLabel);
	m_addr->setLabel(tr("Device address"));
	m_addr->setPrompt(tr("Enter device address"));
	m_addr->setContents(_bdaddr2str(m_info.address()));
	m_addr->setValidator(addrValidator, 17);
	m_addr->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout()->addItem(m_addr);
	layout()->setAlignment(m_addr, Qt::AlignLeft);
	connect(m_addr, SIGNAL(contentsChanged(void)),
		this, SLOT(deviceAddressChanged(void)));

	QRegExpValidator *codValidator =
		new QRegExpValidator(m_codRegexp, this);
	m_class = new LabeledTextEdit(tr("Ok"),
				      LabeledTextEdit::SingleLineEditAndLabel);
	m_class->setLabel(tr("Device class"));
	m_class->setPrompt(tr("Enter device class"));
	m_class->setContents(_cod2str(_cod(m_info)));
	m_class->setValidator(codValidator, 6);
	m_class->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout()->addItem(m_class);
	layout()->setAlignment(m_class, Qt::AlignLeft);
	connect(m_class, SIGNAL(contentsChanged(void)),
		this, SLOT(deviceClassChanged(void)));
}

void BtPage::setupNewData(void) 
{
	BtNdefRecord bt;
	m_message.clear();
	m_message << bt;

	setDevice(QBluetoothDeviceInfo(bt.address(), 
				       bt.name(), 
				       bt.classOfDevice()));
}

bool BtPage::setupData(const QNdefMessage message)
{
	m_message = message;
	BtNdefRecord bt(m_message[m_message.length() - 1]);
	if (bt.isValid() == false) {
		return false;
	}

	setDevice(QBluetoothDeviceInfo(bt.address(), 
				       bt.name(), 
				       bt.classOfDevice()));
	if (isTemporary() == true) {
		if (bt.name() != "") {
			setDefaultName(bt.name());
		} else {
			setDefaultName(_bdaddr2str(bt.address()));
		}
	}
	return true;
}

QNdefMessage BtPage::prepareDataForStorage(void)
{
	BtNdefRecord bt(m_message[m_message.length() - 1]);
	bt.setAddress(m_info.address());
	bt.setClassOfDevice(_cod(m_info));
	bt.setName(m_info.name());
	m_message[m_message.length() - 1] = bt;

	return m_message;
}

void BtPage::noBluetoothAlert(void)
{
	MMessageBox *box = new MMessageBox(tr("Bluetooth is not available. "));
	box->appear(MSceneWindow::DestroyWhenDismissed);
}

void BtPage::choosePhoneBT(void)
{
	QList<QBluetoothHostInfo> locals = QBluetoothLocalDevice::allDevices();
	if (locals.length() > 0) {
		QBluetoothDeviceInfo info(locals[0].getAddress(),
					  locals[0].getName(),
					  N9_COD);
		setDeviceThroughAction(info);
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
		this, SLOT(setDeviceThroughAction(const QBluetoothDeviceInfo)));
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
		this, SLOT(setDeviceThroughAction(const QBluetoothDeviceInfo)));
}

void BtPage::setDevice(const QBluetoothDeviceInfo info)
{
	m_name->setContents(info.isValid()
			    ? info.name()
			    : "");
	
	m_addr->setContents(_bdaddr2str(info.address()));

	m_class->setContents(info.isValid()
			     ? _cod2str(_cod(info))
			     : _cod2str(0));

	updateDevice();
}

void BtPage::setDeviceThroughAction(const QBluetoothDeviceInfo info)
{
	setDevice(info);

	if (info.isValid() == true && info.name() != "") {
		setDefaultName(info.name());
	} else {
		setDefaultName(_bdaddr2str(info.address()));
	}
}

void BtPage::updateDevice(void)
{
	bool dummy;
	QBluetoothAddress addr(m_addr->contents());
	QString name(m_name->contents());
	quint32 cod = m_class->contents().toUInt(&dummy, 16);
	
	m_info = QBluetoothDeviceInfo(addr,
				      name,
				      cod);
	prepareDataForStorage();
#ifdef LABEL_SIZE
	setContentSize(Util::messageLength(m_message));
#endif
}

void BtPage::deviceNameChanged(void)
{
	/* Anything goes, actually */
	updateDevice();
}

void BtPage::deviceAddressChanged(void)
{
	if (m_addr->hasAcceptableInput() == true) {
		setDeviceAddressValidity(true);
		updateDevice();
	} else {
		setDeviceAddressValidity(false);
	}
}

void BtPage::deviceClassChanged(void)
{
	if (m_class->hasAcceptableInput() == true) {
		setDeviceClassValidity(true);
		updateDevice();
	} else {
		setDeviceClassValidity(false);
	}
}

void BtPage::setDeviceAddressValidity(bool valid)
{
	m_deviceAddrValidity = valid;
	setContentValidity(m_deviceNameValidity == true &&
			   m_deviceAddrValidity == true &&
			   m_deviceClassValidity == true);
}

void BtPage::setDeviceClassValidity(bool valid)
{
	m_deviceClassValidity = valid;
	setContentValidity(m_deviceNameValidity == true &&
			   m_deviceAddrValidity == true &&
			   m_deviceClassValidity == true);
}

