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

#ifndef _UNKNOWN_PAGE_H
#define _UNKNOWN_PAGE_H_

#include "CreateEditPage.h"

#include <QNdefMessage>

QTM_USE_NAMESPACE;

class UnknownRecordListModel;
class LabelOrList;

class UnknownPage : public CreateEditPage
{

	Q_OBJECT;

public:

	UnknownPage(int tag = TagStorage::NULL_TAG, 
		    QGraphicsItem *parent = 0);

	virtual ~UnknownPage(void);

protected:

	virtual void createPageSpecificActions(void);

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private:

	Q_DISABLE_COPY(UnknownPage);

#ifdef LABEL_SIZE
	void updateSize(void);
#endif

	QNdefMessage m_message;

	LabelOrList *m_unknownDetails;

	UnknownRecordListModel *m_model;

};

#endif /* _UNKNOWN_PAGE_H_ */
