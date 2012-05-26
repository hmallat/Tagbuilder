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
		m_progress->setStyleName("CommonViewHeaderSpinnerInverted");
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

void ViewHeader::setBusy(void)
{
	progressWidget()->setVisible(true);
	progressWidget()->setUnknownDuration(true);
}

void ViewHeader::clearBusy(void)
{
	progressWidget()->setVisible(false);
	progressWidget()->setUnknownDuration(false);
}

