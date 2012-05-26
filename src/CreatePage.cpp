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

#include "CreatePage.h"
#include "TextPage.h"
#include "TagTypeListModel.h"
#include "TagTypeListCellCreator.h"
#include "LabelOrList.h"

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new TagTypeListCellCreator;
}

CreatePage::CreatePage(QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_model(new TagTypeListModel(this))
{
}

CreatePage::~CreatePage(void)
{
}

void CreatePage::createContent(void)
{
	createCommonContent(m_model,
			    _getCreator,
			    tr("No tag types to select from"),
			    tr("Select tag type"),
			    false,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(tagTypeSelected(const QModelIndex &)));
}

void CreatePage::tagTypeSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model != 0 ? m_model->name(which) : ""));
}

