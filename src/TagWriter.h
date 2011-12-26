/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_WRITER_H_
#define _TAG_WRITER_H_

#include <QObject>
#include <QString>
#include <QStringList>
#include <QNearFieldTarget>
#include <QNdefMessage>
#include <QNearFieldManager>

QTM_USE_NAMESPACE;

class TagWriter : public QObject
{
        Q_OBJECT;

public:

	TagWriter(QObject *parent = NULL);

	void setMessage(QNdefMessage &message);

	void writeMessage(void);

protected Q_SLOTS:

        void targetDetected(QNearFieldTarget *target);

        void targetLost(QNearFieldTarget *target);

        void targetWritten(void);

	void writeError(QNearFieldTarget::Error error, 
			QNearFieldTarget::RequestId id);

Q_SIGNALS:

	void started(void);

	void complete(bool success);

private:

	Q_DISABLE_COPY(TagWriter);

	void writeToDetectedTarget(void);

	void writingComplete(bool success);

        QNearFieldManager *m_manager;

	QNearFieldTarget *m_target;

	QNdefMessage m_message;

};

#endif /* _TAG_WRITER_H_ */
