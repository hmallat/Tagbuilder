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
