/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
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

private:

	Q_DISABLE_COPY(BtSelectionPage);

	BtSelectionPageListModel *m_model;

};

#endif /* _BT_SELECTION_PAGE_H_ */
