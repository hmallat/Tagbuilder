/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BOOKMARK_SELECTION_PAGE_LIST_MODEL_H_
#define _BOOKMARK_SELECTION_PAGE_LIST_MODEL_H_

#include <MAbstractItemModel>
#include <QNdefMessage>

QTM_USE_NAMESPACE;

class BookmarkSelectionPageListModel : public MAbstractItemModel
{

	Q_OBJECT;

public:

	BookmarkSelectionPageListModel(QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	QNdefMessage bookmark(const QModelIndex &index) const;

protected:

	QList<QNdefMessage> m_bookmarks;

};

#endif /* _BOOKMARK_SELECTION_PAGE_LIST_MODEL_H_ */
