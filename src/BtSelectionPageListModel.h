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

QTM_USE_NAMESPACE;

class BtSelectionPageListModel : public QAbstractListModel
{

	Q_OBJECT;

public:

	BtSelectionPageListModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	const QString name(const QModelIndex &index) const;

	const QString icon(const QModelIndex &index) const;

	virtual QBluetoothDeviceInfo device(const QModelIndex &index) const = 0;

private:

	virtual int modelRowCount(const QModelIndex &parent = QModelIndex()) const = 0;

	virtual QVariant indexData(const QModelIndex &index, int role) const = 0;

	virtual QString indexName(const QModelIndex &index) const = 0;

	virtual QString indexIcon(const QModelIndex &index) const = 0;

};

#endif /* _BT_SELECTION_PAGE_LIST_MODEL_H_ */
