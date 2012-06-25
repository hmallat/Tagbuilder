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

#include "UrlPage.h"
#include "TextRecordEdit.h"
#include "LabeledTextEdit.h"
#include "SmartPosterRecord.h"
#include "Util.h"
#include "BookmarkSelectionPage.h"
#include "ContactSelectionPage.h"
#include "ContactDetailPicker.h"
#include "ContactDetailPickerListModel.h"
#include "FoursquareVenueSelectionPage.h"
#include "Tag.h"

#include <QUrl>
#include <QGraphicsLinearLayout>
#include <QNdefNfcUriRecord>
#include <QSignalMapper>
#include <QContactEmailAddress>
#include <QContactPhoneNumber>

#include <MContainer>
#include <MLabel>
#include <MButton>
#include <MButtonGroup>
#include <MAction>

#include <MDebug>

UrlPage::UrlPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_titleRemoveMapper(new QSignalMapper(this)),
	  m_titleChangeMapper(new QSignalMapper(this)),
	  m_url(0),
	  m_addTitle(0),
	  m_titleLayout(0),
	  m_titles(),
	  m_urlValidity(false),
	  m_titleValidity(true)
{
	connect(m_titleRemoveMapper, SIGNAL(mapped(QObject *)),
		this, SLOT(removeTitle(QObject *)));
	connect(m_titleChangeMapper, SIGNAL(mapped(QObject *)),
		this, SLOT(titleChanged(QObject *)));
}

UrlPage::~UrlPage(void)
{
}

void UrlPage::createPageSpecificActions(void)
{
	MAction *pickAction = new MAction(tr("Browser bookmark..."),
					  this);
	pickAction->setLocation(MAction::ApplicationMenuLocation);
	connect(pickAction, SIGNAL(triggered()),
		this, SLOT(chooseFromBookmarks()));
	addAction(pickAction);
	
	MAction *emailAction = new MAction(tr("Addressbook email..."),
					   this);
	emailAction->setLocation(MAction::ApplicationMenuLocation);
	connect(emailAction, SIGNAL(triggered()),
		this, SLOT(chooseEmailContact()));
	addAction(emailAction);
	
	MAction *phoneAction = new MAction(tr("Addressbook phone number..."),
					   this);
	phoneAction->setLocation(MAction::ApplicationMenuLocation);
	connect(phoneAction, SIGNAL(triggered()),
		this, SLOT(choosePhoneContact()));
	addAction(phoneAction);

	MAction *venueAction = new MAction(tr("Foursquare venue..."),
					  this);
	venueAction->setLocation(MAction::ApplicationMenuLocation);
	connect(venueAction, SIGNAL(triggered()),
		this, SLOT(chooseFoursquareVenue()));
	addAction(venueAction);
	
	MAction *venueHereAction = new MAction(tr("Nearby Foursquare venue..."),
					  this);
	venueHereAction->setLocation(MAction::ApplicationMenuLocation);
	connect(venueHereAction, SIGNAL(triggered()),
		this, SLOT(chooseFoursquareVenueNearHere()));
	addAction(venueHereAction);
	
}

void UrlPage::createPageSpecificContent(void)
{
	m_url = new LabeledTextEdit(tr("Ok"),
				    LabeledTextEdit::SingleLineEditAndLabel);
	m_url->setLabel(tr("Bookmark URL"));
	m_url->setPrompt(tr("Enter bookmark URL"));
	m_url->setContents("http://");
	m_url->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	layout()->addItem(m_url);
	layout()->setAlignment(m_url, Qt::AlignCenter);
	connect(m_url, SIGNAL(contentsChanged()),
		this, SLOT(urlChanged(void)));

	{
		MLabel *label = new MLabel(tr("Titles"));
		label->setAlignment(Qt::AlignLeft);
		layout()->addItem(label);
		layout()->setAlignment(label, Qt::AlignLeft);

		m_titleLayout =	new QGraphicsLinearLayout(Qt::Vertical);
		m_titleLayout->setSizePolicy(QSizePolicy::Preferred, 
					     QSizePolicy::Preferred);
		layout()->addItem(m_titleLayout);
		layout()->setAlignment(m_titleLayout, Qt::AlignCenter);

		m_addTitle = new MButton();
		m_addTitle->setText("Add a title");
		m_addTitle->setSizePolicy(QSizePolicy::Preferred, 
					  QSizePolicy::Fixed);
		m_titleLayout->addItem(m_addTitle);
		m_titleLayout->setAlignment(m_addTitle, Qt::AlignCenter);
		connect(m_addTitle, SIGNAL(clicked()),
			this, SLOT(addTitle()));
	}

	{
		MLabel *label = new MLabel(tr("Action"));
		label->setAlignment(Qt::AlignLeft);
		layout()->addItem(label);
		layout()->setAlignment(label, Qt::AlignLeft);

		QGraphicsLinearLayout *but_layout =
			new QGraphicsLinearLayout(Qt::Vertical);
		but_layout->setSizePolicy(QSizePolicy::Preferred, 
					  QSizePolicy::Preferred);
		but_layout->setSpacing(0);
		layout()->addItem(but_layout);
		layout()->setAlignment(but_layout, Qt::AlignCenter);

		const QString title[4] = {
			tr("No action"),
			tr("Execute"),
			tr("Save for later"),
			tr("Open for editing")
		};

		const QString style[4] = {
			"CommonTopSplitButton",
			"CommonVerticalCenterSplitButton",
			"CommonVerticalCenterSplitButton",
			"CommonBottomSplitButton"
		};

		MButtonGroup* group = new MButtonGroup(this);
		for (int i = 0; i < 4; i++) {
			actButton[i] = new MButton();
			actButton[i]->setText(title[i]);
			actButton[i]->setStyleName(style[i]);
			actButton[i]->setCheckable(true);
			actButton[i]->setViewType(MButton::groupType);
			actButton[i]->setSizePolicy(QSizePolicy::Preferred, 
						    QSizePolicy::Fixed);

			connect(actButton[i], SIGNAL(clicked()),
				this, SLOT(actChanged()));

			but_layout->addItem(actButton[i]);
			but_layout->setAlignment(actButton[i], 
						 Qt::AlignHCenter);
			group->addButton(actButton[i]);
		}
		actButton[NoAction]->setChecked(true);
	}

}

enum UrlPage::Action UrlPage::checkedAction(void)
{
	return 
		(actButton[DoAction]->isChecked() == true) ? DoAction :
		(actButton[SaveAction]->isChecked() == true) ? SaveAction :
		(actButton[EditAction]->isChecked() == true) ? EditAction :
		NoAction;
}

void UrlPage::setupNewData(void)
{
#ifdef LABEL_SIZE
	updateSize();
#endif
}

bool UrlPage::setupData(const QNdefMessage message)
{
	bool r = false;

	QNdefRecord record = message[0];

	while (m_titles.length() != 0) {
		removeTitle(m_titles[0]);
	}

	if (record.isRecordType<QNdefNfcUriRecord>()) {
		QNdefNfcUriRecord U(record);
		if (U.isEmpty() == true || U.uri().isValid() == false) {
			goto exit;
		}

		m_url->setContents(U.uri().toString());
		r = true;

		actButton[NoAction]->setChecked(true);

	} else {
		SmartPosterRecord Sp(record);
		if (Sp.isEmpty() == true) {
			goto exit;
		}

		QNdefNfcUriRecord U;
		if (Sp.uri(U) == false || U.uri().isValid() == false) {
			goto exit;
		}
		m_url->setContents(U.uri().toString());

		QList<QNdefNfcTextRecord> T = Sp.titles();
		for (int i = 0; i < T.length(); i++) {
			addTitle();
			m_titles[i]->setContents(T[i].text());
			m_titles[i]->setLanguageCode(T[i].locale());
		}

		SmartPosterRecord::ActionRecord act;

		if (Sp.act(act) == true) {
			actButton[(act.action() == SmartPosterRecord::ActionRecord::ActionDo)
				  ? DoAction :
				  (act.action() == SmartPosterRecord::ActionRecord::ActionSave)
				  ? SaveAction :
				  (act.action() == SmartPosterRecord::ActionRecord::ActionEdit)
				  ? EditAction :
				  NoAction]->setChecked(true);
		} else {
			actButton[NoAction]->setChecked(true);
		}

		r = true;
	}

exit:
	if (isTemporary() == true) {
		if (m_titles.length() != 0) {
			setDefaultName(m_titles[0]->contents());
		}
	}
#ifdef LABEL_SIZE
	updateSize();
#endif
	return r;
}

QNdefMessage UrlPage::prepareDataForStorage(void)
{
	QNdefMessage message;
	enum Action action = checkedAction();

	if (m_titles.length() == 0 && action == NoAction) {
		mDebug(__func__) << "Creating U, no title or action set. ";
		QNdefNfcUriRecord U;
		U.setUri(m_url->contents());
		message << U;
	} else {
		mDebug(__func__) << "Creating Sp, title or action set. ";
		SmartPosterRecord Sp;

		QNdefNfcUriRecord U;
		U.setUri(m_url->contents());
		Sp.setUri(U);

		QList<QNdefNfcTextRecord> titles;
		for (int i = 0; i < m_titles.length(); i++) {
			QNdefNfcTextRecord title;
			title.setText(m_titles[i]->contents());
			title.setLocale(m_titles[i]->languageCode());
			titles << title;
		}
		Sp.setTitles(titles);

		if (action != NoAction) {
			SmartPosterRecord::ActionRecord act;
			act.setAction
				((action == DoAction) ? 
				 SmartPosterRecord::ActionRecord::ActionDo :
				 (action == SaveAction) ? 
				 SmartPosterRecord::ActionRecord::ActionSave :
				 SmartPosterRecord::ActionRecord::ActionEdit);
			Sp.setAct(act);
		}

		message << Sp;

	}
	return message;
}

#ifdef LABEL_SIZE
void UrlPage::updateSize(void)
{
	/* A bunch of calculations could be done without creating the
	   NDEF record, but that'd include checking the URL prefixes
	   and all. Easier just to make a scratch message. */ 
	QNdefMessage message = prepareDataForStorage();
	quint32 ndefLength = Util::messageLength(message);
	setContentSize(ndefLength);
}
#endif

void UrlPage::urlChanged(void)
{
#ifdef LABEL_SIZE
	updateSize();
#endif
	QUrl u(m_url->contents());
	m_urlValidity = u.isValid();
	setContentValidity(m_titleValidity && m_urlValidity);
}

void UrlPage::actChanged(void)
{
#ifdef LABEL_SIZE
	updateSize();
#endif
}

void UrlPage::titleChanged(QObject *which)
{
	(void) which;

#ifdef LABEL_SIZE
	updateSize();
#endif
	checkTitleValidity();
}

void UrlPage::checkTitleValidity(void)
{
	/* Sp standard forbids multiple titles using the same 
	   language, so enable/disable store button accordingly. */
	
	bool duplicate = false;
	for (int i = 0; i < m_titles.length(); i++) {
		for (int j = i + 1; j < m_titles.length(); j++) {
			if (m_titles[i]->languageCode() == 
			    m_titles[j]->languageCode()) {
				duplicate = true; 
				goto done;
			}
		}
	}

done:
	m_titleValidity = (duplicate == true) ? false : true;
	setContentValidity(m_titleValidity && m_urlValidity);
}

void UrlPage::addTitle(void)
{
	/* Add the new title edit just before the add button */

	int pos = m_titleLayout->count() - 1;

	QGraphicsLinearLayout *pack =
		new QGraphicsLinearLayout(Qt::Horizontal);

	TextRecordEdit *title = 
		new TextRecordEdit(tr("Ok"),
				   LabeledTextEdit::SingleLineEditOnly);
	title->setLanguageCode(Util::currentLanguageCode());
	title->setPrompt(tr("Enter title"));
	title->setSizePolicy(QSizePolicy::Preferred, 
			     QSizePolicy::Fixed);
	pack->addItem(title);
	pack->setAlignment(title, Qt::AlignLeft | Qt::AlignVCenter);
	pack->setStretchFactor(title, 999);
	connect(title, SIGNAL(contentsChanged()),
		m_titleChangeMapper, SLOT(map()));
	connect(title, SIGNAL(languageCodeChanged()),
		m_titleChangeMapper, SLOT(map()));
	m_titleChangeMapper->setMapping(title, title);

	MButton *xButton = new MButton();
	xButton->setIconID("icon-s-cancel");
	xButton->setSizePolicy(QSizePolicy::Preferred, 
			       QSizePolicy::Preferred);
	xButton->setStyleName("CommonSquareButton");
	pack->addItem(xButton);
	pack->setAlignment(xButton, Qt::AlignRight | Qt::AlignVCenter);
	pack->setStretchFactor(xButton, 0);
	connect(xButton, SIGNAL(clicked()),
		m_titleRemoveMapper, SLOT(map()));
	m_titleRemoveMapper->setMapping(xButton, title);

	m_titleLayout->insertItem(pos, pack);
	m_titleLayout->setAlignment(pack, Qt::AlignHCenter);

	m_titles << title;

	layout()->invalidate();

#ifdef LABEL_SIZE
	updateSize();
#endif
	checkTitleValidity();
}

void UrlPage::removeTitle(QObject *which)
{
	int pos;

	for (pos = 0; pos < m_titles.length(); pos++) {
		if (m_titles[pos] == static_cast<TextRecordEdit *>(which)) {
			mDebug(__func__) << "Removing title from position " 
					 << pos;
			m_titles.removeAt(pos);

			QGraphicsLinearLayout *pack = 
				static_cast<QGraphicsLinearLayout *>
				(m_titleLayout->itemAt(pos));
			m_titleLayout->removeAt(pos);
			while (pack->count() != 0) {
				mDebug(__func__) << "Remove packed item. ";
				QGraphicsLayoutItem *item = pack->itemAt(0);
				pack->removeAt(0);
				delete item;
			}
			delete pack;

			layout()->invalidate();

			break;
		}
	}

#ifdef LABEL_SIZE
	updateSize();
#endif
	checkTitleValidity();
}

void UrlPage::chooseFromBookmarks(void)
{
	BookmarkSelectionPage *page = 
		new BookmarkSelectionPage();
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const QNdefMessage)),
		this, SLOT(bookmarkChosen(const QNdefMessage)));
}

void UrlPage::bookmarkChosen(const QNdefMessage message)
{
	Tag::dump(message);
	setupData(message);

	if (m_titles.length() != 0) {
		setDefaultName(m_titles[0]->contents());
	}
}

void UrlPage::chooseEmailContact(void)
{
	ContactSelectionPage *page = 
		new ContactSelectionPage(&m_contactManager,
					 Util::EmailAddress);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const QContact)),
		this, SLOT(emailContactChosen(const QContact)));
}

void UrlPage::emailContactChosen(const QContact which)
{
 	mDebug(__func__) << which.displayLabel();
	m_contactLabel = which.displayLabel();
	ContactDetailPicker *picker = 
		new ContactDetailPicker(which,
					Util::EmailAddress,
					true);
	picker->appear(MSceneWindow::DestroyWhenDismissed);
	connect(picker, SIGNAL(contactPicked(const QContact)),
		this, SLOT(emailAddressChosen(const QContact)));
}

void UrlPage::emailAddressChosen(const QContact which)
{
 	mDebug(__func__) << which.displayLabel();

	QNdefMessage message;

	QContactEmailAddress addr = which.detail<QContactEmailAddress>();

	QString mailto = "mailto:" + addr.emailAddress();
	QNdefNfcUriRecord u;
	u.setUri(mailto);

	QNdefNfcTextRecord t;
	t.setText(tr("Email address of %1").arg(m_contactLabel));
	t.setLocale(Util::currentLanguageCode());

	QList<QNdefNfcTextRecord> ts;
	ts << t;

	SmartPosterRecord sp;
	sp.setUri(u);
	sp.setTitles(ts);

	message << sp;

	Tag::dump(message);
	setupData(message);

	setDefaultName(t.text());
}

void UrlPage::choosePhoneContact(void)
{
	ContactSelectionPage *page = 
		new ContactSelectionPage(&m_contactManager,
					 Util::PhoneNumber);
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const QContact)),
		this, SLOT(phoneContactChosen(const QContact)));
}

void UrlPage::phoneContactChosen(const QContact which)
{
 	mDebug(__func__) << which.displayLabel();
	m_contactLabel = which.displayLabel();
	ContactDetailPicker *picker = 
		new ContactDetailPicker(which,
					Util::PhoneNumber,
					true);
	picker->appear(MSceneWindow::DestroyWhenDismissed);
	connect(picker, SIGNAL(contactPicked(const QContact)),
		this, SLOT(phoneNumberChosen(const QContact)));
}

void UrlPage::phoneNumberChosen(const QContact which)
{
 	mDebug(__func__) << which.displayLabel();

	QNdefMessage message;

	QContactPhoneNumber num = which.detail<QContactPhoneNumber>();

	QString tel = "tel:" + num.number();
	QNdefNfcUriRecord u;
	u.setUri(tel);

	QNdefNfcTextRecord t;
	t.setText(tr("Phone number of %1").arg(m_contactLabel));
	t.setLocale(Util::currentLanguageCode());

	QList<QNdefNfcTextRecord> ts;
	ts << t;

	SmartPosterRecord sp;
	sp.setUri(u);
	sp.setTitles(ts);

	message << sp;

	Tag::dump(message);
	setupData(message);

	setDefaultName(t.text());
}

void UrlPage::chooseFoursquareVenue(void)
{
	/* TODO */
}

void UrlPage::chooseFoursquareVenueNearHere(void)
{
	FoursquareVenueSelectionPage *page =
		new FoursquareVenueSelectionPage();
	page->appear(scene(), MSceneWindow::DestroyWhenDismissed);
	connect(page, SIGNAL(selected(const FoursquareVenue)),
		this, SLOT(foursquareVenueChosen(const FoursquareVenue)));
}

void UrlPage::foursquareVenueChosen(const FoursquareVenue which)
{
 	mDebug(__func__) << which.displayLabel();

	QNdefMessage message;

	QString fsq = "http://m.foursquare.com/venue/" + which.id();
	QNdefNfcUriRecord u;
	u.setUri(fsq);

	QNdefNfcTextRecord t;
	t.setText(tr("Foursquare venue %1").arg(which.displayLabel()));
	t.setLocale(Util::currentLanguageCode());

	QList<QNdefNfcTextRecord> ts;
	ts << t;

	SmartPosterRecord sp;
	sp.setUri(u);
	sp.setTitles(ts);

	message << sp;

	Tag::dump(message);
	setupData(message);

	setDefaultName(t.text());
}
