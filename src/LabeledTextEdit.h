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

#ifndef _LABELED_TEXT_EDIT_H_
#define _LABELED_TEXT_EDIT_H_

#include <MStylableWidget>

class MLabel;
class MTextEdit;
class QGraphicsLinearLayout;
class QValidator;

class LabeledTextEdit : public MStylableWidget
{

	Q_OBJECT;
	
public:

	enum Style {
		SingleLineEditOnly,
		SingleLineEditAndLabel,
		MultiLineEditOnly,
		MultiLineEditAndLabel,
	};

	LabeledTextEdit(const QString actionKeyLabel,
			Style style = MultiLineEditAndLabel, 
			QGraphicsItem *parent = 0);

	QString label(void) const;

	QString prompt(void) const;

	QString contents(void) const;

	void setLabel(const QString &);

	void setPrompt(const QString &);

	void setContents(const QString &);

	void setValidator(QValidator *v, int maxLen);

	bool hasAcceptableInput(void);

Q_SIGNALS:

	void contentsChanged(void);

	void editCompleted(void);

private Q_SLOTS:

	void adjustAction(void);

	void returnPressed(void);

private:

	Q_DISABLE_COPY(LabeledTextEdit);

	void resizeEvent(QGraphicsSceneResizeEvent *event);

	QGraphicsLinearLayout *createLayout(void);

	MLabel *labelWidget(void);

	MTextEdit *textWidget(void);

	QString m_actionLabel;

	enum Style m_style;

	QGraphicsLinearLayout *m_layout;

	MLabel *m_label;

	MTextEdit *m_text;

};

#endif /* _LABELED_TEXT_EDIT_H_ */
