/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BT_SELECTION_PAGE_SCAN_LIST_MODEL_H_
#define _BT_SELECTION_PAGE_SCAN_LIST_MODEL_H_

#include "BtSelectionPageListModel.h"
#include <QDBusObjectPath>

class BtSelectionPageScanListModel : public BtSelectionPageListModel
{

	Q_OBJECT;

public:

	BtSelectionPageScanListModel(BluezSupplicant *bluez,
				     QObject *parent = 0);

	~BtSelectionPageScanListModel(void);

private slots:

	void initialized(void);

	void deviceFound(QDBusObjectPath which);

	void deviceLost(QDBusObjectPath which);

};

#endif /* _BT_SELECTION_PAGE_SCAN_LIST_MODEL_H_ */
