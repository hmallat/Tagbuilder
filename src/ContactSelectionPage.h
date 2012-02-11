/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CONTACT_SELECTION_PAGE_H_
#define _CONTACT_SELECTION_PAGE_H_

#include "SelectionPage.h"

#include "Util.h"
#include <QContactManager>
#include <QContact>

QTM_USE_NAMESPACE;

class ContactSelectionPageListModel;

class ContactSelectionPage : public SelectionPage
{
	Q_OBJECT;

public:

	ContactSelectionPage(QContactManager *manager,
			     Util::ContactDetails requiredDetails = 
			     Util::NoContactDetails,
			     QGraphicsItem *parent = 0);

	virtual ~ContactSelectionPage(void);

        virtual void createContent(void);

signals:

	void selected(const QContact which);

private Q_SLOTS:
	
	void itemsReady(void);

	void contactSelected(const QModelIndex &which);

private:

	Q_DISABLE_COPY(ContactSelectionPage);

	ContactSelectionPageListModel *m_model;

};

#endif /* _CONTACT_SELECTION_PAGE_H_ */
