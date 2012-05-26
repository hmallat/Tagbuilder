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
