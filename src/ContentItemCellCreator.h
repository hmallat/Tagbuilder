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

#ifndef _CONTENT_ITEM_CELL_CREATOR_H_
#define _CONTENT_ITEM_CELL_CREATOR_H_

#include <MAbstractCellCreator>
#include <MContentItem>

class ContentItemCellCreator : public MAbstractCellCreator<MWidgetController>
{

public:

	ContentItemCellCreator(MContentItem::ContentItemStyle style = MContentItem::IconAndTwoTextLabels,
			       const QString styleName = "CommonContentItem");

	virtual MWidget *createCell(const QModelIndex &index, 
				    MWidgetRecycler &recycler) const;

	virtual QSizeF cellSize(void) const;

private:

	QSizeF m_cellSize;

	MContentItem::ContentItemStyle m_style;

	QString m_styleName;

};

#endif /* _CONTENT_ITEM_CELL_CREATOR_H_ */
