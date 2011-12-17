/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TEXT_RECORD_EDIT_H_
#define _TEXT_RECORD_EDIT_H_

#include <MStylableWidget>

#include "LabeledTextEdit.h"

class QGraphicsLinearLayout;
class MComboBox;

class TextRecordEdit : public MStylableWidget
{

	Q_OBJECT;

public:

	TextRecordEdit(LabeledTextEdit::Style style,
		       const QStringList availableLanguages,
		       QGraphicsItem *parent = 0);

	QString label(void) const;

	QString prompt(void) const;

	QString contents(void) const;

	QString language(void) const;

	void setLabel(const QString &);

	void setPrompt(const QString &);

	void setContents(const QString &);

	void setLanguage(const QString &);

Q_SIGNALS:

	void contentsChanged(void);

	void languageChanged(void);

private:

	Q_DISABLE_COPY(TextRecordEdit);

	void resizeEvent(QGraphicsSceneResizeEvent *event);

	QGraphicsLinearLayout *createLayout(void);

	LabeledTextEdit *textWidget(void);

	MComboBox *comboWidget(void);

	LabeledTextEdit::Style m_style;

	QStringList m_languages;

	QGraphicsLinearLayout *m_layout;

	LabeledTextEdit *m_text;
	
	MComboBox *m_combo;

};

#endif /* _TEXT_RECORD_EDIT_H_ */
