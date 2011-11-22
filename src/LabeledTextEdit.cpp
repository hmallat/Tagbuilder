/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "LabeledTextEdit.h"

#include <QGraphicsLinearLayout>
#include <MLabel>

LabeledTextEdit::LabeledTextEdit(MTextEditModel::LineMode mode,
				 const QString &label,
				 const QString &prompt,
				 const QString &initialContents,
				 QGraphicsLayoutItem *parent)
	: QGraphicsLayout(parent)
{
	m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);

	m_label = new MLabel();
	m_label->setText(label);
	m_label->setAlignment(Qt::AlignLeft);
	m_layout->addItem(m_label);
	m_layout->setAlignment(m_label, Qt::AlignLeft);

	m_text = new MTextEdit(mode);
	m_text->setPrompt(prompt);
	m_text->setText(initialContents);
	m_layout->addItem(m_text);
	m_layout->setAlignment(m_text, Qt::AlignLeft);

	connect(m_text, SIGNAL(textChanged(void)),
		this, SIGNAL(textChanged(void)));

}

LabeledTextEdit::~LabeledTextEdit(void)
{
	for (int i = count() - 1; i >= 0; --i) {
		QGraphicsLayoutItem *item = itemAt(i);
		removeAt(i);
		if (item) {
			if (item->ownedByLayout())
				delete item;
		}
	}
}

void LabeledTextEdit::setGeometry(const QRectF &rect)
{
	if (m_layout != 0) {
		m_layout->setGeometry(rect);
	}
}

QSizeF LabeledTextEdit::sizeHint(Qt::SizeHint which, 
				 const QSizeF &constraint) const
{
	return m_layout != 0
		? m_layout->sizeHint(which, constraint)
		: QSizeF();
}

int LabeledTextEdit::count(void) const
{
	return m_layout != 0 ? 1 : 0;
}

QGraphicsLayoutItem *LabeledTextEdit::itemAt(int i) const
{
	return i == 0 ? m_layout : NULL;
}

void LabeledTextEdit::removeAt(int index)
{
	(void) index;

	if (m_layout == 0) {
		return;
	}

	m_layout->setParentLayoutItem(0);
	invalidate();
}

const QString LabeledTextEdit::text(void) const
{
	return m_text != 0 ? m_text->text() : "";
}


