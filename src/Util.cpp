/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "Util.h"

quint32 Util::messageLength(const QNdefMessage message)
{
	quint32 total = 0;
	for (int i = 0; i < message.length(); i++) {
		const QNdefRecord record = message[i];
		total += 
			1 + /* NDEF header */
			1 + /* Type length */
			(record.payload().length() < 256 ? 1 : 4) + /* Pllen */
			record.type().length() + /* type */
			record.payload().length(); /* pl */
	}

	return total;
}

