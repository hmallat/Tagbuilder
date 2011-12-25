/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CALENDAR_DETAIL_CELL_H_
#define _CALENDAR_DETAIL_CELL_H_

#include <MListItem>

class MLabel;
class MLayout;

class CalendarDetailCell : public MListItem
{

	Q_OBJECT;

public:

	CalendarDetailCell(QGraphicsItem *parent = 0);

	virtual ~CalendarDetailCell(void);

	QString title(void);

	void setTitle(const QString &title);

	QString subtitle(void);

	void setSubtitle(const QString &subtitle);

private:

	Q_DISABLE_COPY(CalendarDetailCell);

	void createLayout(void);

	MLabel *titleLabelWidget(void);

	MLabel *subtitleLabelWidget(void);

	MLayout *m_layout;

	MLabel *m_titleLabel;

	MLabel *m_subtitleLabel;

};

#endif /* _CALENDAR_DETAIL_CELL_H_ */
