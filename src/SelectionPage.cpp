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

#include "SelectionPage.h"
#include "LabelOrList.h"
#include "ViewHeader.h"

#include <QGraphicsAnchorLayout>
#include <MAction>
#include <MLabel>
#include <MContentItem>
#include <MAbstractItemModel>

SelectionPage::SelectionPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_list(0)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);
}

SelectionPage::~SelectionPage(void)
{
}

void SelectionPage::createContent(void)
{
}

void SelectionPage::createCommonContent(QAbstractItemModel *itemModel,
					MAbstractCellCreator<MWidgetController> *(*getCreator)(void),
					const QString label,
					const QString title, 
					bool groupedList,
					bool multiSelect)
{
	MApplicationPage::createContent();
	setStyleName("CommonApplicationPage");
	setPannable(false);

	MAction *cancelAction = new MAction(tr("Cancel"), this);
	cancelAction->setLocation(MAction::ToolBarLocation);
	connect(cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(cancelAction);

	if (multiSelect == true) {
		MAction *doneAction = new MAction(tr("Done"), this);
		doneAction->setLocation(MAction::ToolBarLocation);
		connect(doneAction, SIGNAL(triggered()),
			this, SIGNAL(done()));
		addAction(doneAction);
	}

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
	layout->setContentsMargins(0, 0, 0, 0);

	m_header = new ViewHeader(title);

	layout->addCornerAnchors(m_header, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);
	layout->addCornerAnchors(m_header, Qt::TopRightCorner,
				 layout, Qt::TopRightCorner);

	m_list = new LabelOrList(itemModel,
				 getCreator,
				 label,
				 groupedList,
				 multiSelect);
	if (multiSelect == false) {
		connect(m_list, SIGNAL(itemClicked(const QModelIndex)),
			this, SIGNAL(clicked(const QModelIndex)));
	}

	layout->addCornerAnchors(m_list, Qt::TopLeftCorner,
				 m_header, Qt::BottomLeftCorner);
	layout->addCornerAnchors(m_list, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);


	centralWidget()->setLayout(layout);
}

void SelectionPage::setBusy(void)
{
	m_header->setBusy();
}

void SelectionPage::clearBusy(void)
{
	m_header->clearBusy();
}

void SelectionPage::selectAll(MAbstractItemModel *itemModel)
{
	if (itemModel->isGrouped() == true) {
		QModelIndex p;
		for (int i = 0; i < itemModel->groupCount(); i++) {
			QModelIndex g = itemModel->index(i, 0, p);
			for (int j = 0; j < itemModel->rowCountInGroup(i); j++) {
				QModelIndex s = itemModel->index(j, 0, g);
				m_list->selectItem(s);
			}
		}
	} else {
		QModelIndex p;
		for (int j = 0; j < itemModel->rowCountInGroup(0); j++) {
			QModelIndex s = itemModel->index(j, 0, p);
			m_list->selectItem(s);
		}
	}
}
