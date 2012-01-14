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
class MLabel;
class NfcdMonitor;

QTM_USE_NAMESPACE;

class WritePage : public MApplicationPage
{
	Q_OBJECT;

public:

	WritePage(QNdefMessage message, 
		  NfcdMonitor *monitor,
		  QGraphicsItem *parent = 0);

	virtual ~WritePage(void);

        virtual void createContent(void);

private Q_SLOTS:

	void writeStarted(void);

	void writeFinished(bool success);

	void updateInfo(void);

private:

	Q_DISABLE_COPY(WritePage);

	void createActions(void);

	TagWriter *m_writer;

	MProgressIndicator *m_indicator;

	MAction *m_done;

	int m_datalen;

	NfcdMonitor *m_monitor;

	MLabel *m_info;

};

#endif /* _WRITE_PAGE_H_ */
