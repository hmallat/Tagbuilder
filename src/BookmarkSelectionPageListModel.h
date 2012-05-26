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

#ifndef _BOOKMARK_SELECTION_PAGE_LIST_MODEL_H_
#define _BOOKMARK_SELECTION_PAGE_LIST_MODEL_H_

#include <MAbstractItemModel>
#include <QNdefMessage>

QTM_USE_NAMESPACE;

class QSparqlConnection;
class QSparqlResult;

class BookmarkSelectionPageListModel : public MAbstractItemModel
{

	Q_OBJECT;

	struct Bookmark {
		QString url;
		QString name;
		QString icon;
	};

public:

	BookmarkSelectionPageListModel(QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	QNdefMessage bookmark(const QModelIndex &index) const;

	bool fetch(void);

Q_SIGNALS:

	void ready(void);

protected:

	QList<struct Bookmark> m_bookmarks;

	QSparqlConnection *m_connection;

	QSparqlResult *m_result;

private Q_SLOTS:
	
	void queryFinished(void);

private:
	static bool readDesktopFile(const QString path,
				    QList<struct Bookmark> &list);

};

#endif /* _BOOKMARK_SELECTION_PAGE_LIST_MODEL_H_ */
