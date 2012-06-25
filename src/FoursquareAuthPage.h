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

#ifndef _FOURSQUARE_AUTH_PAGE_H_
#define _FOURSQUARE_AUTH_PAGE_H_

#include <MApplicationPage>

class QWebView;
class ViewHeader;

class FoursquareAuthPage : public MApplicationPage
{

	Q_OBJECT;

public:

	FoursquareAuthPage(QGraphicsItem *parent = 0);

	virtual ~FoursquareAuthPage(void);

        virtual void createContent(void);

Q_SIGNALS:

	void created(void);

	void authenticationComplete(const QString auth);

	void authenticationFailed(void);

private Q_SLOTS:

	void activate(void);

private:

	Q_DISABLE_COPY(FoursquareAuthPage);

	ViewHeader *m_header;

	QWebView *m_view;

};

#endif /* _FOURSQUARE_AUTH_PAGE_H_ */
