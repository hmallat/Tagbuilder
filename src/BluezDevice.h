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

#ifndef _BLUEZ_DEVICE_H_
#define _BLUEZ_DEVICE_H_

#include <QObject>
#include <QDBusObjectPath>
#include <QDBusConnection>
#include <QBluetoothDeviceInfo>

class QDBusInterface;
class QDBusVariant;
class QDBusPendingCallWatcher;
class QDBusVariant;

QTM_USE_NAMESPACE;

class BluezDevice : public QObject
{

	Q_OBJECT;

public:

	BluezDevice(const QDBusObjectPath path, QObject *parent = 0);

	BluezDevice(const QString address,
		    QMap<QString, QVariant> properties,
		    QObject *parent = 0);

	~BluezDevice(void);

	const QDBusObjectPath path(void) const;

	QDBusInterface *interface(void) const;

	const QString address(void) const;

	const QString name(void) const;

	const QString alias(void) const;

	uint cod(void) const;

	QBluetoothDeviceInfo toBluetoothDeviceInfo(void) const;

signals:

	void deviceUpdated(QDBusObjectPath which);

private slots:

	void getPropertiesDone(QDBusPendingCallWatcher *watcher);

	void propertyChanged(const QString, const QDBusVariant);

	void propertyChanged(const QString, const QVariant);

private:

	Q_DISABLE_COPY(BluezDevice);

	QDBusConnection m_sys;

	QDBusObjectPath m_path;

	QDBusInterface *m_interface;

	QString m_address;

	QString m_name;

	QString m_alias;

	uint m_cod;

};

#endif /* _BLUEZ_DEVICE_H_ */
