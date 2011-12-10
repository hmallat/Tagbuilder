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
#include <QAbstractListModel>
#include <MAbstractCellCreator>

class MContentItem;
class MLabel;
class MList;
class MPannableViewport;
class QGraphicsGridLayout;

class LabelOrList : public QObject, public QGraphicsLayout
{

	Q_OBJECT;

	Q_INTERFACES(QGraphicsLayout);

public:

	LabelOrList(QAbstractListModel *listModel,
		    MAbstractCellCreator<MContentItem> *(*getCreator)(void),
		    const QString &label = QString(),
		    QGraphicsLayoutItem *parent = 0);

	virtual ~LabelOrList(void);

	virtual void setGeometry(const QRectF &rect);

	virtual QSizeF sizeHint(Qt::SizeHint which, 
				const QSizeF &constraint = QSizeF()) const;

	virtual int count(void) const;

	virtual QGraphicsLayoutItem *itemAt(int i) const;

	virtual void removeAt(int index);

Q_SIGNALS:

	void itemClicked(const QModelIndex &);

	void itemLongTapped(const QModelIndex &, const QPointF &);
		
private Q_SLOTS:

	void setDisplay(void);

private:

	Q_DISABLE_COPY(LabelOrList);

	QAbstractListModel *m_model;

	MAbstractCellCreator<MContentItem> *(*m_getCreator)(void);

	QString m_label;

	QGraphicsGridLayout *m_layout;

	int m_previousRowCount;

};

#endif /* _LABEL_OR_LIST_H_ */
