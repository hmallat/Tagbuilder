/*

    NFC Tag Builder for Nokia N9
    Copyright (C) 2011,2012  Hannu Mallat <hmallat@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
        mDebug(__func__) << "Saw target (R). ";
	m_target = target;

	if (m_started == false) {
		mDebug(__func__) << "Ignoring read target, for now. ";
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
        mDebug(__func__) << "Lost target (R). ";

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

