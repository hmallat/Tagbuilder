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

#include "TagListCellCreator.h"
#include "Tag.h"
#include "MainPage.h"
#include "Util.h"

#include <QVariant>

void TagListCellCreator::updateCell(const QModelIndex &index, 
				    MWidget *cell) const
{
	QVariant data = index.data(Qt::DisplayRole);
	const Tag *tag = data.value<const Tag *>();
	quint32 total = Util::messageLength(tag->message());

	MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
	contentItem->setTitle(tag->name());
	contentItem->setImageID(Tag::icon(tag->type()));
	contentItem->setSubtitle(QObject::tr("%1 bytes").arg(total));
}
