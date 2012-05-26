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

#ifndef _BT_SELECTION_PAGE_H_
#define _BT_SELECTION_PAGE_H_

#include "SelectionPage.h"

#include <QBluetoothDeviceInfo>

QTM_USE_NAMESPACE;

class BluezSupplicant;
class BtSelectionPageListModel;
class QModelIndex;

class BtSelectionPage : public SelectionPage
{
	Q_OBJECT;

public:

	enum Type {
		SelectFromExisting,
		SelectFromScanned
	};

	BtSelectionPage(BluezSupplicant *bluez,
			enum Type type, 
			QGraphicsItem *parent = 0);

	virtual ~BtSelectionPage(void);

        virtual void createContent(void);

signals:

	void selected(const QBluetoothDeviceInfo which);

private Q_SLOTS:
	
	void deviceSelected(const QModelIndex &which);

	void scanFailure(void);

private:

	Q_DISABLE_COPY(BtSelectionPage);

	enum Type m_type;

	BtSelectionPageListModel *m_model;

};

#endif /* _BT_SELECTION_PAGE_H_ */
