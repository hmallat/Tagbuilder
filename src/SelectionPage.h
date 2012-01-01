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

#include <QModelIndex>

class LabelOrList;
class QAbstractItemModel;
class MWidget;
template <class C> class MAbstractCellCreator;

class SelectionPage : public MApplicationPage
{
	Q_OBJECT;

public:

	SelectionPage(QGraphicsItem *parent = 0);

	virtual ~SelectionPage(void);

        virtual void createContent(void);

Q_SIGNALS:

	void done(void);

	void clicked(const QModelIndex which);

protected:

	void createCommonContent(QAbstractItemModel *itemModel,
				 MAbstractCellCreator<MWidgetController> *(*getCreator)(void),
				 const QString label,
				 const QString title, 
				 bool groupedList,
				 bool multiSelect);

	LabelOrList *m_list;

private:

	Q_DISABLE_COPY(SelectionPage);

};

#endif /* _SELECTION_PAGE_H_ */
