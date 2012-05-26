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

#ifndef _VIEW_HEADER_H_
#define _VIEW_HEADER_H_

#include <MStylableWidget>

class MLabel;
class MProgressIndicator;
class QGraphicsLayout;

class ViewHeader : public MStylableWidget
{

	Q_OBJECT;

public:

	ViewHeader(const QString text, QGraphicsItem *parent = 0);

public Q_SLOTS:

	void setBusy(void);

	void clearBusy(void);

protected:

	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:

	Q_DISABLE_COPY(ViewHeader);

	QGraphicsLayout *createLayout(void);

	MLabel *labelWidget(void);

	MProgressIndicator *progressWidget(void);

	QString m_text;

	MLabel *m_label;

	MProgressIndicator *m_progress;

	QGraphicsLayout *m_layout;

};

#endif /* _VIEW_HEADER_H_ */
