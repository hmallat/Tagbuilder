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

#include "Util.h"

#include <QContact>
#include <QContactDetail>
#include <QMap>
#include <QVector>

QTM_USE_NAMESPACE;

class ContactDetailPickerListModel : public MAbstractItemModel
{

	Q_OBJECT;

public:

	ContactDetailPickerListModel(Util::ContactDetails details,
				     QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	const QContactDetail detail(const QModelIndex &index) const;

	void setContact(const QContact &contact);

private:

	Util::ContactDetails m_filter;

	QList<enum Util::ContactDetail> m_types;

	QMap<enum Util::ContactDetail, QList<QContactDetail> > m_details;

};

#endif /* _CONTACT_DETAIL_PICKER_LIST_MODEL_H_ */
