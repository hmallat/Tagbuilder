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
#include <QBluetoothDeviceInfo>

class LabeledTextEdit;
class MContentItem;
class MButton;
class BluezSupplicant;

QTM_USE_NAMESPACE;

class BtPage : public MApplicationPage
{

	Q_OBJECT;

public:

	BtPage(BluezSupplicant *bluez,
	       int tag = -1, 
	       QGraphicsItem *parent = 0);

	~BtPage(void);

	virtual void createContent(void);

private Q_SLOTS:

	void nameChanged(void);

	void storeTag(void);

	void choosePhoneBT(void);

	void chooseExistingBT(void);

	void chooseScannedBT(void);

	void setDevice(const QBluetoothDeviceInfo info);

private:

	Q_DISABLE_COPY(BtPage);

	int m_tag;

	LabeledTextEdit *m_name;

	MContentItem *m_device;

	MAction *m_cancelAction;

	MAction *m_storeAction;

	QBluetoothDeviceInfo m_info;

	BluezSupplicant *m_bluez;

};

#endif /* _Bt_PAGE_H_ */
