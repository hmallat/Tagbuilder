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

#ifndef _CONTACT_SELECTION_PAGE_H_
#define _CONTACT_SELECTION_PAGE_H_

#include "SelectionPage.h"

#include "Util.h"
#include <QContactManager>
#include <QContact>

QTM_USE_NAMESPACE;

class ContactSelectionPageListModel;

class ContactSelectionPage : public SelectionPage
{
	Q_OBJECT;

public:

	ContactSelectionPage(QContactManager *manager,
			     Util::ContactDetails requiredDetails = 
			     Util::NoContactDetails,
			     QGraphicsItem *parent = 0);

	virtual ~ContactSelectionPage(void);

        virtual void createContent(void);

signals:

	void selected(const QContact which);

private Q_SLOTS:
	
	void itemsReady(void);

	void contactSelected(const QModelIndex &which);

private:

	Q_DISABLE_COPY(ContactSelectionPage);

	ContactSelectionPageListModel *m_model;

};

#endif /* _CONTACT_SELECTION_PAGE_H_ */
