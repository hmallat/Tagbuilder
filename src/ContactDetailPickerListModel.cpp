/*

    NFC Tag Builder for Nokia N9
    Copyright (C) 2011,2012  Hannu Mallat <hmallat@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "ContactDetailPickerListModel.h"

#include <QContactName>
#include <QContactPhoneNumber>
#include <QContactEmailAddress>
#include <QContactAddress>
#include <QContactUrl>
#include <QContactOrganization>

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
		Util::PhysicalAddress,
		Util::WebAddress,
		Util::Organization
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
		beginRemoveRows(QModelIndex(), 0, m_types.length() - 1, 
				Util::animateLists);
		m_types.clear();
		m_details.clear();
		endRemoveRows();
	}

	if (types.length() > 0) {
		beginInsertRows(QModelIndex(), 0, types.length() - 1, 
				Util::animateLists);
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
	/* TODO: move translations to Util class */
	return 
		(m_types[group] == Util::Name) 
		? tr("Name") :
		(m_types[group] == Util::PhoneNumber) 
		? tr("Phone number") :
		(m_types[group] == Util::EmailAddress) 
		? tr("Email address") :
		(m_types[group] == Util::PhysicalAddress) 
		? tr("Address") :
		(m_types[group] == Util::WebAddress) 
		? tr("Web address") :
		(m_types[group] == Util::Organization) 
		? tr("Organization") :
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

		QString subdetail = "";

		if (number.contexts().length() != 0) {
			QString con = 
				Util::contactDetailContextToString
				(number.contexts()[0]);
			if (con != "") {
				subdetail += con;
			}
		}

		if (number.subTypes().length() != 0) {
			QString sub = 
				Util::phoneNumberSubtypeToString
				(number.subTypes()[0]);
			if (sub != "") {
				subdetail += (subdetail != "" ? ", " : "");
				subdetail += sub;
			}
		}

		parameters << subdetail;

	} else if (type == Util::EmailAddress) {
		QContactEmailAddress addr = 
			static_cast<QContactEmailAddress>(detail);
		parameters << addr.emailAddress();

		QString subdetail = "";

		if (addr.contexts().length() != 0) {
			QString con = 
				Util::contactDetailContextToString
				(addr.contexts()[0]);
			if (con != "") {
				subdetail += con;
			}
		}

		parameters << subdetail;

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

		QString subdetail = "";

		if (addr.contexts().length() != 0) {
			QString con = 
				Util::contactDetailContextToString
				(addr.contexts()[0]);
			if (con != "") {
				subdetail += con;
			}
		}

		parameters << subdetail;

	} else if (type == Util::WebAddress) {
		QContactUrl url = 
			static_cast<QContactUrl>(detail);
		
		mDebug(__func__) << "URL=" << url.url() << "SUBTYPE=" << url.subType();

		parameters << url.url();

		QString subdetail = "";

		if (url.contexts().length() != 0) {
			QString con = 
				Util::contactDetailContextToString
				(url.contexts()[0]);
			if (con != "") {
				subdetail += con;
			}
		}

		parameters << subdetail;

	} else if (type == Util::Organization) {
		QContactOrganization org = 
			static_cast<QContactOrganization>(detail);

		QString rep;
		const char *sep = ", "; 
		if (org.title() != "") {
			if (rep != "") rep += sep;
			rep += org.title();
		}
		if (org.role() != "") {
			if (rep != "") rep += sep;
			rep += org.role();
		}
		if (org.department().length() != 0) {
			for (int i = 0; i < org.department().length(); i++) {
				if (rep != "") rep += sep;
				rep += org.department()[i];
			}
		}
		if (org.name() != "") {
			if (rep != "") rep += sep;
			rep += org.name();
		}
		if (org.location() != "") {
			if (rep != "") rep += sep;
			rep += org.location();
		}

		parameters << rep;

		QString subdetail = "";

		parameters << subdetail;

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
