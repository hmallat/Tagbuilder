/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _CONTACT_DETAIL_PICKER_LIST_CELL_CREATOR_H_
#define _CONTACT_DETAIL_PICKER_LIST_CELL_CREATOR_H_

#include "ContentItemCellCreator.h"

class ContactDetailPickerListCellCreator : public ContentItemCellCreator
{

public:

	ContactDetailPickerListCellCreator(const QString styleName = "CommonContentItem");

	virtual void updateCell(const QModelIndex &index, MWidget *cell) const;

};

#endif /* _CONTACT_DETAIL_PICKER_LIST_CELL_CREATOR_H_ */
