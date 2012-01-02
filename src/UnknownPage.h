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

private Q_SLOTS:

	void showContents(void);

private:

	Q_DISABLE_COPY(UnknownPage);

	void updateSize(void);

	QNdefMessage m_message;

};

#endif /* _UNKNOWN_PAGE_H_ */
