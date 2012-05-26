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

#ifndef _HELP_PAGE_H_
#define _HELP_PAGE_H_

#include <MApplicationPage>

class HelpPage : public MApplicationPage
{
	Q_OBJECT;

public:

	HelpPage(QGraphicsItem *parent = 0);

	virtual ~HelpPage(void);

        virtual void createContent(void);

private Q_SLOTS:

	void linkActivated(const QString &link);

private:

	Q_DISABLE_COPY(HelpPage);

	void createActions(void);

};

#endif /* _HELP_PAGE_H_ */
