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

#include "UnknownPage.h"
#include "UnknownRecordListModel.h"
#include "UnknownRecordListCellCreator.h"
#include "LabelOrList.h"
#include "Util.h"

#include <MLabel>
#include <MButton>
#include <MMessageBox>
#include <MContentItem>
#include <QGraphicsLinearLayout>

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new UnknownRecordListCellCreator;
}

UnknownPage::UnknownPage(int tag, QGraphicsItem *parent)
	: CreateEditPage(tag, parent),
	  m_model(new UnknownRecordListModel(this))
{
}

UnknownPage::~UnknownPage(void)
{
}

void UnknownPage::createPageSpecificActions(void)
{
}

void UnknownPage::createPageSpecificContent(void)
{
	MLabel *title = new MLabel(tr("This tag's contents are unsupported "
				      "and cannot be edited. "
				      "However you can still write the tag "
				      "from the main screen."));
	title->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
	title->setAlignment(Qt::AlignCenter);
	title->setWordWrap(true);
	layout()->addItem(title);
	layout()->setAlignment(title, Qt::AlignCenter);

	m_unknownDetails = new LabelOrList(m_model,
					   _getCreator,
					   tr("Empty message"),
					   false,
					   false);
	m_unknownDetails->setSizePolicy(QSizePolicy::Preferred, 
					QSizePolicy::Preferred);
	layout()->addItem(m_unknownDetails);
	layout()->setAlignment(m_unknownDetails, Qt::AlignCenter);
}

void UnknownPage::setupNewData(void) 
{
	m_message = QNdefMessage();
	m_model->setMessage(m_message);
#ifdef LABEL_SIZE
	updateSize();
#endif
}

bool UnknownPage::setupData(const QNdefMessage message)
{
	m_message = message;
	m_model->setMessage(m_message);
#ifdef LABEL_SIZE
	updateSize();
#endif
	return true;
}

QNdefMessage UnknownPage::prepareDataForStorage(void)
{
	return m_message;
}

#ifdef LABEL_SIZE
void UnknownPage::updateSize(void)
{
	setContentSize(Util::messageLength(m_message));
}
#endif
