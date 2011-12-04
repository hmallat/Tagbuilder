/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BtNdefRecord.h"
#include "VCardNdefRecord.h"
#include "TagStorage.h"
#include "Tag.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QList>

#include <MDebug>

/* TODO: totally inefficient storage implementation to be fixed... */

/*
 * TagStorage is simply a file which is read and written using QDataStream.
 * 
 * First object to read is a qint8 indicating version (0x10 for 1.0). 
 *
 * Each tag has three fields: UTF-8 byte array for name, byte array
 * for contents, and a timestamp for creation time.
 */

#define CURRENT_VERSION 0x10

static QString _storageLocation(void)
{
	QString location = 
		QDesktopServices::storageLocation(QDesktopServices::DataLocation);

	return location;
}

static bool _createStorageLocation(void)
{
	QString location = _storageLocation();

	if (QFile::exists(location)) {
		return true;
	}

	QDir dir;
	return dir.mkpath(location);
}

static QString _storageFilename(void)
{
	return _storageLocation() + "/" + "tag-creator-storage.bin";
}

static bool _writeStorage(QList<Tag *> &contents)
{
	QFile file(_storageFilename());

	if (_createStorageLocation() == false) {
		return false;
	}
	
	if ((file.exists() == true && file.remove() == false)) {
		return false;
	}

	if (file.open(QIODevice::WriteOnly) == false) {
		return false;
	}

	QByteArray data;

	char version = CURRENT_VERSION;
	data.append(&version, 1);

	for (int i = 0; i < contents.length(); i++) {
		Tag *tag = contents[i];

		char namelen[2];
		QByteArray name = tag->name().toUtf8();
		namelen[0] = (name.length() >> 8) & 0xff;
		namelen[1] = (name.length() >> 0) & 0xff;
		data.append(namelen, 2);
		data.append(name.data(), name.length());

		char messlen[4];
		QByteArray mess = tag->message().toByteArray();
		messlen[0] = (mess.length() >> 24) & 0xff;
		messlen[1] = (mess.length() >> 16) & 0xff;
		messlen[2] = (mess.length() >>  8) & 0xff;
		messlen[3] = (mess.length() >>  0) & 0xff;
		data.append(messlen, 4);
		data.append(mess.data(), mess.length());

		char created[4];
		qint64 seconds = tag->creationTime().toMSecsSinceEpoch()/1000;
		created[0] = (seconds >> 24) & 0xff;
		created[1] = (seconds >> 16) & 0xff;
		created[2] = (seconds >>  8) & 0xff;
		created[3] = (seconds >>  0) & 0xff;
		data.append(created, 4);
		
	}

	if (file.write(data) != data.length()) {
		return false;
	}

	return true;
}

static bool _readStorage(QList<Tag *> &contents)
{
	while (contents.isEmpty() == false) {
		delete contents.takeFirst();
	}

	QFile file(_storageFilename());
	if (file.exists() == false) {
		return true; /* initial storage: no entries */
	}

	if (file.open(QIODevice::ReadOnly) == false) {
		return false;
	}

	QByteArray data = file.readAll();
	char *ptr = data.data();
	unsigned int len = data.length();
	unsigned int pos = 0;
	
	if (pos + 1 > len || ptr[pos] != CURRENT_VERSION) {
		mDebug(__func__) << "Unknown version, failing. ";
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
			mDebug(__func__) << "Runt (name length), failing. ";
			goto fail;
		}
		namelen = 
			(((quint16)ptr[pos + 0] << 8) & 0xff00) |
			(((quint16)ptr[pos + 1] << 0) & 0x00ff);
		pos += 2;
		if (pos + namelen > len) {
			mDebug(__func__) << "Runt (name), failing. ";
			goto fail;
		}
		name = QString::fromUtf8(&(ptr[pos]), namelen);
		pos += namelen;

		if (pos + 4 > len) {
			mDebug(__func__) << "Runt (msg length), failing. ";
			goto fail;
		}
		messlen = 
			(((quint32)ptr[pos + 0] << 24) & 0xff000000) |
			(((quint32)ptr[pos + 1] << 16) & 0x00ff0000) |
			(((quint32)ptr[pos + 2] <<  8) & 0x0000ff00) |
			(((quint32)ptr[pos + 3] <<  0) & 0x000000ff);
		pos += 4;
		if (pos + messlen > len) {
			mDebug(__func__) << "Runt (msg), failing. ";
			goto fail;
		}
		mess = QByteArray(&(ptr[pos]), messlen);
		pos += messlen;

		if (pos + 4 > len) {
			mDebug(__func__) << "Runt (timestamp), failing. ";
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
	tags << tag;
	return _writeStorage(tags);
}

bool TagStorageImpl::update(int which, Tag *tag)
{
	Tag *old = tags[which];
	tags[which] = tag;
	delete old;
	return _writeStorage(tags);
}

bool TagStorageImpl::remove(int which)
{
	Tag *old = tags[which];
	tags.removeAt(which);
	delete old;
	return _writeStorage(tags);
}

static TagStorageImpl *singleton = NULL;

static TagStorageImpl *storage(void)
{
	if (singleton != NULL) {
		return singleton;
	}

	QList<Tag *>contents;
	if (_readStorage(contents) == false) {
		/* TODO: should whinge about this in the UI if file
		 * exists but cannot be read! */
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
			const QNdefMessage &message)
{
	Tag *tag = new Tag(name, message);
	return storage()->append(tag);
}

bool TagStorage::update(int which, 
			const QString &name,
			const QNdefMessage &message)
{
	Tag *tag = new Tag(name, message);
	return storage()->update(which, tag);
}

bool TagStorage::remove(int which)
{
	return storage()->remove(which);
}


