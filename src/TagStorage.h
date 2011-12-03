/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_STORAGE_H_
#define _TAG_STORAGE_H_

#include <QNdefMessage>

class Tag;
class QString;

QTM_USE_NAMESPACE;

class TagStorage
{

public:

	static int count(void);

	static const Tag *tag(int which);

	static bool append(const QString &name,
			   const QNdefMessage &message);

	static bool update(int which, 
			   const QString &name,
			   const QNdefMessage &message);

	static bool remove(int which);

};

#endif /* _TAG_STORAGE_H_ */
