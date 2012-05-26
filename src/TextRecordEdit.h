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

	TextRecordEdit(const QString actionKeyLabel,
		       LabeledTextEdit::Style style,
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

	QString m_actionLabel;

	LabeledTextEdit::Style m_style;

	QGraphicsLinearLayout *m_layout;

	LabeledTextEdit *m_text;
	
	MComboBox *m_combo;

	QMap<QString, QString> m_languageToCode;

	QMap<QString, QString> m_codeToLanguage;

};

#endif /* _TEXT_RECORD_EDIT_H_ */
