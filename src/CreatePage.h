/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CREATE_PAGE_H_
#define _CREATE_PAGE_H_

#include <MApplicationPage>

class TagTypeListModel;
class QModelIndex;

class CreatePage : public MApplicationPage
{
	Q_OBJECT;

public:

	CreatePage(QGraphicsItem *parent = 0);

	~CreatePage(void);

        virtual void createContent(void);

signals:

	void selected(QString which);

private Q_SLOTS:
	
	void tagTypeSelected(const QModelIndex &which);

private:

	Q_DISABLE_COPY(CreatePage);

	TagTypeListModel *m_model;

};

#endif /* _CREATE_PAGE_H_ */
