/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BookmarkSelectionPageListModel.h"

#include <QStringList>
#include <MDebug>

BookmarkSelectionPageListModel::
BookmarkSelectionPageListModel(QObject *parent)
	: MAbstractItemModel(parent),
	  m_bookmarks()
{
	setGrouped(false);

	/* TODO: sparql fetch, asynchronous! and construct model data
	   out of the contents. */
}

int BookmarkSelectionPageListModel::groupCount(void) const
{
	return 1;
}

int BookmarkSelectionPageListModel::rowCountInGroup(int group) const
{
	(void) group;
	return m_bookmarks.length();
}

QString BookmarkSelectionPageListModel::groupTitle(int group) const
{
	(void) group;
	return "";
}

QVariant BookmarkSelectionPageListModel::itemData(int row, 
						  int group, 
						  int role) const
{
	(void) group;
	(void) role;

	QStringList parameters;
	QNdefMessage msg = m_bookmarks[row];

	parameters << "No title yet";
	parameters << "No subtitle yet";
	parameters << "No icon yet";

	return qVariantFromValue(parameters);
}

QNdefMessage 
BookmarkSelectionPageListModel::bookmark(const QModelIndex &index) const
{
	return m_bookmarks[index.row()];
}
