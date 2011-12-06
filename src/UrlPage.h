/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _URL_PAGE_H
#define _URL_PAGE_H_

#include "CreateEditPage.h"

#include <QSystemInfo>

class TextRecordEdit;
class LabeledTextEdit;
class MButton;

QTM_USE_NAMESPACE;

class UrlPage : public CreateEditPage
{

	Q_OBJECT;

public:

	UrlPage(int tag = -1, QGraphicsItem *parent = 0);

	~UrlPage(void);

protected:

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void urlChanged(void);

	void titleIncludedChanged(bool);

	void titleChanged(void);

private:

	Q_DISABLE_COPY(UrlPage);

	QSystemInfo *m_sysinfo;

	LabeledTextEdit *m_url;

	MButton *m_titleButton;

	TextRecordEdit *m_title;

};

#endif /* _URL_PAGE_H_ */
