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

#include "TextRecordEdit.h"
#include "LabeledTextEdit.h"
#include "Util.h"

#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MComboBox>
#include <MDebug>

TextRecordEdit::TextRecordEdit(const QString actionLabel,
			       LabeledTextEdit::Style style,
			       QGraphicsItem *parent)
	: MStylableWidget(parent),
	  m_actionLabel(actionLabel),
	  m_style(style),
	  m_layout(0),
	  m_text(0),
	  m_combo(0)
{
}

LabeledTextEdit *TextRecordEdit::textWidget(void)
{
	if (m_text == NULL) {
		m_text = new LabeledTextEdit(m_actionLabel, m_style, this);
		connect(m_text, SIGNAL(contentsChanged()),
			this, SIGNAL(contentsChanged()));
	}

	return m_text;
}

MComboBox *TextRecordEdit::comboWidget(void)
{
	if (m_combo == NULL) {

		QStringList codes = Util::availableLanguageCodes();
		QStringList langs;
		for (int i = 0; i < codes.length(); i++) {
			langs << Util::languageCodeToString(codes[i]);
			m_languageToCode[langs[i]] = codes[i];
			m_codeToLanguage[codes[i]] = langs[i];
		}
		langs.sort();

		m_combo = new MComboBox();
		m_combo->setTitle(tr("Language"));
		m_combo->addItems(langs);	
		connect(m_combo, SIGNAL(currentIndexChanged(int)),
			this, SIGNAL(languageCodeChanged()));
	}

	return m_combo;
}

QGraphicsLinearLayout *TextRecordEdit::createLayout(void)
{
	if (m_layout != 0) {
		delete m_layout;
		m_layout = 0;
	}
	
	m_layout = new QGraphicsLinearLayout(Qt::Vertical);
	m_layout->addItem(textWidget());
	m_layout->addItem(comboWidget());

	return m_layout;
}

void TextRecordEdit::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	MWidgetController::resizeEvent(event);
	
	if (m_layout == NULL) {
		setLayout(createLayout());
	}
}

QString TextRecordEdit::label(void) const
{
	return (m_text != 0) ? m_text->label() : "";
}

QString TextRecordEdit::prompt(void) const
{
	return (m_text != 0) ? m_text->prompt() : "";
}

QString TextRecordEdit::contents(void) const
{
	return (m_text != 0) ? m_text->contents() : "";
}

QString TextRecordEdit::languageCode(void) const
{
	return m_combo != 0 ? m_languageToCode[m_combo->currentText()] : "";
}

void TextRecordEdit::setLabel(const QString &l)
{
	textWidget()->setLabel(l);
}

void TextRecordEdit::setPrompt(const QString &p)
{
	textWidget()->setPrompt(p);
}

void TextRecordEdit::setContents(const QString &c)
{
	textWidget()->setContents(c);
}

void TextRecordEdit::setLanguageCode(const QString &l)
{
	comboWidget(); /* duh */
	QString lang = m_codeToLanguage[l];
	mDebug(__func__) << "code " << l << " is language " << lang << ". ";
	for (int i = 0; i < comboWidget()->count(); i++) {
		if (lang == comboWidget()->itemText(i)) {
			comboWidget()->setCurrentIndex(i);
			return;
		}
	}
}
