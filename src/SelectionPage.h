/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _SELECTION_PAGE_H_
#define _SELECTION_PAGE_H_

#include <MApplicationPage>

class QModelIndex;
class LabelOrList;
class QAbstractItemModel;
class MContentItem;
template <class C> class MAbstractCellCreator;

class SelectionPage : public MApplicationPage
{
	Q_OBJECT;

public:

	SelectionPage(QGraphicsItem *parent = 0);

	~SelectionPage(void);

        virtual void createContent(void);

protected:

	void createCommonContent(QAbstractItemModel *itemModel,
				 MAbstractCellCreator<MContentItem> *(*getCreator)(void),
				 const QString label,
				 const QString title, 
				 bool groupedList);

	LabelOrList *m_list;

private:

	Q_DISABLE_COPY(SelectionPage);

};

#endif /* _SELECTION_PAGE_H_ */
