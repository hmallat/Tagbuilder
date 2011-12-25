/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BT_PAGE_H
#define _BT_PAGE_H_

#include "CreateEditPage.h"

#include <QBluetoothDeviceInfo>
#include <QNdefMessage>

class MButton;
class LabeledTextEdit;
class BluezSupplicant;

QTM_USE_NAMESPACE;

class BtPage : public CreateEditPage
{

	Q_OBJECT;

public:

	BtPage(BluezSupplicant *bluez,
	       int tag = TagStorage::NULL_TAG, 
	       QGraphicsItem *parent = 0);

	virtual ~BtPage(void);

protected:

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void choosePhoneBT(void);

	void chooseExistingBT(void);

	void chooseScannedBT(void);

	void setDevice(const QBluetoothDeviceInfo info);

	void deviceNameChanged(void);

	void deviceAddressChanged(void);

	void deviceClassChanged(void);

private:

	Q_DISABLE_COPY(BtPage);

	void updateDevice(void);

	void noBluetoothAlert(void);

	void setDeviceAddressValidity(bool valid);

	void setDeviceClassValidity(bool valid);

	BluezSupplicant *m_bluez;

	QBluetoothDeviceInfo m_info;

	QNdefMessage m_message;

	QRegExp m_bdaddrRegexp;

	QRegExp m_codRegexp;

	LabeledTextEdit *m_name;

	LabeledTextEdit *m_addr;

	LabeledTextEdit *m_class;

	bool m_deviceNameValidity;

	bool m_deviceAddrValidity;

	bool m_deviceClassValidity;

};

#endif /* _BT_PAGE_H_ */
