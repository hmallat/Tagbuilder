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

#include "ContactSelectionPageListModel.h"

#include <QContactSortOrder>
#include <QStringList>
#include <MDebug>

#include <QContactName>
#include <QContactPhoneNumber>
#include <QContactEmailAddress>
#include <QContactAddress>

ContactSelectionPageListModel::
ContactSelectionPageListModel(QContactManager *manager,
			      Util::ContactDetails requiredDetails,
			      QObject *parent)
	: MAbstractItemModel(parent),
	  m_manager(manager),
	  m_fetch(0),
	  m_contacts(),
	  m_buckets(),
	  m_requiredDetails(requiredDetails)
{
	setGrouped(true);
}

bool ContactSelectionPageListModel::fetch(void)
{
	if (m_fetch != NULL) {
		delete m_fetch;
		m_fetch = NULL;
	}

	m_fetch = new QContactFetchRequest(this);
	m_fetch->setManager(m_manager);

	connect(m_fetch, SIGNAL(resultsAvailable()),
		this, SLOT(resultsAvailable()));
	connect(m_fetch, SIGNAL(stateChanged(QContactAbstractRequest::State)),
		this, SLOT(stateChanged(QContactAbstractRequest::State)));

	mDebug(__func__) << "Starting fetch. ";
	if (m_fetch->start() == false) {
		mDebug(__func__) << "Cannot fetch contacts. ";
		return false;
	}

	return true;
}

void ContactSelectionPageListModel::resultsAvailable(void)
{
	QList<QContact> results;
	QStringList bucketEntries;
	QMap<QString, QContact> insertions;

	if (m_fetch->error() != QContactManager::NoError) {
		mDebug(__func__) << "Cannot fetch contacts, error " 
				 << m_fetch->error();
		/* TODO: indicate in UI */
		goto done;
	}

	results = m_fetch->contacts();

	for (int i = 0; i < results.length(); i++) {
		bool include = true;

		QContact contact = results[i];
		if (contact.displayLabel() == "") {
			continue;
		}

		if (insertions.contains(contact.displayLabel())) {
			mDebug(__func__) << contact.displayLabel() 
					 << " is duplicate, skipping. ";
			continue;
		}

		Util::ContactDetail detail[CONTACT_DETAILS] = {
			Util::Name, 
			Util::PhoneNumber, 
			Util::EmailAddress,
			Util::PhysicalAddress,
			Util::WebAddress,
			Util::Organization
		};

		for (int j = 0; j < CONTACT_DETAILS; j++) {
			const QString name = Util::contactDetailName(detail[j]);
			if ((m_requiredDetails & detail[j]) != 0 &&
			    contact.details(name).length() == 0) {
				mDebug(__func__) << contact.displayLabel() 
						 << " doesn't have "
						 << name
						 << ", skipping. ";
				include = false;
				break;
			}
		}

		if (include == true) {
			insertions[contact.displayLabel()] = contact;
			bucketEntries << contact.displayLabel();
		}

	}
	if (bucketEntries.length() == 0) {
		mDebug(__func__) << "Nothing to be inserted. ";
		/* TODO: indicate in UI */
		goto done;
	}

done:
	Q_EMIT(layoutAboutToBeChanged());

	if (m_buckets.bucketCount() > 0) {
		beginRemoveRows(QModelIndex(), 0, m_buckets.bucketCount() - 1,
				Util::animateLists);
		m_buckets.clear();
		m_contacts.clear();
		endRemoveRows();
	}

	if (bucketEntries.length() > 0) {
		beginInsertRows(QModelIndex(), 0, bucketEntries.length() - 1,
				Util::animateLists);
		m_buckets.setItems(bucketEntries);
		m_contacts = insertions;
		endInsertRows();
	}

	Q_EMIT(layoutChanged());

	Q_EMIT(ready());
}

void ContactSelectionPageListModel::stateChanged(QContactAbstractRequest::State)
{
	/* TODO */
}

int ContactSelectionPageListModel::groupCount(void) const
{
	return m_buckets.bucketCount();
}

int ContactSelectionPageListModel::rowCountInGroup(int group) const
{
	return m_buckets.bucketSize(group);
}

QString ContactSelectionPageListModel::groupTitle(int group) const
{
	return m_buckets.bucketName(group);
}

QVariant ContactSelectionPageListModel::itemData(int row, 
						 int group, 
						 int role) const
{
	(void)role;

	QStringList parameters;
	QString label = m_buckets.bucketContent(group)[row];
	if (m_contacts.contains(label)) {
		QContact contact = m_contacts[label];
		parameters 
			<< contact.displayLabel()
			<< "icon-m-content-avatar-placeholder" /* TODO */;
	} else {
		parameters 
			<< ""
			<< "";
	}

	return qVariantFromValue(parameters);
}

QContact ContactSelectionPageListModel::contact(const QModelIndex &index) const
{
	int row = index.row();
	int group = index.parent().row();

	QString label = m_buckets.bucketContent(group)[row];
	if (m_contacts.contains(label)) {
		return m_contacts[label];
	} else {
		return QContact();
	}
}
