/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CalendarDetailCell.h"

#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MDebug>

CalendarDetailCell::CalendarDetailCell(QGraphicsItem *parent)
	: MListItem(parent),
	  m_layout(0),
	  m_titleLabel(0),
	  m_subtitleLabel(0)
{
	setStyleName("CommonBasicListItem");
	createLayout();
}

CalendarDetailCell::~CalendarDetailCell(void)
{
}

void CalendarDetailCell::createLayout(void)
{
	m_layout = new MLayout(this);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSizePolicy(QSizePolicy::Expanding,
				QSizePolicy::Expanding);

	MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(m_layout,
							      Qt::Vertical);
	policy->insertItem(0, titleLabelWidget(), Qt::AlignLeft);
	policy->insertItem(1, subtitleLabelWidget(), Qt::AlignLeft);
	m_layout->setPolicy(policy);
}

MLabel *CalendarDetailCell::titleLabelWidget(void)
{
	if (m_titleLabel == 0) {
		m_titleLabel = new MLabel(this);
		m_titleLabel->setTextElide(true);
		m_titleLabel->setStyleName("CommonTitle");
	}

	return m_titleLabel;
}

MLabel *CalendarDetailCell::subtitleLabelWidget(void)
{
	if (m_subtitleLabel == 0) {
		m_subtitleLabel = new MLabel(this);
		m_subtitleLabel->setTextElide(true);
		m_subtitleLabel->setStyleName("CommonSubTitle");
	}

	return m_subtitleLabel;
}

QString CalendarDetailCell::title(void)
{
	return titleLabelWidget()->text();
}

void CalendarDetailCell::setTitle(const QString &title)
{
	titleLabelWidget()->setText(title);
}

QString CalendarDetailCell::subtitle(void)
{
	return subtitleLabelWidget()->text();
}

void CalendarDetailCell::setSubtitle(const QString &subtitle)
{
	subtitleLabelWidget()->setText(subtitle);
}
