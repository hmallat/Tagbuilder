/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "UIUtil.h"

#include <QString>
#include <MWidgetController>
#include <MLabel>

MWidgetController *UIUtil::viewHeader(const QString title)
{
	MWidgetController *header = new MWidgetController();
	header->setStyleName("CommonHeaderPanel");
	header->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	MLabel *label = new MLabel(title, header);
	label->setStyleName("CommonHeader");
	label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	return header;
}

