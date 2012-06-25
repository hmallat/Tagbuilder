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

#ifndef _FOURSQUARE_AUTH_STORAGE_H_
#define _FOURSQUARE_AUTH_STORAGE_H_

#include <QObject>

class QString;

class FoursquareAuthStorage : public QObject
{

	class FoursquareAuthStorageImpl;

public:

	static FoursquareAuthStorage *storage(void);

	bool isEmpty(void) const;

	bool clear(void);

	const QString get(void);

	bool set(QString auth);

private:

	Q_DISABLE_COPY(FoursquareAuthStorage);

	FoursquareAuthStorage(FoursquareAuthStorageImpl *impl);

	~FoursquareAuthStorage(void);

	FoursquareAuthStorageImpl *m_impl;

};

#endif /* _FOURSQUARE_AUTH_STORAGE_H_ */
