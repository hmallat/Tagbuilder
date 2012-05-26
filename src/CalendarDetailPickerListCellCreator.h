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

#ifndef _CALENDAR_DETAIL_PICKER_LIST_CELL_CREATOR_H_
#define _CALENDAR_DETAIL_PICKER_LIST_CELL_CREATOR_H_

#include "ContentItemCellCreator.h"

class CalendarDetailPickerListCellCreator : public ContentItemCellCreator
{

public:

	CalendarDetailPickerListCellCreator(const QString styleName = "CommonContentItem");

	virtual void updateCell(const QModelIndex &index, MWidget *cell) const;

};

#endif /* _CALENDAR_DETAIL_PICKER_LIST_CELL_CREATOR_H_ */
