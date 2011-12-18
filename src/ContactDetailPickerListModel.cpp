/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactDetailPickerListModel.h"
#include "Util.h"

#include <QContactPhoneNumber>
#include <MDebug>

ContactDetailPickerListModel::
ContactDetailPickerListModel(const QContact &contact,
			      QObject *parent)
	: MAbstractItemModel(parent),
	  m_contact(contact),
	  m_types(),
	  m_details()
{
	setGrouped(true);

	QList<enum DetailType> types;
	QMap<enum DetailType, QList<QContactDetail> > details;

	QList<QContactDetail> numbers = 
		m_contact.details(QContactPhoneNumber::DefinitionName);
	if (numbers.length() != 0) {
		types << PhoneNumber;
		details[PhoneNumber] = numbers;
	}

	Q_EMIT(layoutAboutToBeChanged());

	beginInsertRows(QModelIndex(), 0, types.length() - 1, false);
	m_types = types;
	m_details = details;
	endInsertRows();

	Q_EMIT(layoutChanged());
}

int ContactDetailPickerListModel::groupCount(void) const
{
	return m_types.length();
}

int ContactDetailPickerListModel::rowCountInGroup(int group) const
{
	return m_details[m_types[group]].length();
}

QString ContactDetailPickerListModel::groupTitle(int group) const
{
	return 
		(m_types[group] == PhoneNumber) ? tr("Phone numbers") :
		tr("Unknown type");
}

QVariant ContactDetailPickerListModel::itemData(int row, 
						int group, 
						int role) const
{
	(void)role;

	QStringList parameters;
	enum DetailType type = m_types[group];
	QContactDetail detail = m_details[type][row];

	if (type == PhoneNumber) {
		QContactPhoneNumber number= 
			static_cast<QContactPhoneNumber>(detail);
		parameters << number.number();
		if (number.subTypes().length() != 0) {
			parameters << 
				Util::phoneNumberSubtypeToString
				(number.subTypes()[0]);
		} else {
			parameters << "";
		}
	}

	return qVariantFromValue(parameters);
}
