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

TextRecordEdit::TextRecordEdit(const QStringList availableLanguages,
			       const QString initialLanguage,
			       const QString initialContents,
			       QGraphicsLayoutItem *parent)
	: QGraphicsLayout(parent)
{
	m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);

	m_text = new LabeledTextEdit(MTextEditModel::MultiLine,
				     "Text",
				     "Enter text",
				     initialContents);
	m_layout->addItem(m_text);
	m_layout->setAlignment(m_text, Qt::AlignHCenter);
	m_layout->setStretchFactor(m_text, 999);
	connect(m_text, SIGNAL(textChanged(void)),
		this, SLOT(textChanged(void)));

	{
		QGraphicsLinearLayout *sub_layout = 
			new QGraphicsLinearLayout(Qt::Horizontal);

		m_langCombo = new MComboBox();
		m_langCombo->setTitle(tr("Text language"));
		m_langCombo->addItems(availableLanguages);
		for (int i = 0; i < m_langCombo->count(); i++) {
			if (initialLanguage == m_langCombo->itemText(i)) {
				m_langCombo->setCurrentIndex(i);
				break;
			}
		}
		if (m_langCombo->currentIndex() == -1) {
			m_langCombo->setCurrentIndex(0);
		}
		sub_layout->addItem(m_langCombo);
		sub_layout->setAlignment(m_langCombo, 
					 Qt::AlignLeft | Qt::AlignVCenter);
		connect(m_langCombo, 
			SIGNAL(currentIndexChanged(int)),
			this,
			SLOT(languageChanged(void)));
		
		m_sizeLabel = new MLabel();
		m_sizeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		sub_layout->addItem(m_sizeLabel);
		sub_layout->setAlignment(m_sizeLabel, 
					 Qt::AlignRight | Qt::AlignVCenter);
		sub_layout->setStretchFactor(m_sizeLabel, 999);

		m_layout->addItem(sub_layout);
	}

	updateSize();
}

TextRecordEdit::~TextRecordEdit(void)
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

void TextRecordEdit::updateSize(void)
{
	/* This is an awful lot of work just to print a number... */

	if (m_layout == 0) {
		return;
	}

	QString lang = m_langCombo->currentText();

	int payloadLength = 
		1 + /* status byte */
		lang.toUtf8().length() + /* language code */
		m_text->text().toUtf8().length(); /* text itself */

	int ndefLength = 
		1 + /* NDEF header */
		1 + /* type length */
		(payloadLength < 256 ? 1 : 4) + /* payload length */
		1 + /* type (T) */
		payloadLength; /* payload */

	m_sizeLabel->setText(tr("%1 bytes").arg(ndefLength));

	m_size = ndefLength;
}

void TextRecordEdit::textChanged(void)
{
	updateSize();
}

void TextRecordEdit::languageChanged(void)
{
	updateSize();
}

const QString TextRecordEdit::language(void) const
{
	return m_langCombo != 0 ? m_langCombo->currentText() : "";
}

const QString TextRecordEdit::contents(void) const
{
	return m_text != 0 ? m_text->text() : "";
}

int TextRecordEdit::size(void) const
{
	return m_size;
}



