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

#include "BookmarkSelectionPageListCellCreator.h"

#include <QVariant>
#include <MContentItem>
#include <QImage>

void BookmarkSelectionPageListCellCreator::updateCell(const QModelIndex &index, 
						      MWidget *cell) const
{
	MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
	QVariant data = index.data(Qt::DisplayRole);
	QStringList parameters = data.value<QStringList>();

	contentItem->setTitle(parameters[0]);
	contentItem->setSubtitle(parameters[1]);
	/* TODO: any proper way of determining this? */
	/* TODO: how slow is image loading? should it be asynch? */
	if (parameters[2].startsWith("/") == true) {
		QImage img(parameters[2]);
		contentItem->setImage(img);
	} else {
		contentItem->setImageID(parameters[2]); 
	}
}
