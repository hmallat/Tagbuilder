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

TagWriter::~TagWriter(void)
{
	stopWriting();
}

void TagWriter::setMessage(QNdefMessage &message)
{
	m_message = message;
}

void TagWriter::stopWriting(void)
{
        mDebug(__func__) << "Stopping detecting targets. ";
        m_manager->setTargetAccessModes(0);
        m_manager->stopTargetDetection();
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
	/* Keep going, just inform the UI */
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
        mDebug(__func__) << "Saw target (W). ";
	if (target->type() == QNearFieldTarget::NfcForumDevice) {
		mDebug(__func__) << "Target is a device, not writing. ";
		return;
	}

	m_target = target;

	connect(m_target, SIGNAL(ndefMessagesWritten(void)),
		this, SLOT(targetWritten(void)));
	connect(m_target, SIGNAL(error(QNearFieldTarget::Error, 
				       QNearFieldTarget::RequestId)),
		this, SLOT(writeError(QNearFieldTarget::Error, 
				      QNearFieldTarget::RequestId)));
	
	Q_EMIT(started());

	writeToDetectedTarget();
}

void TagWriter::targetLost(QNearFieldTarget *target)
{
        mDebug(__func__) << "Lost target (W). ";
	if (target == m_target) {
		disconnect(m_target, SIGNAL(ndefMessagesWritten(void)),
			   this, SLOT(targetWritten(void)));
		disconnect(m_target, SIGNAL(error(QNearFieldTarget::Error, 
						  QNearFieldTarget::RequestId)),
			   this, SLOT(writeError(QNearFieldTarget::Error, 
						 QNearFieldTarget::RequestId)));
		
		m_target = NULL;
	}
}
