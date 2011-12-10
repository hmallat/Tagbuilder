/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
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

				       
