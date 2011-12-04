/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CONTACT_SELECTION_PAGE_LIST_MODEL_H_
#define _CONTACT_SELECTION_PAGE_LIST_MODEL_H_

#include <QAbstractListModel>
#include <QContactManager>
#include <QContact>
#include <QList>

QTM_USE_NAMESPACE;

class ContactSelectionPageListModel : public QAbstractListModel
{

	Q_OBJECT;

public:

	ContactSelectionPageListModel(QContactManager *manager,
				      QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	const QString name(const QModelIndex &index) const;

	const QString icon(const QModelIndex &index) const;

	QContact contact(const QModelIndex &index) const;

protected:

	QContactManager *m_manager;

	QList<QContact> m_contacts;

};

#endif /* _CONTACT_SELECTION_PAGE_LIST_MODEL_H_ */
