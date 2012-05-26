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

#ifndef _CONTACT_DETAIL_PICKER_H_
#define _CONTACT_DETAIL_PICKER_H_

#include "SelectionPage.h"

#include "Util.h"
#include <QContact>

QTM_USE_NAMESPACE;

class ContactDetailPickerListModel;
class LabelOrList;
class MAction;
class QGraphicsLinearLayout;
class QModelIndex;

class ContactDetailPicker : public SelectionPage
{

	Q_OBJECT;

public:

	ContactDetailPicker(const QContact contact, 
			    Util::ContactDetails details = Util::AllContactDetails,
			    bool singleDetailOnly = false,
			    QGraphicsItem *parent = 0);

	virtual ~ContactDetailPicker(void);

        virtual void createContent(void);

Q_SIGNALS:

	void contactPicked(const QContact contact);

private Q_SLOTS:

	void pickingDone(void);

	void detailClicked(const QModelIndex which);

private:

	Q_DISABLE_COPY(ContactDetailPicker);

	QContact m_contact;

	ContactDetailPickerListModel *m_model;

	bool m_single;

};

#endif /* _CONTACT_DETAIL_PICKER_H_ */
