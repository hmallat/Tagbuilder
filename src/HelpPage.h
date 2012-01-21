/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _HELP_PAGE_H_
#define _HELP_PAGE_H_

#include <MApplicationPage>

class HelpPage : public MApplicationPage
{
	Q_OBJECT;

public:

	HelpPage(QGraphicsItem *parent = 0);

	virtual ~HelpPage(void);

        virtual void createContent(void);

private:

	Q_DISABLE_COPY(HelpPage);

	void createActions(void);

};

#endif /* _HELP_PAGE_H_ */
