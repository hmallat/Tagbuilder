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

#ifndef _FOURSQUARE_AUTH_NETWORK_ACCESS_MANAGER_H_
#define _FOURSQUARE_AUTH_NETWORK_ACCESS_MANAGER_H_

#include <QNetworkAccessManager>

class FoursquareAuthNetworkAccessManager : public QNetworkAccessManager
{

	Q_OBJECT;

public:
	FoursquareAuthNetworkAccessManager(QNetworkAccessManager *oldManager, 
					   QString redirPrefix, 
					   QObject *parent = 0);

Q_SIGNALS:

	void tokenReceived(QString token);

protected:

	virtual QNetworkReply* createRequest
		(QNetworkAccessManager::Operation op, 
		 const QNetworkRequest &req, 
		 QIODevice *device);

private:

	QString m_prefix;

};

#endif /* _FOURSQUARE_AUTH_NETWORK_ACCESS_MANAGER_H_ */
