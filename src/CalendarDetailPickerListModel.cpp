/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CalendarDetailPickerListModel.h"
#include "Util.h"

#include <QOrganizerEventTime>
#include <QOrganizerJournalTime>
#include <QOrganizerTodoTime>
#include <QOrganizerItemComment>
#include <QOrganizerItemDescription>
#include <QOrganizerItemDisplayLabel>
#include <QOrganizerItemLocation>

#include <MDebug>

/* TODO: maybe add more details, at least online accounts */

CalendarDetailPickerListModel::
CalendarDetailPickerListModel(const QOrganizerItem &item,
			      QObject *parent)
	: MAbstractItemModel(parent),
	  m_item(item),
	  m_types(),
	  m_details()
{
	setGrouped(true);

	QList<enum DetailType> types;
	QMap<enum DetailType, QList<QOrganizerItemDetail> > details;

	QList<QOrganizerItemDetail> labels = 
		m_item.details(QOrganizerItemDisplayLabel::DefinitionName);
	if (labels.length() != 0) {
		types << Label;
		details[Label] = labels;
	}

	QList<QOrganizerItemDetail> eventtimes = 
		m_item.details(QOrganizerEventTime::DefinitionName);
	if (eventtimes.length() != 0) {
		types << EventTime;
		details[EventTime] = eventtimes;
	}

	QList<QOrganizerItemDetail> todotimes = 
		m_item.details(QOrganizerTodoTime::DefinitionName);
	if (todotimes.length() != 0) {
		types << TodoTime;
		details[TodoTime] = todotimes;
	}

	QList<QOrganizerItemDetail> jourtimes = 
		m_item.details(QOrganizerJournalTime::DefinitionName);
	if (jourtimes.length() != 0) {
		types << JournalTime;
		details[JournalTime] = jourtimes;
	}

	QList<QOrganizerItemDetail> locs = 
		m_item.details(QOrganizerItemLocation::DefinitionName);
	if (locs.length() != 0) {
		types << Location;
		details[Location] = locs;
	}

	QList<QOrganizerItemDetail> descrs = 
		m_item.details(QOrganizerItemDescription::DefinitionName);
	if (descrs.length() != 0) {
		types << Description;
		details[Description] = descrs;
	}

	QList<QOrganizerItemDetail> comms = 
		m_item.details(QOrganizerItemComment::DefinitionName);
	if (comms.length() != 0) {
		types << Comment;
		details[Comment] = comms;
	}

	Q_EMIT(layoutAboutToBeChanged());

	if (types.length() > 0) {
		beginInsertRows(QModelIndex(), 0, types.length() - 1, false);
	}
	m_types = types;
	m_details = details;
	if (types.length() > 0) {
		endInsertRows();
	}

	Q_EMIT(layoutChanged());
}

int CalendarDetailPickerListModel::groupCount(void) const
{
	return m_types.length();
}

int CalendarDetailPickerListModel::rowCountInGroup(int group) const
{
	return m_details[m_types[group]].length();
}

QString CalendarDetailPickerListModel::groupTitle(int group) const
{
	return 
		(m_types[group] == Label) ? tr("Label") :
		(m_types[group] == Location) ? tr("Location") :
		(m_types[group] == EventTime) ? tr("Time") :
		(m_types[group] == JournalTime) ? tr("Time") :
		(m_types[group] == TodoTime) ? tr("Time") :
		(m_types[group] == Description) ? tr("Description") :
		(m_types[group] == Comment) ? tr("Comment") :
		tr("Unknown type");
}

QVariant CalendarDetailPickerListModel::itemData(int row, 
						 int group, 
						 int role) const
{
	(void)role;

	QStringList parameters;
	enum DetailType type = m_types[group];
	QOrganizerItemDetail detail = m_details[type][row];

	if (type == Label) {
		QOrganizerItemDisplayLabel label = 
			static_cast<QOrganizerItemDisplayLabel>(detail);
		parameters << label.label();
		parameters << "";

	} else if (type == Location) {
		QOrganizerItemLocation loc = 
			static_cast<QOrganizerItemLocation>(detail);
		parameters << loc.label();
		parameters << "";

	} else if (type == EventTime) {
		QOrganizerEventTime time = 
			static_cast<QOrganizerEventTime>(detail);

		QString rep;
		QDateTime s = time.startDateTime();
		QDateTime e = time.endDateTime();
		
		if (time.isAllDay() == true) {
			/* Time component is meaningless */
			rep += s.date().toString(Qt::SystemLocaleShortDate);
		} else if (e == QDateTime()) { 
			/* No end time, only start time */
			rep += s.toString(Qt::SystemLocaleShortDate);
		} else {
			rep += s.toString(Qt::SystemLocaleShortDate);
			rep += " - ";
			rep += e.toString(Qt::SystemLocaleShortDate);
		}
		
		parameters << rep;
		parameters << (time.isAllDay() ? tr("All-day event") : "");

	} else if (type == JournalTime) {
		QOrganizerJournalTime time = 
			static_cast<QOrganizerJournalTime>(detail);

		QString rep;
		QDateTime d = time.entryDateTime();
		
		rep += d.toString(Qt::SystemLocaleShortDate);
		
		parameters << rep;
		parameters << "";

	} else if (type == TodoTime) {
		QOrganizerTodoTime time = 
			static_cast<QOrganizerTodoTime>(detail);

		QString rep;
		QDateTime d = time.dueDateTime();
		
		if (time.isAllDay()) {
			rep += d.date().toString(Qt::SystemLocaleShortDate);
		} else {
			rep += d.toString(Qt::SystemLocaleShortDate);
		}
		
		parameters << rep;
		parameters << (time.isAllDay() ? tr("All-day event") : "");

	} else if (type == Description) {
		QOrganizerItemDescription description = 
			static_cast<QOrganizerItemDescription>(detail);
		parameters << description.description();
		parameters << "";

	} else if (type == Comment) {
		QOrganizerItemComment comment = 
			static_cast<QOrganizerItemComment>(detail);
		parameters << comment.comment();
		parameters << "";

	}

	return qVariantFromValue(parameters);
}

const QOrganizerItemDetail 
CalendarDetailPickerListModel::detail(const QModelIndex &index) const
{
	int row = index.row();
	int group = index.parent().row();
	enum DetailType type = m_types[group];
	return m_details[type][row];
}
