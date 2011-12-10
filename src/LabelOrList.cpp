/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "LabelOrList.h"

#include <MLabel>
#include <MList>
#include <MPannableViewport>
#include <QGraphicsGridLayout>

LabelOrList::LabelOrList(QAbstractListModel *listModel,
			 MAbstractCellCreator<MContentItem> *(*getCreator)(void),
			 const QString &label,
			 QGraphicsLayoutItem *parent)
	: QGraphicsLayout(parent),
	  m_model(listModel),
	  m_getCreator(getCreator),
	  m_label(label),
	  m_layout(0),
	  m_previousRowCount(-1)
{

	m_layout = new QGraphicsGridLayout(this);
	m_layout->setSizePolicy(QSizePolicy::Minimum, 
				QSizePolicy::Minimum);

	connect(m_model, SIGNAL(layoutChanged(void)),
		this, SLOT(setDisplay(void)));

	setDisplay();
}

LabelOrList::~LabelOrList(void)
{
}

void LabelOrList::setGeometry(const QRectF &rect)
{
	m_layout->setGeometry(rect);
}

QSizeF LabelOrList::sizeHint(Qt::SizeHint which, 
			     const QSizeF &constraint) const
{
	return m_layout->sizeHint(which, constraint);
}

int LabelOrList::count(void) const
{
	return 1;
}

QGraphicsLayoutItem *LabelOrList::itemAt(int i) const
{
	return (i == 0) ? m_layout : 0;
}

void LabelOrList::removeAt(int index)
{
	if (index != 0) {
		return;
	}

	m_layout->setParentLayoutItem(0);
	invalidate();
}

void LabelOrList::setDisplay(void)
{
	int rowCount = m_model->rowCount();

	if (rowCount != 0 && m_previousRowCount < 1) {

		/* Need to show the list, not the label */

		QGraphicsLayoutItem *old = m_layout->itemAt(0);
		if (old != 0) {
			m_layout->removeAt(0);
			delete old;
		}

		MList *list = new MList();
		list->setItemModel(m_model);
		list->setCellCreator(m_getCreator());
		connect(list, SIGNAL(itemClicked(const QModelIndex &)),
			this, SIGNAL(itemClicked(const QModelIndex &)));
		connect(list, SIGNAL(itemLongTapped(const QModelIndex &,
						    const QPointF &)),
			this, SIGNAL(itemLongTapped(const QModelIndex &,
						    const QPointF &)));
	
		MPannableViewport *view = new MPannableViewport();
		view->setSizePolicy(QSizePolicy::Minimum, 
				    QSizePolicy::Minimum);
		view->setPanDirection(Qt::Vertical);
		view->setMinimumSize(100, 100);
		view->setWidget(list);

		m_layout->addItem(view, 0, 0);

	} else if (rowCount == 0 && m_previousRowCount != 0) {

		/* Need to show the label, not the list */

		QGraphicsLayoutItem *old = m_layout->itemAt(0);
		if (old != 0) {
			m_layout->removeAt(0);
			delete old;
		}

		MLabel *label = new MLabel(m_label);
		label->setAlignment(Qt::AlignCenter);
		label->setWordWrap(true);
		label->setSizePolicy(QSizePolicy::Ignored,
				     QSizePolicy::Ignored);
		m_layout->addItem(label, 0, 0);

	}

	m_previousRowCount = rowCount;
}
