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

	~TagWriter(void);

	void setMessage(QNdefMessage &message);

	void writeMessage(void);

	void stopWriting(void);

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
