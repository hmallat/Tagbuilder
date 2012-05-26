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

/* Show a list of entries; but if there are no entries, show an
   explanatory label. */ 

#ifndef _LABEL_OR_LIST_H_
#define _LABEL_OR_LIST_H_

#include <QGraphicsLayout>
#include <QAbstractItemModel>
#include <MAbstractCellCreator>
#include <QList>

class MWidgetController;
class MLabel;
class MButton;
class MList;
class MPannableViewport;
class QModelIndex;
class QGraphicsGridLayout;
class QItemSelectionModel;

class LabelOrList : public QObject, public QGraphicsLayout
{

	Q_OBJECT;

	Q_INTERFACES(QGraphicsLayout);

public:

	LabelOrList(QAbstractItemModel *listModel,
		    MAbstractCellCreator<MWidgetController> *(*getCreator)(void),
		    const QString &label,
		    bool showGroups,
		    bool multiSelect,
		    QList<MButton *> buttons = QList<MButton *>(),
		    QGraphicsLayoutItem *parent = 0);

	virtual ~LabelOrList(void);

	virtual void setGeometry(const QRectF &rect);

	virtual QSizeF sizeHint(Qt::SizeHint which, 
				const QSizeF &constraint = QSizeF()) const;

	virtual int count(void) const;

	virtual QGraphicsLayoutItem *itemAt(int i) const;

	virtual void removeAt(int index);

	void scrollTo(const QModelIndex &index);

	QItemSelectionModel *selectionModel(void);

	void selectItem(const QModelIndex &index);

	void setLabel(const QString label);

Q_SIGNALS:

	void itemClicked(const QModelIndex &);

	void itemLongTapped(const QModelIndex &, const QPointF &);
		
private Q_SLOTS:

	void setDisplay(void);

private:

	Q_DISABLE_COPY(LabelOrList);

	bool m_grouped;

	QAbstractItemModel *m_model;

	MAbstractCellCreator<MWidgetController> *(*m_getCreator)(void);

	QString m_text;

	QGraphicsGridLayout *m_layout;

	int m_previousRowCount;

	MList *m_list;

	MPannableViewport *m_view;

	MLabel *m_label;

	MWidgetController *m_labelBox;

};

#endif /* _LABEL_OR_LIST_H_ */
