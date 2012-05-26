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

#ifndef _SELECTION_PAGE_H_
#define _SELECTION_PAGE_H_

#include <MApplicationPage>

#include <QModelIndex>

class ViewHeader;
class LabelOrList;
class MAbstractItemModel;
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

protected Q_SLOTS:

	void setBusy(void);

	void clearBusy(void);

protected:

	void selectAll(MAbstractItemModel *itemModel);

	void createCommonContent(QAbstractItemModel *itemModel,
				 MAbstractCellCreator<MWidgetController> *(*getCreator)(void),
				 const QString label,
				 const QString title, 
				 bool groupedList,
				 bool multiSelect);

	LabelOrList *m_list;

private:

	Q_DISABLE_COPY(SelectionPage);

	ViewHeader *m_header;

};

#endif /* _SELECTION_PAGE_H_ */
