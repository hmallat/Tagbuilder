/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

/* Show a list of entries; but if there are no entries, show an
   explanatory label. */ 

#ifndef _LABEL_OR_LIST_H_
#define _LABEL_OR_LIST_H_

#include <QGraphicsLayout>
#include <QAbstractItemModel>
#include <MAbstractCellCreator>

class MWidgetController;
class MLabel;
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

};

#endif /* _LABEL_OR_LIST_H_ */
