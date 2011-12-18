/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ContactDetailCell.h"

#include <MLabel>
#include <MLayout>
#include <MLinearLayoutPolicy>

ContactDetailCell::ContactDetailCell(QGraphicsItem *parent)
	: MListItem(parent),
	  m_layout(0),
	  m_titleLabel(0),
	  m_subtitleLabel(0)
{
	setStyleName(inv("CommonPanel"));
}

ContactDetailCell::~ContactDetailCell(void)
{
}

MLayout *ContactDetailCell::createLayout(void)
{
	m_layout = new MLayout(this);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSizePolicy(QSizePolicy::Expanding,
				QSizePolicy::Expanding);

	MLinearLayoutPolicy *policy = new MLinearLayoutPolicy(m_layout,
							      Qt::Vertical);
	policy->addItem(titleLabelWidget(), 
			Qt::AlignLeft | Qt::AlignVCenter);
	policy->addItem(subtitleLabelWidget(), 
			Qt::AlignLeft | Qt::AlignVCenter);
	m_layout->setPolicy(policy);

	return m_layout;
}

MLabel *ContactDetailCell::titleLabelWidget(void)
{
	if (m_titleLabel == 0) {
		m_titleLabel = new MLabel(this);
		m_titleLabel->setTextElide(true);
		m_titleLabel->setStyleName(inv("CommonTitle"));
	}

	return m_titleLabel;
}

MLabel *ContactDetailCell::subtitleLabelWidget(void)
{
	if (m_subtitleLabel == 0) {
		m_subtitleLabel = new MLabel(this);
		m_subtitleLabel->setTextElide(true);
		m_subtitleLabel->setStyleName(inv("CommonSubTitle"));
	}

	return m_subtitleLabel;
}

QString ContactDetailCell::inv(QString stylename)
{
	return stylename.append("Inverted");
}

QString ContactDetailCell::title(void)
{
	return titleLabelWidget()->text();
}

void ContactDetailCell::setTitle(const QString &title)
{
	titleLabelWidget()->setText(title);
}

QString ContactDetailCell::subtitle(void)
{
	return subtitleLabelWidget()->text();
}

void ContactDetailCell::setSubtitle(const QString &subtitle)
{
	subtitleLabelWidget()->setText(subtitle);
}

