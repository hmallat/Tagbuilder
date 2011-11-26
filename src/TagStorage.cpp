/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TagStorage.h"
#include "Tag.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QList>

/*
 * TagStorage is simply a file which is read and written using QDataStream.
 * 
 * First object to read is a qint8 indicating version (0x10 for 1.0). 
 *
 * Each tag has three fields: UTF-8 byte array for name, byte array
 * for contents, and a timestamp for creation time.
 */

#define CURRENT_VERSION 0x10

class TagStorageImpl
{

public:

	TagStorageImpl(QList<Tag *> tags);

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

TagStorageImpl::TagStorageImpl(QList<Tag *> readTags)
	: tags(readTags)
{
}

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

static bool _readStorage(QFile &file, QList<Tag *> &contents)
{
	while (contents.isEmpty() == false) {
		delete contents.takeFirst();
	}

	if (file.open(QIODevice::ReadWrite) == false) {
		return false;
	}

	QByteArray data = file.readAll();
	char *ptr = data.data();
	unsigned int len = data.length();
	unsigned int pos = 0;
	
	if (pos + 1 > len || ptr[pos] != CURRENT_VERSION) {
		goto fail;
	}
	pos++;

	while (pos < len) {
		QString name;
		QByteArray mess;
		quint16 namelen;
		quint32 messlen;
		quint32 seconds;
		Tag *tag = NULL;

		/* network byte order ftw */

		if (pos + 2 > len) {
			goto fail;
		}
		namelen = 
			(((quint16)ptr[pos + 0] << 8) & 0xff00) |
			(((quint16)ptr[pos + 1] << 0) & 0x00ff);
		pos += 2;
		if (pos + namelen > len) {
			goto fail;
		}
		name = QString::fromUtf8(&(ptr[pos]), namelen);
		pos += namelen;

		if (pos + 4 > len) {
			goto fail;
		}
		messlen = 
			(((quint32)ptr[pos + 0] << 24) & 0xff000000) |
			(((quint32)ptr[pos + 1] << 16) & 0x00ff0000) |
			(((quint32)ptr[pos + 2] <<  8) & 0x0000ff00) |
			(((quint32)ptr[pos + 3] <<  0) & 0x000000ff);
		pos += 4;
		if (pos + messlen > len) {
			goto fail;
		}
		mess = QByteArray(&(ptr[pos]), messlen);
		pos += messlen;

		if (pos + 4 > len) {
			goto fail;
		}
		seconds =
			(((quint32)ptr[pos + 0] << 24) & 0xff000000) |
			(((quint32)ptr[pos + 1] << 16) & 0x00ff0000) |
			(((quint32)ptr[pos + 2] <<  8) & 0x0000ff00) |
			(((quint32)ptr[pos + 3] <<  0) & 0x000000ff);
		pos += 4;

		tag = new Tag(name, mess, seconds);
		contents << tag;
	}

	return true;

fail:
	while (contents.isEmpty() == false) {
		delete contents.takeFirst();
	}
	return false;
}

static TagStorageImpl *storage(void)
{
	if (singleton != NULL) {
		return singleton;
	}

	QString location = 
		QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/" + 
		QCoreApplication::organizationName() + "/" +
		QCoreApplication::applicationName();

	if (QFile::exists(location) == false) {
		QDir dir;
		dir.mkpath(location);
	}

	QFile file(location + "/" + "tag-creator-storage.bin");
	QList<Tag *>contents;

	if (_readStorage(file, contents) == false) {
		/* TODO: should whinge about this in the UI if file exists! */
		singleton = new TagStorageImpl();
	} else {
		singleton = new TagStorageImpl(contents);
	}

	return singleton;
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


