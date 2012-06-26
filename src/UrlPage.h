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

#ifndef _URL_PAGE_H
#define _URL_PAGE_H_

#include "CreateEditPage.h"
#include "FoursquareVenue.h"

#include <QNdefMessage>
#include <QContactManager>

class QSignalMapper;
class TextRecordEdit;
class LabeledTextEdit;
class MButton;
class MContainer;

QTM_USE_NAMESPACE;

class UrlPage : public CreateEditPage
{

	Q_OBJECT;

	enum Action {
		NoAction,
		DoAction,
		SaveAction,
		EditAction
	};

public:

	UrlPage(int tag = TagStorage::NULL_TAG, QGraphicsItem *parent = 0);

	virtual ~UrlPage(void);

protected:

	virtual void createPageSpecificActions(void);

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void urlChanged(void);

	void titleChanged(QObject *which);

	void addTitle(void);

	void removeTitle(QObject *which);

	void actChanged(void);

	void chooseFromBookmarks(void);

	void bookmarkChosen(const QNdefMessage message);

	void chooseEmailContact(void);

	void emailContactChosen(const QContact which);

	void emailAddressChosen(const QContact which);

	void choosePhoneContact(void);

	void phoneContactChosen(const QContact which);

	void phoneNumberChosen(const QContact which);

	void chooseFoursquareVenue(void);

	void foursquareVenueChosen(const FoursquareVenue which);

private:

	Q_DISABLE_COPY(UrlPage);

#ifdef LABEL_SIZE
	void updateSize(void);
#endif

	void checkTitleValidity(void);

	enum Action checkedAction(void);

	QSignalMapper *m_titleRemoveMapper;

	QSignalMapper *m_titleChangeMapper;

	LabeledTextEdit *m_url;

	MButton *m_addTitle;

	QGraphicsLinearLayout *m_titleLayout;

	QList<TextRecordEdit *> m_titles;

	MButton *actButton[4];

	QContactManager m_contactManager;

	QString m_contactLabel;

	bool m_urlValidity;

	bool m_titleValidity;

};

#endif /* _URL_PAGE_H_ */
