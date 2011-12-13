/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BT_DETAILS_PAGE_H_
#define _BT_DETAILS_PAGE_H_

#include <MApplicationPage>
#include <QBluetoothDeviceInfo>
#include <QRegExp>

class LabeledTextEdit;
class MAction;

QTM_USE_NAMESPACE;

class BtDetailsPage : public MApplicationPage
{

	Q_OBJECT;

public:

	BtDetailsPage(const QBluetoothDeviceInfo &info, 
		      QGraphicsItem *parent = 0);

	~BtDetailsPage(void);

	virtual void createContent(void);

Q_SIGNALS:

	void finished(const QBluetoothDeviceInfo &);

private Q_SLOTS:

	void nameChanged(void);

	void addressChanged(void);

	void classChanged(void);

	void accept(void);

private:

	void updateUI(void);

	void setAddressValidity(bool valid);

	void setClassValidity(bool valid);

	QBluetoothDeviceInfo m_info;

	MAction *m_cancelAction;

	MAction *m_acceptAction;

	LabeledTextEdit *m_name;

	LabeledTextEdit *m_addr;

	bool m_nameValidity;

	bool m_addrValidity;

	bool m_classValidity;

	QRegExp m_bdaddrRegexp;

};

#endif /* _BT_DETAILS_PAGE_H_ */
