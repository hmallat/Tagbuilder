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

#ifndef _CONTACT_PAGE_H
#define _CONTACT_PAGE_H_

#include "CreateEditPage.h"

#include <QContact>
#include <QContactManager>

class MLabel;
class LabelOrList;
class ContactDetailPickerListModel;

QTM_USE_NAMESPACE;

class ContactPage : public CreateEditPage
{

	Q_OBJECT;

public:

	ContactPage(int tag = TagStorage::NULL_TAG, 
		    QGraphicsItem *parent = 0);

	virtual ~ContactPage(void);

protected:

	virtual void createPageSpecificActions(void);

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void chooseFromAddressbook(void);

	void setContactThroughAction(const QContact contact);

	void setContactDetails(const QContact contact);

private:

	Q_DISABLE_COPY(ContactPage);

#ifdef LABEL_SIZE
	void updateSize(void);
#endif

	void setContact(const QContact contact);

	QContactManager m_contactManager;

	QContact m_info;

	LabelOrList *m_contactDetails;

	ContactDetailPickerListModel *m_model;

};

#endif /* _CONTACT_PAGE_H_ */
