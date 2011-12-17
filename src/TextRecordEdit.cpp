/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "TextRecordEdit.h"
#include "LabeledTextEdit.h"

#include <QGraphicsLinearLayout>
#include <MLabel>
#include <MComboBox>
#include <MDebug>

TextRecordEdit::TextRecordEdit(LabeledTextEdit::Style style,
			       const QStringList languages,
			       QGraphicsItem *parent)
	: MStylableWidget(parent),
	  m_style(style),
	  m_languages(languages),
	  m_layout(0),
	  m_text(0),
	  m_combo(0)
{
}

LabeledTextEdit *TextRecordEdit::textWidget(void)
{
	if (m_text == NULL) {
		m_text = new LabeledTextEdit(m_style, this);
		connect(m_text, SIGNAL(contentsChanged()),
			this, SIGNAL(contentsChanged()));
	}

	return m_text;
}

MComboBox *TextRecordEdit::comboWidget(void)
{
	if (m_combo == NULL) {
		m_combo = new MComboBox();
		m_combo->setTitle(tr("Language"));
		m_combo->addItems(m_languages);
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

QString TextRecordEdit::language(void) const
{
	return m_combo != 0 ? m_combo->currentText() : "";
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

void TextRecordEdit::setLanguage(const QString &l)
{
	if (l == "") {
		comboWidget()->setCurrentIndex(0);
		return;
	}

	for (int i = 0; i < comboWidget()->count(); i++) {
		if (l == comboWidget()->itemText(i)) {
			comboWidget()->setCurrentIndex(i);
			return;
		}
	}

	comboWidget()->addItem(l);
	comboWidget()->setCurrentIndex(comboWidget()->count() - 1);
}
