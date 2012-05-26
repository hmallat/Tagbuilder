/*

    NFC Tag Builder for Nokia N9
    Copyright (C) 2011,2012  Hannu Mallat <hmallat@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
#include <QOrganizerTodoProgress>

#include <MDebug>

/* TODO: maybe add more details, at least online accounts */

CalendarDetailPickerListModel::
CalendarDetailPickerListModel(QObject *parent)
	: MAbstractItemModel(parent),
	  m_types(),
	  m_details()
{
	setGrouped(true);
}

void CalendarDetailPickerListModel::setOrganizerItem(const QOrganizerItem &item)
{

	QList<enum Util::CalendarDetail> types;
	QMap<enum Util::CalendarDetail, QList<QOrganizerItemDetail> > details;

	Util::CalendarDetail type[CALENDAR_DETAILS] = {
		Util::Label, 
		Util::Location, 
		Util::EventTime,
		Util::JournalTime,
		Util::TodoTime,
		Util::Description,
		Util::Comment,
		Util::TodoProgress
	};

	QString label = item.displayLabel();
	for (int i = 0; i < CALENDAR_DETAILS; i++) {
		const QString name = Util::calendarDetailName(type[i]);
		QList<QOrganizerItemDetail> dets = item.details(name);
		if (dets.length() != 0) {
			mDebug(__func__) << label << "got some " << name;
			types << type[i];
			details[type[i]] = dets;
		} else {
			mDebug(__func__) << label << "has no " << name;
		}
	}

	Q_EMIT(layoutAboutToBeChanged());

	if (m_types.length() > 0) {
		beginRemoveRows(QModelIndex(), 0, m_types.length() - 1, 
				Util::animateLists);
		m_types.clear();
		m_details.clear();
		endRemoveRows();
	}

	if (types.length() > 0) {
		beginInsertRows(QModelIndex(), 0, types.length() - 1, 
				Util::animateLists);
		m_types = types;
		m_details = details;
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
	/* TODO: move translation to Util */
	return 
		(m_types[group] == Util::Label) ? tr("Label") :
		(m_types[group] == Util::Location) ? tr("Location") :
		(m_types[group] == Util::EventTime) ? tr("Time") :
		(m_types[group] == Util::JournalTime) ? tr("Time") :
		(m_types[group] == Util::TodoTime) ? tr("Due time") :
		(m_types[group] == Util::Description) ? tr("Description") :
		(m_types[group] == Util::Comment) ? tr("Comment") :
		(m_types[group] == Util::TodoProgress) ? tr("Status") :
		tr("Unknown type");
}

QVariant CalendarDetailPickerListModel::itemData(int row, 
						 int group, 
						 int role) const
{
	(void)role;

	QStringList parameters;
	enum Util::CalendarDetail type = m_types[group];
	QOrganizerItemDetail detail = m_details[type][row];

	if (type == Util::Label) {
		QOrganizerItemDisplayLabel label = 
			static_cast<QOrganizerItemDisplayLabel>(detail);
		parameters << label.label();
		parameters << "";

	} else if (type == Util::Location) {
		QOrganizerItemLocation loc = 
			static_cast<QOrganizerItemLocation>(detail);
		parameters << loc.label();
		parameters << "";

	} else if (type == Util::EventTime) {
		QOrganizerEventTime time = 
			static_cast<QOrganizerEventTime>(detail);

		QString rep;
		QDateTime s = time.startDateTime();
		QDateTime e = time.endDateTime();
		
		if (time.isAllDay() == true) {
			/* Time component is meaningless */
			rep += s.date().toString(Qt::SystemLocaleShortDate);
		} else {
			rep += Util::eventTimeToString(s, e, true);
		}
		
		parameters << rep;
		parameters << (time.isAllDay() 
			       ? tr("All-day event") 
			       : Util::eventDurationToString(s, e));

	} else if (type == Util::JournalTime) {
		QOrganizerJournalTime time = 
			static_cast<QOrganizerJournalTime>(detail);

		QString rep;
		QDateTime d = time.entryDateTime();
		
		rep += d.toString(Qt::SystemLocaleShortDate);
		
		parameters << rep;
		parameters << "";

	} else if (type == Util::TodoTime) {
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
		parameters << (time.isAllDay() ? tr("All-day item") : "");

	} else if (type == Util::Description) {
		QOrganizerItemDescription description = 
			static_cast<QOrganizerItemDescription>(detail);
		parameters << description.description();
		parameters << "";

	} else if (type == Util::Comment) {
		QOrganizerItemComment comment = 
			static_cast<QOrganizerItemComment>(detail);
		parameters << comment.comment();
		parameters << "";

	} else if (type == Util::TodoProgress) {
		QOrganizerTodoProgress prog = 
			static_cast<QOrganizerTodoProgress>(detail);
		QString rep;
		QDateTime f;

		switch (prog.status()) {

		case QOrganizerTodoProgress::StatusNotStarted:
			rep = tr("Not started");
			break;

		case QOrganizerTodoProgress::StatusComplete:
			f = prog.finishedDateTime();
			if (f.isValid() == true) {
				rep = tr("Finished %1").arg(Util::eventTimeToString(f, QDateTime()));
			} else {
				rep = tr("Finished");
			}
			break;

		default:
			rep = tr("In progress");
		}

		parameters << rep;
		parameters << "";
	}

	return qVariantFromValue(parameters);
}

const QOrganizerItemDetail 
CalendarDetailPickerListModel::detail(const QModelIndex &index) const
{
	int row = index.row();
	int group = index.parent().row();
	enum Util::CalendarDetail type = m_types[group];
	return m_details[type][row];
}
