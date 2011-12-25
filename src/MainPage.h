/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
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

	void tagSelected(const QModelIndex &);

	void tagLongSelected(const QModelIndex &,
			     const QPointF &);

	void pageAppeared(void);

	void pageDisappearing(void);

	void messageRead(const QNdefMessage contents);

private:

	Q_DISABLE_COPY(MainPage);

	void createObjectMenuActions(void);

	void editTag(int index);

	MObjectMenu *m_objectMenu;

	MObjectMenu *m_unknownObjectMenu;

	QModelIndex m_longTapIndex;

	BluezSupplicant *m_bluez;

	TagReader *m_reader;

};

#endif /* _MAIN_PAGE_H_ */
