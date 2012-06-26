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

#include "FoursquareAuthPage.h"
#include "FoursquareAuthNetworkAccessManager.h"
#include "ViewHeader.h"

#include <QGraphicsWebView>
#include <QGraphicsAnchorLayout>
#include <QNetworkReply>
#include <MAction>

#define CLIENT_ID "WJTLH3FLFCSPRKTYYBSSUCLHDLQA4YITU2AWBANS1C5LRREX"
#define REDIRECT_URL "http://hannu.mallat.fi/n9/nfctagbuilder/callback"

FoursquareAuthPage::FoursquareAuthPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_header(0),
	  m_view(0),
	  m_nam(0)
{
	setComponentsDisplayMode(MApplicationPage::EscapeButton,
				 MApplicationPageModel::Hide);

	connect(this, SIGNAL(created()), 
		this, SLOT(activate()),
		Qt::QueuedConnection);
}

FoursquareAuthPage::~FoursquareAuthPage(void)
{
}

void FoursquareAuthPage::createContent(void)
{
	MApplicationPage::createContent();
	setStyleName("CommonApplicationPage");
	setPannable(false);

	MAction *cancelAction = new MAction(tr("Cancel"), this);
	cancelAction->setLocation(MAction::ToolBarLocation);
	connect(cancelAction, SIGNAL(triggered()),
		this, SLOT(dismiss()));
	addAction(cancelAction);

	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout();
	layout->setContentsMargins(0, 0, 0, 0);

	m_header = new ViewHeader(tr("Foursquare authentication"));

	layout->addCornerAnchors(m_header, Qt::TopLeftCorner,
				 layout, Qt::TopLeftCorner);
	layout->addCornerAnchors(m_header, Qt::TopRightCorner,
				 layout, Qt::TopRightCorner);

	m_view = new QGraphicsWebView();
	m_view->setResizesToContents(false);
	connect(m_view, SIGNAL(loadStarted()),
		this, SLOT(pageLoadStarted()));
	connect(m_view, SIGNAL(loadFinished(bool)),
		this, SLOT(pageLoadFinished(bool)));

	layout->addCornerAnchors(m_view, Qt::TopLeftCorner,
				 m_header, Qt::BottomLeftCorner);
	layout->addCornerAnchors(m_view, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);

	m_nam = new FoursquareAuthNetworkAccessManager(m_view->page()->networkAccessManager(), 
						       REDIRECT_URL,
						       this);
	connect(m_nam, SIGNAL(tokenReceived(QString)),
		this, SLOT(tokenReceived(QString)));

	m_view->page()->setNetworkAccessManager(m_nam);

	centralWidget()->setLayout(layout);

	Q_EMIT(created());
}

void FoursquareAuthPage::activate(void)
{
	QString url =
		"https://foursquare.com/oauth2/authenticate"
		"?client_id=" CLIENT_ID
		"&response_type=token"
		"&redirect_uri=" REDIRECT_URL
		"&display=webpopup";

	m_view->load(url);
}

void FoursquareAuthPage::setBusy(void)
{
	m_header->setBusy();
}

void FoursquareAuthPage::clearBusy(void)
{
	m_header->clearBusy();
}

void FoursquareAuthPage::pageLoadStarted(void)
{
	setBusy();
}

void FoursquareAuthPage::pageLoadFinished(bool ok)
{
	(void)ok;
	clearBusy();
}

void FoursquareAuthPage::tokenReceived(QString token)
{
	Q_EMIT(authenticationComplete(token));
	dismiss();
}
