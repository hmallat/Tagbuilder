/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BT_SELECTION_PAGE_EXISTING_LIST_MODEL_H_
#define _BT_SELECTION_PAGE_EXISTING_LIST_MODEL_H_

#include "BtSelectionPageListModel.h"
#include <QDBusObjectPath>

class BtSelectionPageExistingListModel : public BtSelectionPageListModel
{

	Q_OBJECT;

public:

	BtSelectionPageExistingListModel(BluezSupplicant *bluez,
					 QObject *parent = 0);

private Q_SLOTS:

	void initialized(void);

	void deviceCreated(QDBusObjectPath which);

	void deviceRemoved(QDBusObjectPath which);

	void deviceUpdated(QDBusObjectPath which);

};

#endif /* _BT_SELECTION_PAGE_EXISTING_LIST_MODEL_H_ */
