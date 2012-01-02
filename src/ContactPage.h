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

class MLabel;
class MList;

QTM_USE_NAMESPACE;

class ContactPage : public CreateEditPage
{

	Q_OBJECT;

public:

	ContactPage(int tag = TagStorage::NULL_TAG, 
		    QGraphicsItem *parent = 0);

	virtual ~ContactPage(void);

protected:

	virtual void createPageSpecificActions(void);

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void chooseFromAddressbook(void);

	void setContactThroughAction(const QContact contact);

	void setContactDetails(const QContact contact);

private:

	Q_DISABLE_COPY(ContactPage);

	void updateSize(void);

	void setContact(const QContact contact);

	QContactManager m_contactManager;

	QContact m_info;

	MLabel *m_contactTitle;

	MList *m_contactDetails;

};

#endif /* _CONTACT_PAGE_H_ */
