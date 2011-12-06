/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TEXT_PAGE_H_
#define _TEXT_PAGE_H_

#include "CreateEditPage.h"

#include <QSystemInfo>

class TextRecordEdit;
class LabeledTextEdit;

QTM_USE_NAMESPACE;

class TextPage : public CreateEditPage
{
	Q_OBJECT;

public:

	TextPage(int tag = -1, QGraphicsItem *parent = 0);

	~TextPage(void);

protected:

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void textChanged(void);

private:

	Q_DISABLE_COPY(TextPage);

	TextRecordEdit *m_edit;

	QSystemInfo *m_sysinfo;

};

#endif /* _TEXT_PAGE_H_ */
