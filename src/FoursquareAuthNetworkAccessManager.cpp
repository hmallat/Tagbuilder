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

#include "FoursquareAuthNetworkAccessManager.h"

#include <QNetworkProxy>
#include <QNetworkRequest>
#include <MDebug>

FoursquareAuthNetworkAccessManager::
FoursquareAuthNetworkAccessManager(QNetworkAccessManager *oldManager, 
				   QString redirPrefix, 
				   QObject *parent) :
	QNetworkAccessManager(parent),
	m_prefix(redirPrefix + "#access_token=")
{
	setCache(oldManager->cache());
	setCookieJar(oldManager->cookieJar());
	setProxy(oldManager->proxy());
	setProxyFactory(oldManager->proxyFactory());
}

QNetworkReply* FoursquareAuthNetworkAccessManager::
createRequest(QNetworkAccessManager::Operation op, 
	      const QNetworkRequest &req, 
	      QIODevice *device)
{
	mDebug(__func__) << req.url();

	QString u = req.url().toString();
	if (u.startsWith(m_prefix) == true)
		Q_EMIT(tokenReceived(u.right(u.length() - m_prefix.length())));
	    
	return QNetworkAccessManager::createRequest(op, req, device);
}
