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

#include "TagStorage.h"
#include "Tag.h"
#include "Util.h"

#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QList>

#include <MDebug>

const int TagStorage::NULL_TAG = -1;
const int TagStorage::TEMPORARY_TAG = -2;

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
	return _storageLocation() + "/" + "storage.bin";
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

class TagStorage::TagStorageImpl
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

	Tag *m_temp;

};

TagStorage::TagStorageImpl::TagStorageImpl(QList<Tag *> readTags)
	: tags(readTags),
	  m_temp(0)
{
}

TagStorage::TagStorageImpl::TagStorageImpl(void)
	: tags(),
	  m_temp(0)
{
}

TagStorage::TagStorageImpl::~TagStorageImpl(void)
{
	while (tags.isEmpty() == false) {
		delete tags.takeFirst();
	}
	delete m_temp;
}

int TagStorage::TagStorageImpl::count(void)
{
	return tags.length();
}

const Tag* TagStorage::TagStorageImpl::at(int which)
{
	if (which == TagStorage::NULL_TAG) {
		return 0;
	}
	if (which == TagStorage::TEMPORARY_TAG) {
		return m_temp;
	}
	return tags[which];
}

bool TagStorage::TagStorageImpl::append(Tag *tag)
{
	mDebug(__func__) << "Appending to tag storage: ";
	mDebug(__func__) << "New message: ";
	Tag::dump(tag->message());
	tags << tag;
	return _writeStorage(tags);
}

bool TagStorage::TagStorageImpl::update(int which, Tag *tag)
{
	if (which == TagStorage::NULL_TAG) {
		return false;
	}

	const Tag *old = at(which);

	mDebug(__func__) << "Appending tag storage: ";
	if (old != NULL) {
		mDebug(__func__) << "Previous message: ";
		Tag::dump(old->message());
	}
	mDebug(__func__) << "Updated message: ";
	Tag::dump(tag->message());

	delete old;
	if (which == TagStorage::TEMPORARY_TAG) {
		m_temp = tag;
		return true;
	} else {
		tags[which] = tag;
		return _writeStorage(tags);
	}
}

bool TagStorage::TagStorageImpl::remove(int which)
{
	if (which == TagStorage::NULL_TAG) {
		return false;
	}

	const Tag *old = at(which);
	delete old;
	if (which == TagStorage::TEMPORARY_TAG) {
		m_temp = NULL;
		return true;
	} else {
		tags.removeAt(which);
		return _writeStorage(tags);
	}
}

static TagStorage *singleton = 0;

TagStorage *TagStorage::storage(void)
{
	if (singleton != 0) {
		return singleton;
	}

	QList<Tag *>contents;
	if (_readStorage(contents) == false) {
		/* TODO: should whinge about this in the UI if file
		 * exists but cannot be read! */
		singleton = new TagStorage(new TagStorageImpl());
	} else {
		singleton = new TagStorage(new TagStorageImpl(contents));
	}

	return singleton;
}

TagStorage::TagStorage(TagStorageImpl *impl, QObject *parent)
	: MAbstractItemModel(parent),
	  m_impl(impl)
{
}

TagStorage::~TagStorage(void)
{
	delete m_impl;
}

int TagStorage::count(void) const
{
	return m_impl->count();
}

int TagStorage::groupCount(void) const
{
	return 0;
}

int TagStorage::rowCountInGroup(int group) const
{
	(void) group;
	return count();
}

QString TagStorage::groupTitle(int group) const
{
	(void) group;
	return "";
}

const Tag *TagStorage::tag(int which) const
{
	return m_impl->at(which);
}

QVariant TagStorage::itemData(int row, int group, int role) const
{
	(void) role, (void) group;
	
	return qVariantFromValue(tag(row));
}

bool TagStorage::append(const QString &name, 
			const QNdefMessage &message)
{
	Tag *tag = new Tag(name, message);
	bool r;

	Q_EMIT(layoutAboutToBeChanged());
	beginInsertRows(QModelIndex(), count(), count(), Util::animateLists);
	r = m_impl->append(tag);
	endInsertRows();
	Q_EMIT(layoutChanged());

	return r;
}

bool TagStorage::update(int which, 
			const QString &name,
			const QNdefMessage &message)
{
	Tag *tag = new Tag(name, message);
	bool r;

	r = m_impl->update(which, tag);
	if (r == true && which >= 0) {
		QModelIndex index = createIndex(which, 0);
		Q_EMIT(dataChanged(index, index));
	}

	return r;
}

bool TagStorage::remove(int which)
{
	bool r;

	if (which >= 0) {
		Q_EMIT(layoutAboutToBeChanged());
		beginRemoveRows(QModelIndex(), which, which, 
				Util::animateLists);
	}
	r = m_impl->remove(which);
	if (which >= 0) {
		endRemoveRows();
		Q_EMIT(layoutChanged());
	}

	return r;
}
