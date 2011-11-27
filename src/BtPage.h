/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _BT_PAGE_H
#define _BT_PAGE_H_

#include <MApplicationPage>

class LabeledTextEdit;
class MContentItem;

class BtPage : public MApplicationPage
{

	Q_OBJECT;

public:

	BtPage(int tag = -1, QGraphicsItem *parent = 0);

	~BtPage(void);

	virtual void createContent(void);

private Q_SLOTS:

	void nameChanged(void);

	void storeTag(void);

private:

	Q_DISABLE_COPY(BtPage);

	int m_tag;

	LabeledTextEdit *m_name;

	MContentItem *m_device;

	MAction *m_cancelAction;

	MAction *m_storeAction;

};

#endif /* _Bt_PAGE_H_ */
