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

TextRecordEdit::TextRecordEdit(const QString title,
			       const QString titlePrompt,
			       const QStringList availableLanguages,
			       const QString initialLanguage,
			       const QString initialContents,
			       bool editMode,
			       QGraphicsLayoutItem *parent)
	: QGraphicsLayout(parent)
{
	m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);

	m_text = new LabeledTextEdit(editMode == true
				     ? LabeledTextEdit::MultiLineEditAndLabel
				     : LabeledTextEdit::SingleLineEditAndLabel);
	m_text->setLabel(title);
	m_text->setPrompt(titlePrompt);
	m_text->setContents(initialContents);
	m_layout->addItem(m_text);
	m_layout->setAlignment(m_text, Qt::AlignHCenter);
	m_layout->setStretchFactor(m_text, 999);
	connect(m_text, SIGNAL(contentsChanged(void)),
		this, SIGNAL(contentsChanged(void)));

	{
		QGraphicsLinearLayout *sub_layout = 
			new QGraphicsLinearLayout(Qt::Horizontal);

		m_langCombo = new MComboBox();
		m_langCombo->setTitle(tr("Language"));
		m_langCombo->addItems(availableLanguages);
		sub_layout->addItem(m_langCombo);
		sub_layout->setAlignment(m_langCombo, 
					 Qt::AlignLeft | Qt::AlignVCenter);
		connect(m_langCombo, SIGNAL(currentIndexChanged(int)),
			this, SIGNAL(languageChanged(void)));
		
		m_layout->addItem(sub_layout);
	}

	setLanguage(initialLanguage);
}

TextRecordEdit::~TextRecordEdit(void)
{
	mDebug(__func__) << "Deleting...";
	for (int i = count() - 1; i >= 0; --i) {
		mDebug(__func__) << "Removing at " << i;
		QGraphicsLayoutItem *item = itemAt(i);
		removeAt(i);
		if (item != NULL && item->ownedByLayout() == true) {
			mDebug(__func__) << "Subdeleting at " << i;
			delete item;
		}
	}
}

void TextRecordEdit::setGeometry(const QRectF &rect)
{
	if (m_layout != 0) {
		m_layout->setGeometry(rect);
	}
}

QSizeF TextRecordEdit::sizeHint(Qt::SizeHint which, 
				const QSizeF &constraint) const
{
	return m_layout != 0
		? m_layout->sizeHint(which, constraint)
		: QSizeF();
}

int TextRecordEdit::count(void) const
{
	return m_layout != 0 ? 1 : 0;
}

QGraphicsLayoutItem *TextRecordEdit::itemAt(int i) const 
{
	return i == 0 ? m_layout : NULL;
}

void TextRecordEdit::removeAt(int i)
{
	(void) i;

	if (m_layout == 0) {
		return;
	}

	m_layout->setParentLayoutItem(0);
	invalidate();
}

const QString TextRecordEdit::language(void) const
{
	return m_langCombo != 0 ? m_langCombo->currentText() : "";
}

void TextRecordEdit::setLanguage(const QString language)
{
	if (language == "") {
		m_langCombo->setCurrentIndex(0);
		return;
	}

	if (m_langCombo != 0) {
		for (int i = 0; i < m_langCombo->count(); i++) {
			if (language == m_langCombo->itemText(i)) {
				m_langCombo->setCurrentIndex(i);
				break;
			}
		}
		if (m_langCombo->currentIndex() == -1) {
			m_langCombo->addItem(language);
			m_langCombo->setCurrentIndex(m_langCombo->count() - 1);
		}
	}
}

const QString TextRecordEdit::contents(void) const
{
	return m_text != 0 ? m_text->contents() : "";
}

void TextRecordEdit::setContents(const QString what)
{
	if (m_text != 0) {
		m_text->setContents(what);
	}
}
