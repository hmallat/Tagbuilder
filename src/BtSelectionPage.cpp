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

#include "BtSelectionPage.h"
#include "BtSelectionPageListCellCreator.h"
#include "BtSelectionPageExistingListModel.h"
#include "BtSelectionPageScanListModel.h"
#include "BluezSupplicant.h"
#include "LabelOrList.h"

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new BtSelectionPageListCellCreator;
}

BtSelectionPage::BtSelectionPage(BluezSupplicant *bluez,
				 enum BtSelectionPage::Type type,
				 QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_type(type),
	  m_model(0)
{
	m_model = (m_type == SelectFromExisting)
		? static_cast<BtSelectionPageListModel *>(new BtSelectionPageExistingListModel(bluez, this))
		: static_cast<BtSelectionPageListModel *>(new BtSelectionPageScanListModel(bluez, this));
	if (m_type == SelectFromScanned) {
		connect(m_model, SIGNAL(scanFailure(void)),
			this, SLOT(scanFailure(void)));
	}
}

BtSelectionPage::~BtSelectionPage(void)
{
}

void BtSelectionPage::createContent(void)
{
	QString info = 
		(m_type == SelectFromExisting)
		? tr("No Bluetooth devices to select from")
		: tr("Scanning for nearby Bluetooth devices ...");

	createCommonContent(m_model,
			    _getCreator,
			    info,
			    tr("Select the device to use"),
			    false,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(deviceSelected(const QModelIndex &)));

	if (m_type == SelectFromScanned) {
		setBusy(); /* Never clears */
	} else {
		BtSelectionPageExistingListModel *e_model =
			static_cast<BtSelectionPageExistingListModel *>(m_model);
		if (e_model->isReady() == false) {
			setBusy();
			connect(e_model, SIGNAL(ready()),
				this, SLOT(clearBusy()));
		}
	}
}

void BtSelectionPage::deviceSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->device(which)));
}

void BtSelectionPage::scanFailure(void)
{
	m_list->setLabel(tr("Cannot scan. Check that Bluetooth "
			    "is available. "));
	clearBusy();
}
