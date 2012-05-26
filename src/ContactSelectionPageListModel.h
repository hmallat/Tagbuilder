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

#ifndef _CONTACT_SELECTION_PAGE_LIST_MODEL_H_
#define _CONTACT_SELECTION_PAGE_LIST_MODEL_H_

#include <MAbstractItemModel>

#include "Util.h"
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
				      Util::ContactDetails requiredDetails =
				      Util::NoContactDetails,
				      QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	QContact contact(const QModelIndex &index) const;

	bool fetch(void);

Q_SIGNALS:

	void ready(void);

protected Q_SLOTS:

	void resultsAvailable(void);

	void stateChanged(QContactAbstractRequest::State);

protected:

	bool m_fetchDone;

	QContactManager *m_manager;

	QContactFetchRequest *m_fetch;

	QMap<QString, QContact> m_contacts;

	MLocaleBuckets m_buckets;

	Util::ContactDetails m_requiredDetails;

};

#endif /* _CONTACT_SELECTION_PAGE_LIST_MODEL_H_ */
