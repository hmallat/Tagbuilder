/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TagReader.h"

#include <MDebug>

TagReader::TagReader(QObject *parent) :
	QObject(parent), 
	m_manager(new QNearFieldManager(this)),
	m_target(NULL),
	m_started(false)
{
	connect(m_manager,
		SIGNAL(targetDetected(QNearFieldTarget*)),
		this,
		SLOT(targetDetected(QNearFieldTarget*)));
	connect(m_manager,
		SIGNAL(targetLost(QNearFieldTarget*)),
		this,
		SLOT(targetLost(QNearFieldTarget*)));

        mDebug(__func__) << "Start detecting targets. ";
        QList<QNearFieldTarget::Type> emptyList;
        m_manager->setTargetAccessModes(QNearFieldManager::NdefReadTargetAccess);
        m_manager->startTargetDetection(emptyList);
}

TagReader::~TagReader(void)
{
        mDebug(__func__) << "Stop detecting targets. ";
        m_manager->stopTargetDetection();
}

void TagReader::start(void)
{
	m_started = true;
}

void TagReader::stop(void)
{
	m_started = false;
}

void TagReader::targetDetected(QNearFieldTarget *target)
{
        mDebug(__func__) << "Saw target. ";
	m_target = target;

	if (m_started == false) {
		return; /* Not now dear */
	}

	connect(target, SIGNAL(ndefMessageRead(const QNdefMessage &)),
		this, SLOT(targetRead(const QNdefMessage &)));
	connect(target, SIGNAL(error(QNearFieldTarget::Error, 
				     QNearFieldTarget::RequestId)),
		this, SLOT(readError(QNearFieldTarget::Error, 
				     QNearFieldTarget::RequestId)));
	
	m_target->readNdefMessages();
}

void TagReader::targetLost(QNearFieldTarget *target)
{
        mDebug(__func__) << "Lost target. ";

	if (target == m_target) {

		disconnect(target, SIGNAL(ndefMessageRead(const QNdefMessage &)),
			   this, SLOT(targetRead(const QNdefMessage &)));
		disconnect(target, SIGNAL(error(QNearFieldTarget::Error, 
						QNearFieldTarget::RequestId)),
			   this, SLOT(readError(QNearFieldTarget::Error, 
						QNearFieldTarget::RequestId)));
		
		m_target = NULL;
	}
}

void TagReader::targetRead(const QNdefMessage &message)
{
	Q_EMIT(messageRead(message));
}

void TagReader::readError(QNearFieldTarget::Error err, 
			  QNearFieldTarget::RequestId id)
{
	(void)err, (void)id;
	mDebug(__func__) << "Read error. ";
	/* TODO: maybe-show */
}

