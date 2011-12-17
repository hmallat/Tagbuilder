/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _URL_PAGE_H
#define _URL_PAGE_H_

#include "CreateEditPage.h"

#include <QSystemInfo>

class QSignalMapper;
class TextRecordEdit;
class LabeledTextEdit;
class MButton;
class MContainer;

QTM_USE_NAMESPACE;

class UrlPage : public CreateEditPage
{

	Q_OBJECT;

	enum Action {
		NoAction,
		DoAction,
		SaveAction,
		EditAction
	};

public:

	UrlPage(int tag = -1, QGraphicsItem *parent = 0);

	~UrlPage(void);

protected:

	virtual void createPageSpecificContent(void);

	virtual void setupNewData(void);

	virtual bool setupData(const QNdefMessage message);

	virtual QNdefMessage prepareDataForStorage(void);

private Q_SLOTS:

	void urlChanged(void);

	void titleChanged(QObject *which);

	void addTitle(void);

	void removeTitle(QObject *which);

	void actChanged(void);

private:

	Q_DISABLE_COPY(UrlPage);

	void updateSize(void);

	enum Action checkedAction(void);

	QSystemInfo *m_sysinfo;

	QSignalMapper *m_titleRemoveMapper;

	QSignalMapper *m_titleChangeMapper;

	LabeledTextEdit *m_url;

	MButton *m_addTitle;

	QGraphicsLinearLayout *m_titleLayout;

	QList<TextRecordEdit *> m_titles;

	MButton *actButton[4];

};

#endif /* _URL_PAGE_H_ */
