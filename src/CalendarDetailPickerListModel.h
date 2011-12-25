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
#include <QOrganizerItem>
#include <QOrganizerItemDetail>
#include <QMap>
#include <QVector>

QTM_USE_NAMESPACE;

class CalendarDetailPickerListModel : public MAbstractItemModel
{

	Q_OBJECT;

	enum DetailType {
		Label,
		Location,
		EventTime,
		JournalTime,
		TodoTime,
		Description,
		Comment
	};

public:

	CalendarDetailPickerListModel(const QOrganizerItem &item,
				     QObject *parent = 0);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

	const QOrganizerItemDetail detail(const QModelIndex &index) const;

private:

	QOrganizerItem m_item;

	QList<enum DetailType> m_types;

	QMap<enum DetailType, QList<QOrganizerItemDetail> > m_details;

};

#endif /* _CALENDAR_DETAIL_PICKER_LIST_MODEL_H_ */
