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

#ifndef _FOURSQUARE_VENUE_H_
#define _FOURSQUARE_VENUE_H_

#include <QString>
#include <QMap>

class FoursquareVenue
{

public:

	static const int DISTANCE_UNKNOWN;

	FoursquareVenue(void);

	FoursquareVenue(const QString &id,
			const QString &label,
			int distance = DISTANCE_UNKNOWN);

	FoursquareVenue(const FoursquareVenue &other);

	FoursquareVenue &operator=(const FoursquareVenue &other);

	~FoursquareVenue(void);

	bool isEmpty(void) const;

	const QString displayLabel(void) const;

	const QString id(void) const;

	int distance(void) const;

private:

	QString m_id;

	QString m_label;

	int m_distance;

};

Q_DECLARE_TYPEINFO(FoursquareVenue, Q_COMPLEX_TYPE);

#endif /* _FOURSQUARE_VENUE_H_ */
