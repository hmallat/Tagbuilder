/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CONTACT_SELECTION_PAGE_LIST_MODEL_H_
#define _CONTACT_SELECTION_PAGE_LIST_MODEL_H_

#include <MAbstractItemModel>
#include <QContactManager>
#include <QContact>
#include <QContactFetchRequest>
#include <QMap>
#include <MLocaleBuckets>

QTM_USE_NAMESPACE;

class ContactSelectionPageListModel : public MAbstractItemModel
{

	Q_OBJECT;

public:

	ContactSelectionPageListModel(QContactManager *manager,
				      QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	QContact contact(const QModelIndex &index) const;

protected Q_SLOTS:

	void resultsAvailable(void);

	void stateChanged(QContactAbstractRequest::State);

protected:

	QContactManager *m_manager;

	QContactFetchRequest *m_fetch;

	QMap<QString, QContact> m_contacts;

	MLocaleBuckets m_buckets;

};

#endif /* _CONTACT_SELECTION_PAGE_LIST_MODEL_H_ */
