/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtDetailsPage.h"
#include "LabeledTextEdit.h"
#include <MAction>
#include <MLabel>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <QBluetoothAddress>
#include <QRegExpValidator>
#include <QRegExp>

#include <MDebug>

#define HEX "[0-9A-Fa-f]"
#define COL ":"

BtDetailsPage::BtDetailsPage(const QBluetoothDeviceInfo &info, 
			     QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_info(info),
	  m_cancelAction(0),
	  m_acceptAction(0),
	  m_name(0),
	  m_addr(0),
	  m_nameValidity(true),
	  m_addrValidity(true),
	  m_classValidity(true),
	  m_bdaddrRegexp()
{
	m_bdaddrRegexp = QRegExp(HEX HEX COL 
				 HEX HEX COL 
				 HEX HEX COL 
				 HEX HEX COL 
				 HEX HEX COL 
				 HEX HEX);
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

BtDetailsPage::~BtDetailsPage(void)
{
}

void BtDetailsPage::createContent(void)
{
	setPannable(false);

	m_cancelAction = new MAction(tr("Cancel"), this);
	m_cancelAction->setLocation(MAction::ToolBarLocation);
	connect(m_cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(m_cancelAction);

	m_acceptAction = new MAction(tr("Done"), this);
	m_acceptAction->setLocation(MAction::ToolBarLocation);
	connect(m_acceptAction, SIGNAL(triggered()),
		this, SLOT(accept()));
	addAction(m_acceptAction);

	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Vertical);
	layout->setSizePolicy(QSizePolicy::Preferred, 
			      QSizePolicy::Minimum);

	MLabel *label = new MLabel(tr("<big>Edit device data</big>"));
	label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	label->setAlignment(Qt::AlignCenter);
	layout->addItem(label);
	layout->setAlignment(label, Qt::AlignCenter);

	m_name = new LabeledTextEdit(MTextEditModel::SingleLine,
				     tr("Device name"),
				     tr("Enter device name"));
	m_name->textEdit()->setText(m_info.name());
	m_name->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout->addItem(m_name);
	layout->setAlignment(m_name, Qt::AlignLeft);
	connect(m_name->textEdit(), SIGNAL(textChanged(void)),
		this, SLOT(nameChanged(void)));

	QRegExpValidator *addrValidator =
		new QRegExpValidator(m_bdaddrRegexp, this);

	m_addr = new LabeledTextEdit(MTextEditModel::SingleLine,
				     tr("Device address"),
				     tr("Enter device address"));

	char addrstr[18] = { 0 };
	quint64 bdaddr = m_info.address().toUInt64();
	for (int i = 0; i < 6; i++) {
		static const char *hex = "0123456789ABCDEF";
		addrstr[3*i + 0] = hex[(bdaddr >> (44 - i*8)) & 0xf];
		addrstr[3*i + 1] = hex[(bdaddr >> (40 - i*8)) & 0xf];
		addrstr[3*i + 2] = (i < 5) ? ':' : '\0';
	}
	m_addr->textEdit()->setText(addrstr); 
	m_addr->textEdit()->setValidator(addrValidator);
	m_addr->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	layout->addItem(m_addr);
	layout->setAlignment(m_addr, Qt::AlignLeft);
	connect(m_addr->textEdit(), SIGNAL(textChanged(void)),
		this, SLOT(addressChanged(void)));

	centralWidget()->setLayout(layout);

}

void BtDetailsPage::updateUI(void)
{
	if (m_nameValidity == false || 
	    m_addrValidity == false ||
	    m_classValidity == false) {
		m_acceptAction->setEnabled(false);
	} else {
		m_acceptAction->setEnabled(true);
	}
}

void BtDetailsPage::nameChanged(void)
{
	/* Anything goes, actually, so nothing to update */
}

void BtDetailsPage::addressChanged(void)
{
	/* There's some funny business going on with regexp validators... 
	   works ok for limiting input but doesn't validate properly... */

	if (m_addr->textEdit()->hasAcceptableInput() == true) {
		setAddressValidity(true);
	} else {
		setAddressValidity(false);
	}
}

void BtDetailsPage::classChanged(void)
{
	/* TODO */
}

void BtDetailsPage::setAddressValidity(bool valid)
{
	m_addrValidity = valid;
	updateUI();
}

void BtDetailsPage::setClassValidity(bool valid)
{
	m_classValidity = valid;
	updateUI();
}

void BtDetailsPage::accept(void)
{
	quint32 cod =
		((m_info.serviceClasses() & 0x7ff) << 13) | 
		((m_info.majorDeviceClass() & 0x1f) << 8) |
		((m_info.minorDeviceClass() & 0x3f) << 2);

	QBluetoothAddress addr(m_addr->textEdit()->text());

	QBluetoothDeviceInfo ret(addr,
				 m_name->textEdit()->text(),
				 cod);
	Q_EMIT(finished(ret));
	dismiss();
}
