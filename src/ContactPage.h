/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CONTACT_PAGE_H
#define _CONTACT_PAGE_H_

#include <MApplicationPage>
#include <QContact>
#include <QContactManager>
#include <QContactFetchRequest>

class LabeledTextEdit;
class MContentItem;
class MButton;
class MAction;

QTM_USE_NAMESPACE;

class ContactPage : public MApplicationPage
{

	Q_OBJECT;

public:

	ContactPage(int tag = -1, 
		    QGraphicsItem *parent = 0);

	~ContactPage(void);

	virtual void createContent(void);

private Q_SLOTS:

	void nameChanged(void);

	void storeTag(void);

	void useMyInformation(void);

	void chooseFromAddressbook(void);

	void singleContactFetched(void);

	void setContact(const QContact contact);

private:

	Q_DISABLE_COPY(ContactPage);

	int m_tag;

	LabeledTextEdit *m_name;

	MContentItem *m_contact;

	MAction *m_cancelAction;

	MAction *m_storeAction;

	QContactManager m_contactManager;

	QContactFetchRequest *m_fetch;

	QContact m_info;

};

#endif /* _CONTACT_PAGE_H_ */
