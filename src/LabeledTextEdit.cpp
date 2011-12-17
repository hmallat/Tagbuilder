/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "LabeledTextEdit.h"

#include <MLabel>
#include <MTextEdit>
#include <QGraphicsLinearLayout>
#include <MDebug>

LabeledTextEdit::LabeledTextEdit(Style style, QGraphicsItem *parent)
	: MStylableWidget(parent),
	  m_style(style),
	  m_layout(0),
	  m_label(0),
	  m_text(0)
{
}

MLabel *LabeledTextEdit::labelWidget(void)
{
	if (m_label == NULL) {
		m_label = new MLabel(this);
		m_label->setAlignment(Qt::AlignLeft);
	}

	return m_label;
}

MTextEdit *LabeledTextEdit::textWidget(void)
{
	if (m_text == NULL) {
		MTextEditModel::LineMode mode = 
			(m_style == LabeledTextEdit::SingleLineEditOnly ||
			 m_style == LabeledTextEdit::SingleLineEditAndLabel)
			? MTextEditModel::SingleLine 
			: MTextEditModel::MultiLine;
		m_text = new MTextEdit(mode, "", this);
		connect(m_text, SIGNAL(textChanged()),
			this, SIGNAL(contentsChanged()));
	}

	return m_text;
}

QGraphicsLinearLayout *LabeledTextEdit::createLayout(void)
{
	if (m_layout != 0) {
		delete m_layout;
		m_layout = 0;
	}
	
	m_layout = new QGraphicsLinearLayout(Qt::Vertical);
	if (m_style == LabeledTextEdit::SingleLineEditAndLabel ||
	    m_style == LabeledTextEdit::MultiLineEditAndLabel) {
		m_layout->addItem(labelWidget());
	}
	m_layout->addItem(textWidget());

	return m_layout;
}

void LabeledTextEdit::resizeEvent(QGraphicsSceneResizeEvent *event)
{
	MWidgetController::resizeEvent(event);
	
	if (m_layout == NULL) {
		setLayout(createLayout());
	}
}

QString LabeledTextEdit::label(void) const
{
	return (m_label != 0) ? m_label->text() : "";
}

QString LabeledTextEdit::prompt(void) const
{
	return (m_text != 0) ? m_text->prompt() : "";
}

QString LabeledTextEdit::contents(void) const
{
	return (m_text != 0) ? m_text->text() : "";
}

void LabeledTextEdit::setLabel(const QString &l)
{
	labelWidget()->setText(l);
}

void LabeledTextEdit::setPrompt(const QString &p)
{
	textWidget()->setPrompt(p);
}

void LabeledTextEdit::setContents(const QString &c)
{
	textWidget()->setText(c);
}
