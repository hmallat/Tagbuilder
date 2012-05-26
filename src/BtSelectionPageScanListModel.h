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

#ifndef _BT_SELECTION_PAGE_SCAN_LIST_MODEL_H_
#define _BT_SELECTION_PAGE_SCAN_LIST_MODEL_H_

#include "BtSelectionPageListModel.h"

class BtSelectionPageScanListModel : public BtSelectionPageListModel
{

	Q_OBJECT;

public:

	BtSelectionPageScanListModel(BluezSupplicant *bluez,
				     QObject *parent = 0);

	~BtSelectionPageScanListModel(void);

Q_SIGNALS:

	void scanFailure(void);

private Q_SLOTS:

	void initialized(void);

	void deviceFound(QString which);

	void deviceLost(QString which);

	void discoveryStateChanged(enum BluezSupplicant::DiscoveryState);

};

#endif /* _BT_SELECTION_PAGE_SCAN_LIST_MODEL_H_ */
