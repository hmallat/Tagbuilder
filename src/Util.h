/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _UTIL_H_
#define _UTIL_H_

#include <QNdefMessage>

QTM_USE_NAMESPACE;

class Util
{

public:

	static quint32 messageLength(const QNdefMessage message);

};

#endif /* UTIL_H */
