/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CONTACT_DETAIL_PICKER_H_
#define _CONTACT_DETAIL_PICKER_H_

#include <MApplicationPage>
#include <QContact>

QTM_USE_NAMESPACE;

class LabelOrList;
class MAction;
class QGraphicsLinearLayout;
class ContactDetailPickerListModel;
class QModelIndex;

class ContactDetailPicker : public MApplicationPage
{

	Q_OBJECT;

public:

	ContactDetailPicker(const QContact contact, QGraphicsItem *parent = 0);

	virtual ~ContactDetailPicker(void);

        virtual void createContent(void);

Q_SIGNALS:

	void contactPicked(const QContact contact);

private Q_SLOTS:

	void pickingDone(void);

private:

	Q_DISABLE_COPY(ContactDetailPicker);

	QContact m_contact;

	ContactDetailPickerListModel *m_model;

	MAction *m_cancelAction;

	MAction *m_doneAction;

	LabelOrList *m_list;

};

#endif /* _CONTACT_DETAIL_PICKER_H_ */
