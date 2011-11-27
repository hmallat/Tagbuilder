/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _URL_PAGE_H
#define _URL_PAGE_H_

#include <MApplicationPage>
#include <QList>

class TextRecordEdit;
class LabeledTextEdit;

class UrlPage : public MApplicationPage
{

	Q_OBJECT;

public:

	UrlPage(int tag = -1, QGraphicsItem *parent = 0);

	~UrlPage(void);

	virtual void createContent(void);

private Q_SLOTS:

	void nameChanged(void);

	void storeTag(void);

private:

	Q_DISABLE_COPY(UrlPage);

	int m_tag;

	LabeledTextEdit *m_name;

	LabeledTextEdit *m_url;

	QList<TextRecordEdit *> m_titleEdits;

	MAction *m_cancelAction;

	MAction *m_storeAction;

};

#endif /* _URL_PAGE_H_ */
