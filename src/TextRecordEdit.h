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

#include <QMap>

#include "LabeledTextEdit.h"

class QGraphicsLinearLayout;
class MComboBox;

class TextRecordEdit : public MStylableWidget
{

	Q_OBJECT;

public:

	TextRecordEdit(LabeledTextEdit::Style style,
		       QGraphicsItem *parent = 0);

	QString label(void) const;

	QString prompt(void) const;

	QString contents(void) const;

	QString languageCode(void) const;

	void setLabel(const QString &);

	void setPrompt(const QString &);

	void setContents(const QString &);

	void setLanguageCode(const QString &);

Q_SIGNALS:

	void contentsChanged(void);

	void languageCodeChanged(void);

private:

	Q_DISABLE_COPY(TextRecordEdit);

	void resizeEvent(QGraphicsSceneResizeEvent *event);

	QGraphicsLinearLayout *createLayout(void);

	LabeledTextEdit *textWidget(void);

	MComboBox *comboWidget(void);

	LabeledTextEdit::Style m_style;

	QGraphicsLinearLayout *m_layout;

	LabeledTextEdit *m_text;
	
	MComboBox *m_combo;

	QMap<QString, QString> m_languageToCode;

	QMap<QString, QString> m_codeToLanguage;

};

#endif /* _TEXT_RECORD_EDIT_H_ */
