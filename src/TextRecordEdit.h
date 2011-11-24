/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TEXT_RECORD_EDIT_H_
#define _TEXT_RECORD_EDIT_H_

#include <QGraphicsLayout>

class QGraphicsLinearLayout;
class LabeledTextEdit;
class MLabel;
class MComboBox;

class TextRecordEdit : public QObject, public QGraphicsLayout
{

	Q_OBJECT;

	Q_INTERFACES(QGraphicsLayout);

public:

	TextRecordEdit(const QStringList availableLanguages,
		       const QString initialLanguage,
		       const QString initialContents = "",
		       QGraphicsLayoutItem *parent = 0);

	virtual ~TextRecordEdit(void);

	virtual void setGeometry(const QRectF &rect);

	virtual QSizeF sizeHint(Qt::SizeHint which, 
				const QSizeF &constraint = QSizeF()) const;

	virtual int count(void) const;

	virtual QGraphicsLayoutItem *itemAt(int i) const;

	virtual void removeAt(int index);

	const QString language(void) const;

	const QString contents(void) const;

	void setContents(const QString contents);

	int size(void) const;

private Q_SLOTS:

	void textChanged(void);

	void languageChanged(void);

private:

	Q_DISABLE_COPY(TextRecordEdit);

	QGraphicsLinearLayout *m_layout;

	LabeledTextEdit *m_text;
	
	MLabel *m_sizeLabel;

	MComboBox *m_langCombo;

	int m_size;

	void updateSize(void);

};

#endif /* _TEXT_RECORD_EDIT_H_ */
