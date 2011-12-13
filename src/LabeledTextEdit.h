/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _LABELED_TEXT_EDIT_H_
#define _LABELED_TEXT_EDIT_H_

#include <QGraphicsLayout>
#include <MTextEdit>

class QGraphicsLinearLayout;

class LabeledTextEdit : public QObject, public QGraphicsLayout
{

	Q_OBJECT;

	Q_INTERFACES(QGraphicsLayout);

public:

	LabeledTextEdit(MTextEditModel::LineMode mode = MTextEditModel::SingleLine,
			const QString &label = QString(),
			const QString &prompt = QString(),
			const QString &initialContents = QString(),
			QGraphicsLayoutItem *parent = 0);

	virtual ~LabeledTextEdit(void);

	virtual void setGeometry(const QRectF &rect);

	virtual QSizeF sizeHint(Qt::SizeHint which, 
				const QSizeF &constraint = QSizeF()) const;

	virtual int count(void) const;

	virtual QGraphicsLayoutItem *itemAt(int i) const;

	virtual void removeAt(int index);

	MTextEdit *textEdit(void);

private:

	Q_DISABLE_COPY(LabeledTextEdit);

	QGraphicsLinearLayout *m_layout;

	MTextEdit *m_text;
	
};

#endif /* _LABELED_TEXT_EDIT_H_ */
