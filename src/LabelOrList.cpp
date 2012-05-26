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

#include "LabelOrList.h"

#include <MLabel>
#include <MList>
#include <MButton>
#include <MPannableViewport>
#include <QGraphicsGridLayout>
#include <QGraphicsLinearLayout>
#include <QItemSelectionModel>

#include <MDebug>

LabelOrList::LabelOrList(QAbstractItemModel *itemModel,
			 MAbstractCellCreator<MWidgetController> *(*getCreator)(void),
			 const QString &text,
			 bool showGroups, 
			 bool multiSelect,
			 QList<MButton *> buttons,
			 QGraphicsLayoutItem *parent)
	: QGraphicsLayout(parent),
	  m_grouped(showGroups),
	  m_model(itemModel),
	  m_getCreator(getCreator),
	  m_text(text),
	  m_layout(0),
	  m_previousRowCount(-1),
	  m_list(0),
	  m_view(0),
	  m_label(0),
	  m_labelBox(0)
{

	m_layout = new QGraphicsGridLayout(this);

	connect(m_model, SIGNAL(layoutChanged(void)),
		this, SLOT(setDisplay(void)));
	connect(m_model, SIGNAL(dataChanged(const QModelIndex &,
					    const QModelIndex &)),
		this, SLOT(setDisplay(void)));
	connect(m_model, SIGNAL(rowsInserted(const QModelIndex &,
					     int,
					     int)),
		this, SLOT(setDisplay(void)));
	connect(m_model, SIGNAL(rowsRemoved(const QModelIndex &,
					    int,
					    int)),
		this, SLOT(setDisplay(void)));

	m_list = new MList();
	m_list->setShowGroups(m_grouped);
	if (m_grouped == true) {
		m_list->setIndexDisplayMode(MList::Auto);
	}
	m_list->setSelectionMode(multiSelect == true
				 ? MList::MultiSelection
				 : MList::NoSelection);
	m_list->setItemModel(m_model);
	m_list->setCellCreator(m_getCreator());
	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SIGNAL(itemClicked(const QModelIndex &)));
	connect(m_list, SIGNAL(itemLongTapped(const QModelIndex &,
					      const QPointF &)),
		this, SIGNAL(itemLongTapped(const QModelIndex &,
					    const QPointF &)));
	
	m_view = new MPannableViewport();
	m_view->setSizePolicy(QSizePolicy::Minimum, 
			      QSizePolicy::Minimum);
	m_view->setPanDirection(Qt::Vertical);
	m_view->setMinimumSize(100, 100);
	m_view->setWidget(m_list);

	{
		m_labelBox = new MWidgetController();
		m_labelBox->setSizePolicy(QSizePolicy::Ignored,
					  QSizePolicy::Ignored);

		QGraphicsLinearLayout *layout = 
			new QGraphicsLinearLayout(Qt::Vertical,
						  m_labelBox);

		MLabel *topSpacer = new MLabel();
		layout->addItem(topSpacer);
		
		m_label = new MLabel(m_text);
		m_label->setAlignment(Qt::AlignCenter);
		m_label->setWordWrap(true);
#if 0
		m_label->setSizePolicy(QSizePolicy::Ignored,
				       QSizePolicy::Ignored);
#else
		m_label->setSizePolicy(QSizePolicy::Ignored,
				       QSizePolicy::Fixed);
#endif
		m_label->setStyleName("LabelOrListLabel");
		layout->addItem(m_label);
		layout->setAlignment(m_label, Qt::AlignCenter);

		for (int i = 0; i < buttons.length(); i++) {
			layout->addItem(buttons[i]);
			layout->setAlignment(buttons[i], Qt::AlignCenter);
		}

		MLabel *botSpacer = new MLabel();
		layout->addItem(botSpacer);
	}

	setDisplay();
}

LabelOrList::~LabelOrList(void)
{
	for (int i = count() - 1; i >= 0; --i) {
		QGraphicsLayoutItem *item = itemAt(i);
		removeAt(i);
		if (item) {
			if (item->ownedByLayout())
				delete item;
		}
	}
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
	mDebug(__func__) << "ENTER, model changed";

	int rowCount = m_model->rowCount();
	mDebug(__func__) << "Model's got " << rowCount << " rows, was "
			 << m_previousRowCount << " before. ";

	if (rowCount != 0 && m_previousRowCount < 1) {
		mDebug(__func__) << "::: showing list";

		/* Need to show the list, not the label */
		if (m_layout->count() != 0) {
			m_layout->removeAt(0);
		}
		m_labelBox->hide();
		m_view->show();
		m_layout->addItem(m_view, 0, 0);

	} else if (rowCount == 0 && m_previousRowCount != 0) {
		mDebug(__func__) << "::: showing label";

		/* Need to show the label, not the list */
		if (m_layout->count() != 0) {
			m_layout->removeAt(0);
		}
		m_view->hide();
		m_labelBox->show();
		m_layout->addItem(m_labelBox, 0, 0);
	}

	m_previousRowCount = rowCount;
}

void LabelOrList::scrollTo(const QModelIndex &index)
{
	m_list->scrollTo(index, 
			 MList::PositionAtTopHint,
			 MList::NonAnimated);
}

QItemSelectionModel *LabelOrList::selectionModel(void)
{
	return m_list->selectionModel();
}

void LabelOrList::selectItem(const QModelIndex &index)
{
	m_list->selectItem(index);
}

void LabelOrList::setLabel(const QString label)
{
	m_label->setText(label);
}
