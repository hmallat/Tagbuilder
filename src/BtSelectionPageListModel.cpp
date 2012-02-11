/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtSelectionPageListModel.h"

#include <QStringList>
#include <QBluetoothAddress>

QTM_USE_NAMESPACE;

BtSelectionPageListModel::BtSelectionPageListModel(BluezSupplicant *bluez,
						   QObject *parent)
	: MAbstractItemModel(parent),
	  m_bluez(bluez)
{
}

int BtSelectionPageListModel::groupCount(void) const
{
	return 0;
}

int BtSelectionPageListModel::rowCountInGroup(int group) const
{
	(void) group;
	return m_device_ids.length();
}

QString BtSelectionPageListModel::groupTitle(int group) const
{
	(void) group;
	return "";
}

QVariant BtSelectionPageListModel::itemData(int row,
					    int group,
					    int role) const
{
	(void) role;
	(void) group;

	QString id = m_device_ids[row];
	QBluetoothDeviceInfo info = m_devices[id];
	QStringList parameters;
	parameters 
		<< info.name() 
		<< info.address().toString() 
		<< "icon-m-content-bluetooth" /* TODO */;
	return qVariantFromValue(parameters);
}

const QString BtSelectionPageListModel::name(const QModelIndex &index) const
{
	QString id = m_device_ids[index.row()];
	QBluetoothDeviceInfo info = m_devices[id];
	return info.name();
}

const QString BtSelectionPageListModel::icon(const QModelIndex &index) const
{
	(void)index;
	return "icon-m-content-bluetooth" /* TODO */;
}

QBluetoothDeviceInfo BtSelectionPageListModel::device(const QModelIndex &index) const
{
	QString id = m_device_ids[index.row()];
	return m_devices[id];
}
