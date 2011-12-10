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

class MContentItem;
class BluezSupplicant;

QTM_USE_NAMESPACE;

class BtPage : public CreateEditPage
{

	Q_OBJECT;

public:

	BtPage(BluezSupplicant *bluez,
	       int tag = -1, 
	       QGraphicsItem *parent = 0);

	~BtPage(void);

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

private:

	Q_DISABLE_COPY(BtPage);

	void updateSize(void);

	void noBluetoothAlert(void);

	MContentItem *m_device;

	QBluetoothDeviceInfo m_info;

	BluezSupplicant *m_bluez;

};

#endif /* _BT_PAGE_H_ */
