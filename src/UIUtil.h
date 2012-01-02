/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _UI_UTIL_H_
#define _UI_UTIL_H_

class QString;
class MWidgetController;

class UIUtil
{

public:

	static MWidgetController *viewHeader(const QString title);

};

#endif /* UI_UTIL_H */
