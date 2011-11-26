/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_LIST_MODEL_H_
#define _TAG_LIST_MODEL_H_

#include <QAbstractListModel>

class TagListModel : public QAbstractListModel
{

	Q_OBJECT;

public:

	TagListModel(QObject *parent = 0);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

};

#endif /* _TAG_LIST_MODEL_H_ */
