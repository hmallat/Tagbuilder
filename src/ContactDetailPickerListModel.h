/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CONTACT_DETAIL_PICKER_LIST_MODEL_H_
#define _CONTACT_DETAIL_PICKER_LIST_MODEL_H_

#include <MAbstractItemModel>
#include <QContact>
#include <QContactDetail>
#include <QMap>
#include <QVector>

QTM_USE_NAMESPACE;

class ContactDetailPickerListModel : public MAbstractItemModel
{

	Q_OBJECT;

	enum DetailType {
		Name,
		PhoneNumber,
		EmailAddress,
		PhysicalAddress
	};

public:

	ContactDetailPickerListModel(const QContact &contact,
				     QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	const QContactDetail item(int row, int group) const;

private:

	QContact m_contact;

	QList<enum DetailType> m_types;

	QMap<enum DetailType, QList<QContactDetail> > m_details;

};

#endif /* _CONTACT_DETAIL_PICKER_LIST_MODEL_H_ */
