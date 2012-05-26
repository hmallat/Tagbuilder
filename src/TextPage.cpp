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

#include "TextPage.h"
#include "LabeledTextEdit.h"
#include "TextRecordEdit.h"
#include "Util.h"

#include <QGraphicsLinearLayout>
#include <MAction>
#include <MLabel>

#include <QNdefNfcTextRecord>
#include <QNdefMessage>

#include <MDebug>

QTM_USE_NAMESPACE;

TextPage::TextPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_edit(NULL)
{
}

TextPage::~TextPage(void)
{
}

void TextPage::createPageSpecificActions(void)
{
}

void TextPage::createPageSpecificContent(void)
{
	m_edit = new TextRecordEdit("", 
				    LabeledTextEdit::MultiLineEditAndLabel);
	m_edit->setLabel(tr("Text"));
	m_edit->setPrompt(tr("Enter text"));
	layout()->addItem(m_edit);
	layout()->setAlignment(m_edit, Qt::AlignHCenter);

	connect(m_edit, SIGNAL(contentsChanged()),
		this, SLOT(textChanged(void)));

	connect(m_edit, SIGNAL(languageCodeChanged()),
		this, SLOT(langChanged(void)));
}

void TextPage::setupNewData(void)
{
	m_edit->setLanguageCode(Util::currentLanguageCode());
#ifdef LABEL_SIZE
	updateSize();
#endif
}

bool TextPage::setupData(QNdefMessage message)
{
	QNdefNfcTextRecord T(message[0]);
	if (T.isEmpty() == true) {
		return false;
	}

	m_edit->setLanguageCode(T.locale());
	m_edit->setContents(T.text());
#ifdef LABEL_SIZE
	updateSize();
#endif
	return true;
}

QNdefMessage TextPage::prepareDataForStorage(void)
{
	QNdefMessage message;
	QNdefNfcTextRecord T;
	T.setEncoding(QNdefNfcTextRecord::Utf8);
	T.setLocale(m_edit->languageCode());
	T.setText(m_edit->contents());
	message << T;
	return message;
}

void TextPage::textChanged(void)
{
	setContentValidity(m_edit->contents() != "" ? true : false);
#ifdef LABEL_SIZE
	updateSize();
#endif
}

void TextPage::langChanged(void)
{
#ifdef LABEL_SIZE
	updateSize();
#endif
}

#ifdef LABEL_SIZE
void TextPage::updateSize(void)
{
	/* Quite a bunch of calculation just to get one byte count */

	QString lang = m_edit->languageCode();
	QString cont = m_edit->contents();

	quint32 payloadLength = 
		1 + /* status byte */
		lang.toAscii().length() + /* language code is in ASCII */
		cont.toUtf8().length(); /* text itself is in UTF-8 */

	quint32 ndefLength = 
		1 + /* NDEF header */
		1 + /* type length */
		(payloadLength < 256 ? 1 : 4) + /* payload length */
		1 + /* type (T) */
		payloadLength; /* payload */

	setContentSize(ndefLength);
}
#endif
