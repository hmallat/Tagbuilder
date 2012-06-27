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

#include "LabeledTextEdit.h"
#include "Util.h"

#include <MLabel>
#include <MTextEdit>
#include <MInputMethodState>
#include <QGraphicsLinearLayout>

#include <MDebug>

LabeledTextEdit::LabeledTextEdit(const QString actionLabel,
				 Style style, 
				 QGraphicsItem *parent)
	: MStylableWidget(parent),
	  m_actionLabel(actionLabel),
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
		if (m_actionLabel != "") {
			m_text->attachToolbar(Util::imAttributeExtensionId());
			MInputMethodState::instance()->setExtendedAttribute
				(Util::imAttributeExtensionId(),
				 "/keys",
				 "actionKey",
				 "label",
				 QVariant(QString(m_actionLabel)));
		}

		connect(m_text, SIGNAL(textChanged()),
			this, SIGNAL(contentsChanged()));

		connect(m_text, SIGNAL(returnPressed()),
			this, SIGNAL(editCompleted()));

		if (mode == MTextEditModel::SingleLine) {
			connect(m_text, SIGNAL(returnPressed()),
				this, SLOT(returnPressed()));
		}
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

void LabeledTextEdit::setValidator(QValidator *v, int maxLen)
{
	textWidget()->setContentType(M::CustomContentType);
	textWidget()->setInputMethodCorrectionEnabled(true);
	textWidget()->setValidator(v);
	if (maxLen > 0) {
		textWidget()->setMaxLength(maxLen);
	}
	if (m_actionLabel != "") {
		connect(m_text, SIGNAL(textChanged()),
			this, SLOT(adjustAction()));
		adjustAction();
	}
}

bool LabeledTextEdit::hasAcceptableInput(void)
{
	return textWidget()->hasAcceptableInput();
}

void LabeledTextEdit::adjustAction(void)
{
	MInputMethodState::instance()->setExtendedAttribute
		(Util::imAttributeExtensionId(),
		 "/keys",
		 "actionKey",
		 "enabled",
		 QVariant(hasAcceptableInput()));
}

void LabeledTextEdit::returnPressed(void)
{
	/* TODO: could set focus to the next widget */
	mDebug(__func__) << "Return pressed. ";
	textWidget()->clearFocus();
}
