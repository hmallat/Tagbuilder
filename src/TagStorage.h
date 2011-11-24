/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_STORAGE_H_
#define _TAG_STORAGE_H_

#include <QList>

class Tag;

class TagStorage
{

public:

	static const QList<Tag *> storedTags(void);

	static bool append(Tag *tag);

	static bool update(Tag *tag);

};

#endif /* _TAG_STORAGE_H_ */
