/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactSelectionPageListModel.h"

#include <QContactSortOrder>
#include <QStringList>
#include <MDebug>

ContactSelectionPageListModel::
ContactSelectionPageListModel(QContactManager *manager,
			      QObject *parent)
	: MAbstractItemModel(parent),
	  m_manager(manager),
	  m_fetch(0),
	  m_contacts(),
	  m_buckets()
{
	setGrouped(true);

	/* Not really needed, but keep for reference */
	QContactSortOrder order;
	order.setBlankPolicy(QContactSortOrder::BlanksFirst);
	order.setCaseSensitivity(Qt::CaseInsensitive);
	order.setDetailDefinitionName(QContactDisplayLabel::DefinitionName,
				      QContactDisplayLabel::FieldLabel);
	order.setDirection(Qt::AscendingOrder);
	
	QList<QContactSortOrder> sorts;
	sorts << order;

	m_fetch = new QContactFetchRequest(this);
	m_fetch->setManager(manager);
	m_fetch->setSorting(sorts);

	connect(m_fetch, SIGNAL(resultsAvailable()),
		this, SLOT(resultsAvailable()));
	connect(m_fetch, SIGNAL(stateChanged(QContactAbstractRequest::State)),
		this, SLOT(stateChanged(QContactAbstractRequest::State)));

	mDebug(__func__) << "Starting fetch. ";
	if (m_fetch->start() == false) {
		mDebug(__func__) << "Cannot fetch contacts. ";
		/* TODO: indicate in UI */
	}
}

void ContactSelectionPageListModel::resultsAvailable(void)
{
	if (m_fetch->error() != QContactManager::NoError) {
		mDebug(__func__) << "Cannot fetch contacts, error " 
				 << m_fetch->error();
		/* TODO: indicate in UI */
		return;
	}

	QList<QContact> results = m_fetch->contacts();

	QStringList bucketEntries;
	QMap<QString, QContact> insertions;

	for (int i = 0; i < results.length(); i++) {
		QContact contact = results[i];
		if (contact.displayLabel() == "") {
			continue;
		}

		if (insertions.contains(contact.displayLabel())) {
			mDebug(__func__) << "Duplicate, skipping. ";
			continue;
		}

		insertions[contact.displayLabel()] = contact;
		bucketEntries << contact.displayLabel();
	}
	if (bucketEntries.length() == 0) {
		mDebug(__func__) << "Nothing to be inserted. ";
		/* TODO: indicate in UI */
		return;
	}

	Q_EMIT(layoutAboutToBeChanged());

	beginInsertRows(QModelIndex(), 0, bucketEntries.length() - 1, false);
	m_buckets.setItems(bucketEntries);
	m_contacts = insertions;
	endInsertRows();

	Q_EMIT(layoutChanged());
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