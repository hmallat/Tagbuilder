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

class TextRecordEdit;
class LabeledTextEdit;

QTM_USE_NAMESPACE;

class TextPage : public CreateEditPage
{
	Q_OBJECT;

public:

	TextPage(int tag = TagStorage::NULL_TAG, 
		 QGraphicsItem *parent = 0);

	virtual ~TextPage(void);

protected:

	virtual void createPageSpecificActions(void);

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void textChanged(void);

	void langChanged(void);

private:

	Q_DISABLE_COPY(TextPage);

#ifdef LABEL_SIZE
	void updateSize(void);
#endif

	TextRecordEdit *m_edit;

};

#endif /* _TEXT_PAGE_H_ */
