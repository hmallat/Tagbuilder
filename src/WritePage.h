/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _WRITE_PAGE_H_
#define _WRITE_PAGE_H_

#include <MApplicationPage>
#include <QNdefMessage>

class MAction;
class MProgressIndicator;
class TagWriter;

QTM_USE_NAMESPACE;

class WritePage : public MApplicationPage
{
	Q_OBJECT;

public:

	WritePage(QNdefMessage message, QGraphicsItem *parent = 0);

	virtual ~WritePage(void);

        virtual void createContent(void);

private Q_SLOTS:

	void writeStarted(void);

	void writeFinished(bool success);

private:

	Q_DISABLE_COPY(WritePage);

	TagWriter *m_writer;

	MProgressIndicator *m_indicator;

	MAction *m_done;

	int m_datalen;

};

#endif /* _WRITE_PAGE_H_ */
