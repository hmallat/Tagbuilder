/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BT_SELECTION_PAGE_EXISTING_LIST_MODEL_H_
#define _BT_SELECTION_PAGE_EXISTING_LIST_MODEL_H_

#include "BtSelectionPageListModel.h"
#include <QBluetoothDeviceInfo>
#include <QDBusObjectPath>
#include <QMap>
#include <QList>

QTM_USE_NAMESPACE;

class BluezSupplicant;
class QDBusObjectPath;

class BtSelectionPageExistingListModel : public BtSelectionPageListModel
{

	Q_OBJECT;

public:

	BtSelectionPageExistingListModel(BluezSupplicant *bluez,
					 QObject *parent = 0);

	virtual QBluetoothDeviceInfo device(const QModelIndex &index) const;

private slots:

	void deviceCreated(QDBusObjectPath which);

	void deviceRemoved(QDBusObjectPath which);

	void deviceUpdated(QDBusObjectPath which);

private:

	virtual int modelRowCount(const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant indexData(const QModelIndex &index, int role) const;

	virtual QString indexName(const QModelIndex &index) const;

	virtual QString indexIcon(const QModelIndex &index) const;

	BluezSupplicant *m_bluez;

	QList<QDBusObjectPath> m_device_ids;

	QMap<QDBusObjectPath, QBluetoothDeviceInfo> m_devices;

};

#endif /* _BT_SELECTION_PAGE_EXISTING_LIST_MODEL_H_ */
