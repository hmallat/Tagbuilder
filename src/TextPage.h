/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TEXT_PAGE_H_
#define _TEXT_PAGE_H_

#include <QString>
#include <QSystemInfo>
#include <MApplicationPage>

class Tag;
class TextRecordEdit;
class LabeledTextEdit;

QTM_USE_NAMESPACE;

class TextPage : public MApplicationPage
{
	Q_OBJECT;

public:

	TextPage(Tag *tag = 0,
		 QGraphicsItem *parent = 0);

	~TextPage(void);

        virtual void createContent(void);

private Q_SLOTS:

	void nameChanged(void);

	void storeTag(void);

private:

	Q_DISABLE_COPY(TextPage);

	Tag* m_tag;

	LabeledTextEdit *m_name;

	bool m_nameFocused;

	TextRecordEdit *m_edit;

	QSystemInfo *m_sysinfo;

	MAction *m_cancelAction;

	MAction *m_storeAction;

};

#endif /* _TEXT_PAGE_H_ */
