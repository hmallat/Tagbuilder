/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

/* Common parent for all create/edit pages; contains common components
   and methods. */ 

#ifndef _CREATE_EDIT_PAGE_H_
#define _CREATE_EDIT_PAGE_H_

#include <MApplicationPage>

#include <QNdefMessage>
#include "TagStorage.h"

class MAction;
class MLabel;
class LabeledTextEdit;
class QGraphicsLinearLayout;

QTM_USE_NAMESPACE;

class CreateEditPage : public MApplicationPage
{

	Q_OBJECT;

public:

	CreateEditPage(int tag = TagStorage::NULL_TAG, 
		       QGraphicsItem *parent = 0);

	virtual ~CreateEditPage(void);

	virtual void createContent(void);

protected:

	virtual void createPageSpecificActions(void) = 0;

	virtual void createPageSpecificContent(void) = 0;

	virtual void setupNewData(void) = 0;

	virtual bool setupData(const QNdefMessage message) = 0;

	virtual QNdefMessage prepareDataForStorage(void) = 0;

	QGraphicsLinearLayout *layout(void);

protected Q_SLOTS:

	void setContentValidity(bool);

	void setContentSize(quint32 bytes);

private Q_SLOTS:

	void nameChanged(void);

	void load();

	void storeAndExit(void);
	
	void setNameValidity(bool);

private:

	Q_DISABLE_COPY(CreateEditPage);

	void createActions(void);

	void updateUI(void);

	int m_tag;

	MAction *m_cancelAction;

	MAction *m_storeAction;

	QGraphicsLinearLayout *m_layout;

	MLabel *m_size;

	LabeledTextEdit *m_name;

	bool m_contentValidity;

	bool m_nameValidity;

};

#endif /* _CREATE_EDIT_PAGE_H_ */
