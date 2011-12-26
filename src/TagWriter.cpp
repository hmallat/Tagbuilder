/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TagWriter.h"

#include <MDebug>
#include <QFile>

TagWriter::TagWriter(QObject *parent) :
	QObject(parent), 
	m_manager(new QNearFieldManager(this)),
	m_target(NULL)
{
        connect(m_manager,
                SIGNAL(targetDetected(QNearFieldTarget*)),
                this,
                SLOT(targetDetected(QNearFieldTarget*)));
        connect(m_manager,
                SIGNAL(targetLost(QNearFieldTarget*)),
                this,
                SLOT(targetLost(QNearFieldTarget*)));
}

void TagWriter::setMessage(QNdefMessage &message)
{
	m_message = message;
}

void TagWriter::writeMessage(void)
{
        mDebug(__func__) << "Detecting targets. ";
        QList<QNearFieldTarget::Type> emptyList;
        m_manager->setTargetAccessModes(QNearFieldManager::NdefWriteTargetAccess);
        m_manager->startTargetDetection(emptyList);
}

void TagWriter::writingComplete(bool success)
{
        mDebug(__func__) << "Stopping detecting targets. ";
        m_manager->setTargetAccessModes(0);
        m_manager->stopTargetDetection();
	Q_EMIT(complete(success));
}

void TagWriter::writeToDetectedTarget(void)
{
	QList<QNdefMessage> wrapper;
	wrapper.append(m_message);
	m_target->writeNdefMessages(wrapper);
}

void TagWriter::targetWritten(void)
{
	/* Wrote a message succesfully, that's it then */
        mDebug(__func__) << "Done writing. ";
	writingComplete(true);
}

void TagWriter::writeError(QNearFieldTarget::Error err, 
			   QNearFieldTarget::RequestId id)
{
	(void)err, (void)id;
	mDebug(__func__) << "Write error. ";
	writingComplete(false);
}

void TagWriter::targetDetected(QNearFieldTarget *target)
{
        mDebug(__func__) << "Saw target. ";
	m_target = target;

	connect(target, SIGNAL(ndefMessagesWritten(void)),
		this, SLOT(targetWritten(void)));
	connect(target, SIGNAL(error(QNearFieldTarget::Error, 
				     QNearFieldTarget::RequestId)),
		this, SLOT(writeError(QNearFieldTarget::Error, 
				      QNearFieldTarget::RequestId)));
	
	Q_EMIT(started());

	writeToDetectedTarget();
}

void TagWriter::targetLost(QNearFieldTarget *target)
{
        mDebug(__func__) << "Lost target. ";
	if (target == m_target) {
		m_target = NULL;
	}
}
