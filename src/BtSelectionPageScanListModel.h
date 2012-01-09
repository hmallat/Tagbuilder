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
