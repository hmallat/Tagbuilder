/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BOOKMARK_SELECTION_PAGE_H_
#define _BOOKMARK_SELECTION_PAGE_H_

#include "SelectionPage.h"

#include <QNdefMessage>

QTM_USE_NAMESPACE;

class BookmarkSelectionPageListModel;

class BookmarkSelectionPage : public SelectionPage
{
	Q_OBJECT;

public:

	BookmarkSelectionPage(QGraphicsItem *parent = 0);

	virtual ~BookmarkSelectionPage(void);

        virtual void createContent(void);

signals:

	void selected(const QNdefMessage which);

private Q_SLOTS:
	
	void itemsReady(void);

	void bookmarkSelected(const QModelIndex &which);

private:

	Q_DISABLE_COPY(BookmarkSelectionPage);

	BookmarkSelectionPageListModel *m_model;

};

#endif /* _BOOKMARK_SELECTION_PAGE_H_ */
