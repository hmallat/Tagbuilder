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

#include "VCardNdefRecord.h"

VCardNdefRecord VCardNdefRecord::fromSupportedMimeType(const QNdefRecord &other)
{
	VCardNdefRecord r;
	if (hasSupportedMimeType(other)) {
		r.setPayload(other.payload());
	}
	return r;
}

bool VCardNdefRecord::hasSupportedMimeType(const QNdefRecord &other)
{
	return (other.typeNameFormat() == QNdefRecord::Mime &&
		(other.type() == "text/vcard" ||
		 other.type() == "text/x-vcard" ||
		 other.type() == "text/x-vCard" ||
		 other.type() == "text/directory" ||
		 other.type() == "text/directory;profile=vCard"));
}

				       
