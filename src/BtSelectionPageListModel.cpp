/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtSelectionPageListModel.h"

BtSelectionPageListModel::BtSelectionPageListModel(QObject *parent)
	: QAbstractListModel(parent)
{
}

int BtSelectionPageListModel::rowCount(const QModelIndex &parent) const
{
	return modelRowCount(parent);
}

QVariant BtSelectionPageListModel::data(const QModelIndex &index, 
					int role) const
{
	return indexData(index, role);
}

const QString BtSelectionPageListModel::name(const QModelIndex &index) const
{
	return indexName(index);
}

const QString BtSelectionPageListModel::icon(const QModelIndex &index) const
{
	return indexIcon(index);
}
