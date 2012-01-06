/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "ViewHeader.h"

#include <MLabel>
#include <MProgressIndicator>
#include <QGraphicsLinearLayout>

#include <MDebug>

ViewHeader::ViewHeader(const QString text,
		       QGraphicsItem *parent)
	: MStylableWidget(parent),
	  m_text(text),
	  m_label(0),
	  m_progress(0),
	  m_layout(0)
{
	setStyleName("CommonHeaderPanel");
	setSizePolicy(QSizePolicy::Preferred,
		      QSizePolicy::Fixed);
}

void ViewHeader::setBusy(void)
{
	if (m_progress != NULL) {
		m_progress->setVisible(true);
		m_progress->setUnknownDuration(true);
	}
}

void ViewHeader::clearBusy(void)
{
	if (m_progress != NULL) {
		m_progress->setVisible(false);
		m_progress->setUnknownDuration(false);
	}
}

MLabel *ViewHeader::labelWidget(void)
{
	if (m_label == NULL) {
		m_label = new MLabel(m_text);
		m_label->setStyleName("CommonHeader");
		m_label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		m_label->setSizePolicy(QSizePolicy::Preferred, 
				       QSizePolicy::Fixed);
	}

	return m_label;
}

MProgressIndicator *ViewHeader::progressWidget(void)
{
	if (m_progress == NULL) {
		m_progress = new MProgressIndicator
			(0, MProgressIndicator::spinnerType);
		m_progress->setStyleName("CommonViewHeaderSpinner");
		m_progress->setSizePolicy(QSizePolicy::Fixed, 
					  QSizePolicy::Fixed);
		clearBusy();
	}

	return m_progress;
}

QGraphicsLayout *ViewHeader::createLayout(void)
{
	if (m_layout != 0) {
		delete m_layout;
		m_layout = 0;
	}
	
	QGraphicsLinearLayout *layout = 
		new QGraphicsLinearLayout(Qt::Horizontal);
	layout->setSizePolicy(QSizePolicy::Preferred, 
				QSizePolicy::Fixed);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	layout->addItem(labelWidget());
	layout->setAlignment(labelWidget(), 
			     Qt::AlignVCenter | Qt::AlignLeft);
		       
	layout->addItem(progressWidget());
	layout->setAlignment(progressWidget(), 
			     Qt::AlignVCenter | Qt::AlignRight);

	m_layout = layout;
	return m_layout;
}

void ViewHeader::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	MWidgetController::resizeEvent(event);
	
	if (m_layout == NULL) {
		setLayout(createLayout());
	}
}
