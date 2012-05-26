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

#ifndef _MAIN_PAGE_H_
#define _MAIN_PAGE_H_

#include <MApplicationPage>
#include <QModelIndex>
#include <QNdefMessage>

class QGraphicsAnchorLayout;
class MObjectMenu;
class BluezSupplicant;
class TagReader;
class NfcdMonitor;

QTM_USE_NAMESPACE;

class MainPage : public MApplicationPage
{
	Q_OBJECT;

public:

	MainPage(QGraphicsItem *parent = 0);

	virtual ~MainPage(void);

        virtual void createContent(void);

private Q_SLOTS:

	void createTag(void);

	void createSelectedTag(QString which);

	void editTag(void);

	void removeTag(void);

	void removeAllTags(void);

	void showAbout(void);

	void showHelp(void);

	void tagSelected(const QModelIndex &);

	void tagLongSelected(const QModelIndex &,
			     const QPointF &);

	void pageAppeared(void);

	void pageDisappearing(void);

	void messageRead(const QNdefMessage contents);

	void linkActivated(const QString &);

private:

	Q_DISABLE_COPY(MainPage);

	void createObjectMenuActions(void);

	void createActions(void);

	void editTag(int index);

	MObjectMenu *m_objectMenu;

	MObjectMenu *m_unknownObjectMenu;

	QModelIndex m_longTapIndex;

	BluezSupplicant *m_bluez;

	TagReader *m_reader;

	NfcdMonitor *m_monitor;

};

#endif /* _MAIN_PAGE_H_ */
