/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BT_PAGE_H
#define _BT_PAGE_H_

#include <MApplicationPage>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>

class LabeledTextEdit;
class MContentItem;

QTM_USE_NAMESPACE;

class BtPage : public MApplicationPage
{

	Q_OBJECT;

public:

	BtPage(int tag = -1, QGraphicsItem *parent = 0);

	~BtPage(void);

	virtual void createContent(void);

private Q_SLOTS:

	void nameChanged(void);

	void storeTag(void);

	void choosePhoneBT(void);

	void chooseExistingBT(void);

	void chooseScannedBT(void);

	void deviceDiscovered(const QBluetoothDeviceInfo &info);
	
	void discoveryError(QBluetoothDeviceDiscoveryAgent::Error err);

	void discoveryFinished(void);

	void setDevice(const QBluetoothDeviceInfo &info);

private:

	Q_DISABLE_COPY(BtPage);

	int m_tag;

	LabeledTextEdit *m_name;

	MContentItem *m_device;

	MAction *m_cancelAction;

	MAction *m_storeAction;

	QBluetoothDeviceDiscoveryAgent *m_discovery;

	QBluetoothDeviceInfo m_info;

};

#endif /* _Bt_PAGE_H_ */
