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

class QGraphicsAnchorLayout;
class MObjectMenu;

class MainPage : public MApplicationPage
{
	Q_OBJECT;

public:

	MainPage(QGraphicsItem *parent = 0);

	~MainPage(void);

        virtual void createContent(void);

private Q_SLOTS:

	void createTag(void);

	void createSelectedTag(QString which);

	void editTag(void);

	void removeTag(void);

	void showAbout(void);

	void refreshList(void);

	void tagSelected(const QModelIndex &);

	void tagLongSelected(const QModelIndex &,
			     const QPointF &);

private:

	Q_DISABLE_COPY(MainPage);

	void createObjectMenuActions(void);

	void createTagButtons(QGraphicsAnchorLayout *layout);

	MObjectMenu *m_objectMenu;

	QModelIndex m_longTapIndex;

};

#endif /* _MAIN_PAGE_H_ */
