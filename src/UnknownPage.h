/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _UNKNOWN_PAGE_H
#define _UNKNOWN_PAGE_H_

#include "CreateEditPage.h"

#include <QNdefMessage>

QTM_USE_NAMESPACE;

class UnknownRecordListModel;
class LabelOrList;

class UnknownPage : public CreateEditPage
{

	Q_OBJECT;

public:

	UnknownPage(int tag = TagStorage::NULL_TAG, 
		    QGraphicsItem *parent = 0);

	virtual ~UnknownPage(void);

protected:

	virtual void createPageSpecificActions(void);

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private:

	Q_DISABLE_COPY(UnknownPage);

#ifdef LABEL_SIZE
	void updateSize(void);
#endif

	QNdefMessage m_message;

	LabelOrList *m_unknownDetails;

	UnknownRecordListModel *m_model;

};

#endif /* _UNKNOWN_PAGE_H_ */
