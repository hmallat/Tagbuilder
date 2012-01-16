/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CALENDAR_DETAIL_PICKER_LIST_MODEL_H_
#define _CALENDAR_DETAIL_PICKER_LIST_MODEL_H_

#include <MAbstractItemModel>

#include "Util.h"

#include <QOrganizerItem>
#include <QOrganizerItemDetail>
#include <QMap>
#include <QVector>

QTM_USE_NAMESPACE;

class CalendarDetailPickerListModel : public MAbstractItemModel
{

	Q_OBJECT;

public:

	CalendarDetailPickerListModel(QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	const QOrganizerItemDetail detail(const QModelIndex &index) const;

	void setOrganizerItem(const QOrganizerItem &item);

private:

	QList<enum Util::CalendarDetail> m_types;

	QMap<enum Util::CalendarDetail, QList<QOrganizerItemDetail> > m_details;

};

#endif /* _CALENDAR_DETAIL_PICKER_LIST_MODEL_H_ */
