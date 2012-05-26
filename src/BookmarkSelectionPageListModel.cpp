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

#include "BookmarkSelectionPageListModel.h"
#include "SmartPosterRecord.h"
#include "Util.h"

#include <QNdefNfcUriRecord>
#include <QNdefNfcTextRecord>
#include <QStringList>
#include <QUrl>
#include <QSparqlConnection>
#include <QSparqlQuery>
#include <QSparqlResult>
#include <QSparqlError>
#include <QSettings>

#include <MDebug>

static QNdefMessage _bookmarkToMessage(QString url,
				       QString name,
				       QString icon)
{
	(void) icon;

	QNdefMessage message;

	QNdefNfcUriRecord u;
	u.setUri(url);

	if (name != "") {
		SmartPosterRecord sp;

		sp.setUri(u);

		QNdefNfcTextRecord t;
		t.setText(name);
		t.setLocale(Util::currentLanguageCode());

		QList<QNdefNfcTextRecord> ts;
		ts << t;
		sp.setTitles(ts);

		message << sp;
	} else {
		message << u;
	}

	return message;
}

bool 
BookmarkSelectionPageListModel::readDesktopFile(const QString path,
						QList<struct Bookmark> &list)
{
	QSettings settings(path, QSettings::IniFormat);
	settings.beginGroup("Desktop Entry");
	if (settings.contains("Type") == true &&
	    settings.value("Type").toString() == "Link") {
		QString url;
		QString name;
		QString icon;

		/* TODO: worry about encoding? */

		if (settings.contains("URL") == true) {
			url = settings.value("URL").toString();
		}
		
		if (settings.contains("Name") == true) {
			name = settings.value("Name").toString();
		}
		
		if (settings.contains("Icon") == true) {
			icon = settings.value("Icon").toString();
		}
		
		if (url != "") {
			struct Bookmark b = { url, name, icon };
			list << b;
			return true;
		}
	}

	return false;
}

BookmarkSelectionPageListModel::
BookmarkSelectionPageListModel(QObject *parent)
	: MAbstractItemModel(parent),
	  m_bookmarks(),
	  m_connection(new QSparqlConnection("QTRACKER",
					     QSparqlConnectionOptions(),
					     this))
{
	setGrouped(false);
}

bool BookmarkSelectionPageListModel::fetch(void)
{
	QSparqlQuery q = QSparqlQuery("select ?b where { ?b a nfo:Bookmark }");
	m_result = m_connection->exec(q);
	if (m_result == 0 || m_result->hasError() == true) {
		mDebug(__func__) << "Failed to execute query: "
				 << ((m_result == 0) 
				     ? "Null result"
				     : m_result->lastError().message());
		return false;
	} 

	connect(m_result, SIGNAL(finished()),
		this, SLOT(queryFinished()));
	return true;
}

void BookmarkSelectionPageListModel::queryFinished(void)
{
	QList<struct Bookmark> marks;

	if (m_result->hasError() == true) {
		mDebug(__func__) << "Failed to execute query: "
				 << m_result->lastError().message();
		goto done;
	} else {
		mDebug(__func__) << "Query returned " 
				 << m_result->size() 
				 << " rows. ";
		m_result->first();
		while (m_result->isValid() == true) {
			QString s = m_result->stringValue(0);
			mDebug(__func__) << s;
			if (s.startsWith("file://") == true) {
				s = s.mid(7);
				if (readDesktopFile(s, marks) == false) {
					mDebug(__func__) << "Failed to read "
							 << s;
				}
			}
			m_result->next();
		}
	}

	if (marks.length() == 0) {
		mDebug(__func__) << "Nothing to be inserted. ";
		goto done;
	}

	Q_EMIT(layoutAboutToBeChanged());

	beginInsertRows(QModelIndex(), 0, marks.length() - 1, 
			Util::animateLists);
	m_bookmarks = marks;
	endInsertRows();

	Q_EMIT(layoutChanged());

done:
	Q_EMIT(ready());
}

int BookmarkSelectionPageListModel::groupCount(void) const
{
	return 1;
}

int BookmarkSelectionPageListModel::rowCountInGroup(int group) const
{
	(void) group;
	return m_bookmarks.length();
}

QString BookmarkSelectionPageListModel::groupTitle(int group) const
{
	(void) group;
	return "";
}

QVariant BookmarkSelectionPageListModel::itemData(int row, 
						  int group, 
						  int role) const
{
	(void) group;
	(void) role;

	QStringList parameters;
	parameters << m_bookmarks[row].name;
	parameters << m_bookmarks[row].url;
	parameters << m_bookmarks[row].icon;

	return qVariantFromValue(parameters);
}

QNdefMessage 
BookmarkSelectionPageListModel::bookmark(const QModelIndex &index) const
{
	return _bookmarkToMessage(m_bookmarks[index.row()].url,
				  m_bookmarks[index.row()].name,
				  m_bookmarks[index.row()].icon);
}
