/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_TYPE_LIST_MODEL_H_
#define _TAG_TYPE_LIST_MODEL_H_

#include <QAbstractListModel>

class TagTypeListModel : public QAbstractListModel
{

	Q_OBJECT;

public:

	static const QString BLUETOOTH_TAG;

	static const QString URL_TAG;

	static const QString CALENDAR_TAG;

	static const QString CONTACT_TAG;

	static const QString TEXT_TAG;

	static const QString WLAN_TAG;

	TagTypeListModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	const QString name(const QModelIndex &index) const;

	const QString icon(const QModelIndex &index) const;

};

#endif /* _TAG_TYPE_LIST_MODEL_H_ */
