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

#include "FoursquareVenue.h"

FoursquareVenue::FoursquareVenue(void) :
	m_id(""),
	m_label("")
{
}

FoursquareVenue::FoursquareVenue(const QString &id,
				 const QString &label) :
	m_id(id),
	m_label(label)
{
}

FoursquareVenue::FoursquareVenue(const FoursquareVenue &other) :
	m_id(other.m_id),
	m_label(other.m_label)
{
}

FoursquareVenue &FoursquareVenue::operator=(const FoursquareVenue &other)
{
	if (this != &other) {
		m_id = other.m_id;
		m_label = other.m_label;
	}

	return *this;
}

FoursquareVenue::~FoursquareVenue(void)
{
}

bool FoursquareVenue::isEmpty(void) const
{
	return m_id == "";
}

const QString FoursquareVenue::displayLabel(void) const
{
	return m_label;
}

const QString FoursquareVenue::id(void) const
{
	return m_id;
}
