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

#include "FoursquareAuthStorage.h"

#include <QString>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QDir>
#include <QFile>
#include <QList>

#include <MDebug>

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
	return _storageLocation() + "/" + "4sqrauth.bin";
}

static bool _writeStorage(const QString &contents)
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

	QByteArray data = contents.toAscii();
	if (file.write(data) != data.length()) {
		return false;
	}

	return true;
}

static bool _readStorage(QString &contents)
{
	contents.clear();

	QFile file(_storageFilename());
	if (file.exists() == false) {
		return true; /* initial storage: no entries */
	}

	if (file.open(QIODevice::ReadOnly) == false) {
		return false;
	}

	QByteArray data = file.readAll();
	contents.append(data);

	return true;
}

class FoursquareAuthStorage::FoursquareAuthStorageImpl
{

public:

	FoursquareAuthStorageImpl(void);

	bool isEmpty(void) const;

	const QString get(void);

	bool set(QString auth);

private:

	QString m_auth;

};

FoursquareAuthStorage::FoursquareAuthStorageImpl::
FoursquareAuthStorageImpl(void) :
	m_auth("")
{
	_readStorage(m_auth);
}

bool FoursquareAuthStorage::FoursquareAuthStorageImpl::isEmpty(void) const
{
	return m_auth == "";
}

const QString FoursquareAuthStorage::FoursquareAuthStorageImpl::get(void)
{
	return m_auth;
}

bool FoursquareAuthStorage::FoursquareAuthStorageImpl::set(const QString auth)
{
	m_auth = auth;
	return _writeStorage(auth);
}

static FoursquareAuthStorage *singleton = 0;

FoursquareAuthStorage *FoursquareAuthStorage::storage(void)
{
	if (singleton != 0) {
		return singleton;
	}

	singleton = new FoursquareAuthStorage(new FoursquareAuthStorageImpl());
	return singleton;
}

FoursquareAuthStorage::FoursquareAuthStorage(FoursquareAuthStorageImpl *impl)
	: m_impl(impl)
{
}

FoursquareAuthStorage::~FoursquareAuthStorage(void)
{
	delete m_impl;
}

bool FoursquareAuthStorage::isEmpty(void) const
{
	return m_impl->isEmpty();
}

const QString FoursquareAuthStorage::get(void)
{
	return m_impl->get();
}

bool FoursquareAuthStorage::set(const QString auth)
{
	return m_impl->set(auth);
}

bool FoursquareAuthStorage::clear(void)
{
	return m_impl->set("");
}
