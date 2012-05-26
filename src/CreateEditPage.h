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

	void setDefaultName(const QString name);

	bool isTemporary(void);

protected Q_SLOTS:

	void setContentValidity(bool);

#ifdef LABEL_SIZE
	void setContentSize(quint32 bytes);
#endif

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

#ifdef LABEL_SIZE
	MLabel *m_size;
#endif

	LabeledTextEdit *m_name;

	bool m_contentValidity;

	bool m_nameValidity;

	bool m_nameEdited;

};

#endif /* _CREATE_EDIT_PAGE_H_ */
