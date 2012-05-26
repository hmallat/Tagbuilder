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

#include "BookmarkSelectionPage.h"
#include "BookmarkSelectionPageListCellCreator.h"
#include "BookmarkSelectionPageListModel.h"
#include "LabelOrList.h"

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new BookmarkSelectionPageListCellCreator;
}

BookmarkSelectionPage::BookmarkSelectionPage(QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_model(new BookmarkSelectionPageListModel(this))
{
}

BookmarkSelectionPage::~BookmarkSelectionPage(void)
{
}

void BookmarkSelectionPage::createContent(void)
{
	createCommonContent(m_model,
			    _getCreator,
			    tr("Fetching bookmarks"),
			    tr("Select bookmark"),
			    false,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(bookmarkSelected(const QModelIndex &)));

	connect(m_model, SIGNAL(ready()), this, SLOT(itemsReady()));
	if (m_model->fetch() == true) {
		setBusy();
	} else {
		m_list->setLabel(tr("Cannot retrieve bookmarks"));
	}
}

void BookmarkSelectionPage::itemsReady(void)
{
	clearBusy();
	m_list->setLabel(tr("No bookmarks to select from"));
}

void BookmarkSelectionPage::bookmarkSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->bookmark(which)));
}
