/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TagStorage.h"
#include "Tag.h"

#include <QDesktopServices>
#include <QFile>
#include <QList>

/*
 * TagStorage is simply a file which is read and written using QDataStream.
 * 
 * First object to read is a qint8 indicating version (0x10 for 1.0). 
 *
 * Each tag has three objects: QString for ID, QDateTime for creation time
 * and QByteArray for the contents. 
 */

#define CURRENT_VERSION 0x10

class TagStorageImpl
{

public:

	TagStorageImpl(void);

	~TagStorageImpl(void);

	const QList<Tag *> storedTags(void);

	bool append(Tag *tag);

private:

	QList<Tag *> tags;

};

TagStorageImpl::TagStorageImpl(void)
{
}

TagStorageImpl::~TagStorageImpl(void)
{
	while (tags.isEmpty() == false) {
		delete tags.takeFirst();
	}
}

const QList<Tag *> TagStorageImpl::storedTags(void)
{
	return tags;
}

bool TagStorageImpl::append(Tag *tag)
{
	tags << tag;
	return true;
}

static TagStorageImpl *singleton = NULL;

static TagStorageImpl *storage(void)
{
	if (singleton != NULL) {
		return singleton;
	}

#if 0 /* TODO complete */
	QString location = QDesktopServices::storageLocation
		(QDesktopServices::DataLocation) +
		QCoreApplication::organizationName() + "/" +
		QCoreApplication::applicationName();

	if (QFile::exists(location) == false) {
		QDir dir;
		dir.mkpath(location);
	}

	QFile file(directory + "/" + "tag-creator-storage.bin");
	if (file.open(QIODecice::ReadWrite) == false) {
		return NULL;
	}

	QDataStream in(file);
	quint8 version = ;
#else
	singleton = new TagStorageImpl();
	return singleton;
#endif
}

const QList<Tag *> TagStorage::storedTags(void)
{
	return storage()->storedTags();
}

bool TagStorage::append(Tag *tag)
{
	return storage()->append(tag);
}
