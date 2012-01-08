/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _UNKNOWN_RECORD_LIST_MODEL_H_
#define _UNKNOWN_RECORD_LIST_MODEL_H_

#include <MAbstractItemModel>

#include "Util.h"
#include <QNdefMessage>

QTM_USE_NAMESPACE;

class UnknownRecordListModel : public MAbstractItemModel
{

	Q_OBJECT;

public:

	UnknownRecordListModel(QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	void setMessage(const QNdefMessage &message);

private:

	QNdefMessage m_message;

};

#endif /* _UNKNOWN_RECORD_LIST_MODEL_H_ */
