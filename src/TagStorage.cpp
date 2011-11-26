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

	int count(void);

	const Tag *at(int which);

	bool append(Tag *tag);

	bool update(int which, Tag *tag);

	bool remove(int which);

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

int TagStorageImpl::count(void)
{
	return tags.length();
}

const Tag* TagStorageImpl::at(int which)
{
	return tags[which];
}

bool TagStorageImpl::append(Tag *tag)
{
	/* should also add to persistent storage */
	tags << tag;
	return true;
}

bool TagStorageImpl::update(int which, Tag *tag)
{
	/* should also update persistent storage */
	Tag *old = tags[which];
	tags[which] = tag;
	delete old;
	return true;
}

bool TagStorageImpl::remove(int which)
{
	/* should also remove from persistent storage */
	Tag *old = tags[which];
	tags.removeAt(which);
	delete old;
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

int TagStorage::count(void)
{
	return storage()->count();
}

const Tag *TagStorage::tag(int which)
{
	return storage()->at(which);
}

bool TagStorage::append(const QString &name, 
			const QtMobility::QNdefMessage &message)
{
	Tag *tag = new Tag(name, message);
	return storage()->append(tag);
}

bool TagStorage::update(int which, 
			const QString &name,
			const QtMobility::QNdefMessage &message)
{
	Tag *tag = new Tag(name, message);
	return storage()->update(which, tag);
}

bool TagStorage::remove(int which)
{
	return storage()->remove(which);
}


