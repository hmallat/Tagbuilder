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

	LabeledTextEdit(Style style = MultiLineEditAndLabel, 
			QGraphicsItem *parent = 0);

	QString label(void) const;

	QString prompt(void) const;

	QString contents(void) const;

	void setLabel(const QString &);

	void setPrompt(const QString &);

	void setContents(const QString &);

Q_SIGNALS:

	void contentsChanged(void);

private:

	Q_DISABLE_COPY(LabeledTextEdit);

	void resizeEvent(QGraphicsSceneResizeEvent *event);

	QGraphicsLinearLayout *createLayout(void);

	MLabel *labelWidget(void);

	MTextEdit *textWidget(void);

	enum Style m_style;

	QGraphicsLinearLayout *m_layout;

	MLabel *m_label;

	MTextEdit *m_text;

};

#endif /* _LABELED_TEXT_EDIT_H_ */
