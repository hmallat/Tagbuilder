/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CONTACT_PAGE_H
#define _CONTACT_PAGE_H_

#include "CreateEditPage.h"

#include <QContact>
#include <QContactManager>

class MContentItem;

QTM_USE_NAMESPACE;

class ContactPage : public CreateEditPage
{

	Q_OBJECT;

public:

	ContactPage(int tag = -1, 
		    QGraphicsItem *parent = 0);

	~ContactPage(void);

protected:

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void chooseFromAddressbook(void);

	void setContact(const QContact contact);

private:

	Q_DISABLE_COPY(ContactPage);

	void updateSize(void);

	MContentItem *m_contact;

	QContactManager m_contactManager;

	QContact m_info;

};

#endif /* _CONTACT_PAGE_H_ */
