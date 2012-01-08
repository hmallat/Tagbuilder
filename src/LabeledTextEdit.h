/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
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
