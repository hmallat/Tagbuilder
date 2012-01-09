/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BT_SELECTION_PAGE_LIST_MODEL_H_
#define _BT_SELECTION_PAGE_LIST_MODEL_H_

#include <QAbstractListModel>
#include <QBluetoothDeviceInfo>

#include "BluezSupplicant.h"

QTM_USE_NAMESPACE;

class BtSelectionPageListModel : public QAbstractListModel
{

	Q_OBJECT;

public:

	BtSelectionPageListModel(BluezSupplicant *bluez,
				 QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	const QString name(const QModelIndex &index) const;

	const QString icon(const QModelIndex &index) const;

	QBluetoothDeviceInfo device(const QModelIndex &index) const;

protected:

	BluezSupplicant *m_bluez;

	QList<QString> m_device_ids;

	QMap<QString, QBluetoothDeviceInfo> m_devices;

};

#endif /* _BT_SELECTION_PAGE_LIST_MODEL_H_ */
