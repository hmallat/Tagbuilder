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

#include "ContentItemCellCreator.h"

ContentItemCellCreator::ContentItemCellCreator(MContentItem::ContentItemStyle style,
					       const QString styleName)
	: MAbstractCellCreator<MWidgetController>(),
	  m_cellSize(QSizeF(-1, -1)),
	  m_style(style),
	  m_styleName(styleName)
{
  MContentItem *cell = new MContentItem(m_style);
	m_cellSize = cell->effectiveSizeHint(Qt::PreferredSize);
	delete cell;
}

QSizeF ContentItemCellCreator::cellSize(void) const
{
	return m_cellSize;
}

MWidget *ContentItemCellCreator::createCell(const QModelIndex &index, 
					    MWidgetRecycler &recycler) const 
{
        MContentItem *cell = 
		dynamic_cast<MContentItem *>
		(recycler.take(MContentItem::staticMetaObject.className()));
        
        if (cell == NULL) {
		cell = new MContentItem(m_style);
		cell->setStyleName(m_styleName);
        }

        updateCell(index, cell);

        return cell;
}

