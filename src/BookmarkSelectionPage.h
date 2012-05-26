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
