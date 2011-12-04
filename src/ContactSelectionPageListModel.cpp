/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactSelectionPageListModel.h"

#include <QStringList>

ContactSelectionPageListModel::
ContactSelectionPageListModel(QContactManager *manager,
			      QObject *parent)
	: QAbstractListModel(parent),
	  m_manager(manager),
	  m_contacts()
{
}

int ContactSelectionPageListModel::rowCount(const QModelIndex &parent) const
{
	(void) parent;
	return m_contacts.length();
}

QVariant ContactSelectionPageListModel::data(const QModelIndex &index, 
					     int role) const
{
	(void) index, (void) role;

	/* TODO */
	QStringList parameters;
	parameters << "Foo Bar";
	return qVariantFromValue(parameters);
}

const QString ContactSelectionPageListModel::name(const QModelIndex &index) const
{
	(void)index;
	return "Foo Bar"; /* TODO */
}

const QString ContactSelectionPageListModel::icon(const QModelIndex &index) const
{
	(void)index;
	return "icon-m-content-avatar-placeholder" /* TODO */;
}

QContact ContactSelectionPageListModel::contact(const QModelIndex &index) const
{
	return m_contacts[index.row()];
}
