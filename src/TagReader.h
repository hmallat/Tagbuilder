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
