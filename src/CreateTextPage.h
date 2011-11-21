/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CREATE_TEXT_PAGE_H_
#define _CREATE_TEXT_PAGE_H_

#include <QString>
#include <QSystemInfo>
#include <MApplicationPage>

class TextRecordEdit;

QTM_USE_NAMESPACE;

class CreateTextPage : public MApplicationPage
{
	Q_OBJECT;

public:

	CreateTextPage(QGraphicsItem *parent = 0);

	~CreateTextPage(void);

        virtual void createContent(void);

private Q_SLOTS:

	void saveTag(void);

private:

	Q_DISABLE_COPY(CreateTextPage);

	TextRecordEdit *m_edit;

	QSystemInfo *m_sysinfo;

};

#endif /* _CREATE_TEXT_PAGE_H_ */
