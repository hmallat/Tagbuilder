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

#ifndef _TAG_STORAGE_H_
#define _TAG_STORAGE_H_

#include <MAbstractItemModel>
#include <QNdefMessage>

class Tag;
class QString;

QTM_USE_NAMESPACE;

class TagStorage : public MAbstractItemModel
{

	class TagStorageImpl;

public:

	static const int NULL_TAG;

	static const int TEMPORARY_TAG;

	static TagStorage *storage(void);

	int count(void) const;

	const Tag *tag(int which) const;

	bool append(const QString &name,
		    const QNdefMessage &message);

	bool update(int which, 
		    const QString &name,
		    const QNdefMessage &message);
	
	bool remove(int which);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

private:

	Q_DISABLE_COPY(TagStorage);

	TagStorage(TagStorageImpl *impl, QObject *parent = 0);

	~TagStorage(void);

	TagStorageImpl *m_impl;

};

#endif /* _TAG_STORAGE_H_ */
