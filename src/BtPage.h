/*

    NFC Tag Builder for Nokia N9
    Copyright (C) 2011,2012  Hannu Mallat <hmallat@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

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

	virtual void createPageSpecificActions(void);

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void choosePhoneBT(void);

	void chooseExistingBT(void);

	void chooseScannedBT(void);

	void deviceNameChanged(void);

	void deviceAddressChanged(void);

	void deviceClassChanged(void);

	void setDeviceThroughAction(const QBluetoothDeviceInfo info);

private:

	Q_DISABLE_COPY(BtPage);

	void setDevice(const QBluetoothDeviceInfo info);

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
