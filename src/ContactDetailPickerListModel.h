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

#ifndef _CONTACT_DETAIL_PICKER_LIST_MODEL_H_
#define _CONTACT_DETAIL_PICKER_LIST_MODEL_H_

#include <MAbstractItemModel>

#include "Util.h"

#include <QContact>
#include <QContactDetail>
#include <QMap>
#include <QVector>

QTM_USE_NAMESPACE;

class ContactDetailPickerListModel : public MAbstractItemModel
{

	Q_OBJECT;

public:

	ContactDetailPickerListModel(Util::ContactDetails details,
				     QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	const QContactDetail detail(const QModelIndex &index) const;

	void setContact(const QContact &contact);

private:

	Util::ContactDetails m_filter;

	QList<enum Util::ContactDetail> m_types;

	QMap<enum Util::ContactDetail, QList<QContactDetail> > m_details;

};

#endif /* _CONTACT_DETAIL_PICKER_LIST_MODEL_H_ */
