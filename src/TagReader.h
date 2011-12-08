/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_READER_H_
#define _TAG_READER_H_

#include <QObject>
#include <QNdefMessage>
#include <QNearFieldManager>
#include <QNearFieldTarget>

QTM_USE_NAMESPACE;

class TagReader : public QObject
{
  
	Q_OBJECT;

public:

	TagReader(QObject *parent);

	~TagReader(void);

	void start(void);

	void stop(void);

Q_SIGNALS:

	void messageRead(const QNdefMessage contents);

private Q_SLOTS:

	void targetDetected(QNearFieldTarget *target);

	void targetLost(QNearFieldTarget *target);

	void targetRead(const QNdefMessage &message);

	void readError(QNearFieldTarget::Error error, 
		       QNearFieldTarget::RequestId id);

private:

	Q_DISABLE_COPY(TagReader);

	QNearFieldManager *m_manager;

	QNearFieldTarget *m_target;

	bool m_started;

};

#endif /* _TAG_READER_H_ */
