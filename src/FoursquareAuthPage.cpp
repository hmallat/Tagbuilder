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
#include "ViewHeader.h"

#include <QWebView>
#include <QGraphicsAnchorLayout>
#include <QGraphicsProxyWidget>
#include <MAction>

#define CLIENT_ID "WJTLH3FLFCSPRKTYYBSSUCLHDLQA4YITU2AWBANS1C5LRREX"
#define REDIRECT_URL "http://hannu.mallat.fi/n9/nfctagbuilder/callback"

FoursquareAuthPage::FoursquareAuthPage(QGraphicsItem *parent)
	: MApplicationPage(parent),
	  m_header(0),
	  m_view(0)
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

	m_view = new QWebView();
	QGraphicsProxyWidget *proxy = scene()->addWidget(m_view);

	layout->addCornerAnchors(proxy, Qt::TopLeftCorner,
				 m_header, Qt::BottomLeftCorner);
	layout->addCornerAnchors(proxy, Qt::BottomRightCorner,
				 layout, Qt::BottomRightCorner);

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
		"&display=touch";

	m_view->load(url);

}
