/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CREATE_TEXT_PAGE_H_
#define _CREATE_TEXT_PAGE_H_

#include <QString>
#include <QSystemInfo>
#include <MApplicationPage>

class MTextEdit;
class MLabel;
class MComboBox;

QTM_USE_NAMESPACE;

class CreateTextPage : public MApplicationPage
{
	Q_OBJECT;

public:

	CreateTextPage(QGraphicsItem *parent = 0);

	~CreateTextPage(void);

        virtual void createContent(void);

private Q_SLOTS:

	void textChanged(void);

	void languageChanged(void);

private:

	Q_DISABLE_COPY(CreateTextPage);

	MTextEdit *m_text;

	MLabel *m_sizeLabel;

	MComboBox *m_langCombo;

	QSystemInfo *m_sysinfo;

	void updateSize(void);

};

#endif /* _CREATE_TEXT_PAGE_H_ */
