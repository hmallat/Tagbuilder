/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactDetailPickerListModel.h"

#include <QContactName>
#include <QContactPhoneNumber>
#include <QContactEmailAddress>
#include <QContactAddress>

#include <MDebug>

/* TODO: maybe add more details, at least online accounts */

ContactDetailPickerListModel::
ContactDetailPickerListModel(Util::ContactDetails d,
			     QObject *parent)
	: MAbstractItemModel(parent),
	  m_filter(d),
	  m_types(),
	  m_details()
{
	setGrouped(true);
}

void ContactDetailPickerListModel::setContact(const QContact &contact)
{
	QList<enum Util::ContactDetail> types;
	QMap<enum Util::ContactDetail, QList<QContactDetail> > details;

	Util::ContactDetail type[CONTACT_DETAILS] = {
		Util::Name, 
		Util::PhoneNumber, 
		Util::EmailAddress,
		Util::PhysicalAddress
	};

	for (int i = 0; i < CONTACT_DETAILS; i++) {
		if ((m_filter & type[i]) != 0) {
			const QString name = Util::contactDetailName(type[i]);
			QList<QContactDetail> dets = contact.details(name);
			if (dets.length() != 0) {
				types << type[i];
				details[type[i]] = dets;
			}
		}
	}

	Q_EMIT(layoutAboutToBeChanged());

	if (m_types.length() > 0) {
		beginRemoveRows(QModelIndex(), 0, m_types.length() - 1, false);
		m_types.clear();
		m_details.clear();
		endRemoveRows();
	}

	if (types.length() > 0) {
		beginInsertRows(QModelIndex(), 0, types.length() - 1, false);
		m_types = types;
		m_details = details;
		endInsertRows();
	}

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
		(m_types[group] == Util::Name) 
		? tr("Name") :
		(m_types[group] == Util::PhoneNumber) 
		? tr("Phone number") :
		(m_types[group] == Util::EmailAddress) 
		? tr("Email address") :
		(m_types[group] == Util::PhysicalAddress) 
		? tr("Address") :
		tr("Unknown type");
}

QVariant ContactDetailPickerListModel::itemData(int row, 
						int group, 
						int role) const
{
	(void)role;

	QStringList parameters;
	enum Util::ContactDetail type = m_types[group];
	QContactDetail detail = m_details[type][row];

	if (type == Util::Name) {
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

	} else if (type == Util::PhoneNumber) {
		QContactPhoneNumber number = 
			static_cast<QContactPhoneNumber>(detail);
		parameters << number.number();

		QString detail = "";

		if (number.contexts().length() != 0) {
			QString con = 
				Util::contactDetailContextToString
				(number.contexts()[0]);
			if (con != "") {
				detail += con;
			}
		}

		if (number.subTypes().length() != 0) {
			QString sub = 
				Util::phoneNumberSubtypeToString
				(number.subTypes()[0]);
			if (sub != "") {
				detail += (detail != "" ? ", " : "");
				detail += sub;
			}
		}

		parameters << detail;

	} else if (type == Util::EmailAddress) {
		QContactEmailAddress addr = 
			static_cast<QContactEmailAddress>(detail);
		parameters << addr.emailAddress();

		QString detail = "";

		if (addr.contexts().length() != 0) {
			QString con = 
				Util::contactDetailContextToString
				(addr.contexts()[0]);
			if (con != "") {
				detail += con;
			}
		}

		parameters << detail;

	} else if (type == Util::PhysicalAddress) {
		QContactAddress addr = 
			static_cast<QContactAddress>(detail);

		/* TODO: this shit should be localized */
		/* TODO: list cells are fixed size? If so, cannot use <br> */
		QString rep;
		const char *sep = ", "; /* "<br>" */
		if (addr.postOfficeBox() != "") {
			if (rep != "") rep += sep;
			rep += addr.postOfficeBox();
		}
		if (addr.street() != "") {
			if (rep != "") rep += sep;
			rep += addr.street();
		}
		if (addr.postcode() != "") {
			if (rep != "") rep += sep;
			rep += addr.postcode();
		}
		if (addr.locality() != "") {
			if (rep != "") rep += sep;
			rep += addr.locality();
		}
		if (addr.region() != "") {
			if (rep != "") rep += sep;
			rep += addr.region();
		}
		if (addr.country() != "") {
			if (rep != "") rep += sep;
			rep += addr.country();
		}

		parameters << rep;

		QString detail = "";

		if (addr.contexts().length() != 0) {
			QString con = 
				Util::contactDetailContextToString
				(addr.contexts()[0]);
			if (con != "") {
				detail += con;
			}
		}

		parameters << detail;

	}

	return qVariantFromValue(parameters);
}

const QContactDetail 
ContactDetailPickerListModel::detail(const QModelIndex &index) const
{
	int row = index.row();
	int group = index.parent().row();
	enum Util::ContactDetail type = m_types[group];
	return m_details[type][row];
}
