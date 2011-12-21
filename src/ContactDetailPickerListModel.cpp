/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactDetailPickerListModel.h"
#include "Util.h"

#include <QContactName>
#include <QContactPhoneNumber>
#include <QContactEmailAddress>
#include <QContactAddress>

#include <MDebug>

/* TODO: maybe add more details, at least online accounts */

ContactDetailPickerListModel::
ContactDetailPickerListModel(const QContact &contact,
			      QObject *parent)
	: MAbstractItemModel(parent),
	  m_contact(contact),
	  m_types(),
	  m_details(),
	  m_selection()
{
	setGrouped(true);

	QList<enum DetailType> types;
	QMap<enum DetailType, QList<QContactDetail> > details;
	QMap<enum DetailType, QVector<bool> > selections;

	QList<QContactDetail> names = 
		m_contact.details(QContactName::DefinitionName);
	if (names.length() != 0) {
		types << Name;
		details[Name] = names;
		selections[Name] = QVector<bool>(names.length(), 
						 false);
	}

	QList<QContactDetail> numbers = 
		m_contact.details(QContactPhoneNumber::DefinitionName);
	if (numbers.length() != 0) {
		types << PhoneNumber;
		details[PhoneNumber] = numbers;
		selections[PhoneNumber] = QVector<bool>(numbers.length(), 
							false);
	}

	QList<QContactDetail> emails = 
		m_contact.details(QContactEmailAddress::DefinitionName);
	if (emails.length() != 0) {
		types << EmailAddress;
		details[EmailAddress] = emails;
		selections[EmailAddress] = QVector<bool>(emails.length(), 
							 false);
	}

	QList<QContactDetail> addresses = 
		m_contact.details(QContactAddress::DefinitionName);
	if (addresses.length() != 0) {
		types << PhysicalAddress;
		details[PhysicalAddress] = addresses;
		selections[PhysicalAddress] = QVector<bool>(addresses.length(),
							    false);
	}

	Q_EMIT(layoutAboutToBeChanged());

	beginInsertRows(QModelIndex(), 0, types.length() - 1, false);
	m_types = types;
	m_details = details;
	m_selection = selections;
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
		(m_types[group] == Name) ? tr("Name") :
		(m_types[group] == PhoneNumber) ? tr("Phone number") :
		(m_types[group] == EmailAddress) ? tr("Email address") :
		(m_types[group] == PhysicalAddress) ? tr("Address") :
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

	if (type == Name) {
		QContactName name = 
			static_cast<QContactName>(detail);

		/* TODO: might need to localize this shit */
		QString rep;
		if (name.prefix() != "") {
			rep += name.prefix() + " ";
		}
		if (name.firstName() != "") {
			rep += name.firstName() + " ";
		}
		if (name.middleName() != "") {
			rep += name.middleName() + " ";
		}
		if (name.lastName() != "") {
			rep += name.lastName() + " ";
		}
		if (name.suffix() != "") {
			rep += name.suffix() + " ";
		}

		parameters << rep;
		parameters << "";

	} else if (type == PhoneNumber) {
		QContactPhoneNumber number = 
			static_cast<QContactPhoneNumber>(detail);
		parameters << number.number();
		if (number.subTypes().length() != 0) {
			parameters << 
				Util::phoneNumberSubtypeToString
				(number.subTypes()[0]);
		} else {
			parameters << "";
		}

	} else if (type == EmailAddress) {
		QContactEmailAddress addr = 
			static_cast<QContactEmailAddress>(detail);
		parameters << addr.emailAddress();
		parameters << "";

	} else if (type == PhysicalAddress) {
		QContactAddress addr = 
			static_cast<QContactAddress>(detail);

		/* TODO: this shit should be localized */
		QString rep;
		if (addr.postOfficeBox() != "") {
			rep += addr.postOfficeBox() + "<br>";
		}
		if (addr.street() != "") {
			rep += addr.street() + "<br>";
		}
		if (addr.postcode() != "") {
			rep += addr.postcode() + "<br>";
		}
		if (addr.locality() != "") {
			rep += addr.locality() + "<br>";
		}
		if (addr.region() != "") {
			rep += addr.region() + "<br>";
		}
		if (addr.country() != "") {
			rep += addr.country() + "<br>";
		}

		parameters << rep;
		if (addr.subTypes().length() != 0) {
			parameters << 
				Util::addressSubtypeToString
				(addr.subTypes()[0]);
		} else {
			parameters << "";
		}
	}

	return qVariantFromValue(parameters);
}

const QContactDetail ContactDetailPickerListModel::item(int row, 
							int group) const
{
	enum DetailType type = m_types[group];
	return m_details[type][row];
}

bool ContactDetailPickerListModel::itemSelected(const QModelIndex &index) const
{
	int row = index.row();
	int group = index.parent().row();
	enum DetailType type = m_types[group];
	return m_selection[type][row];
}

void ContactDetailPickerListModel::setItemSelected(const QModelIndex &index, 
						   bool selected)
{
	int row = index.row();
	int group = index.parent().row();
	enum DetailType type = m_types[group];
	m_selection[type][row] = selected;
	mDebug(__func__) << "group " << group 
			 << ", row " << row 
			 << "set to " << (selected ? "" : "de") 
			 << "selected. ";
}



