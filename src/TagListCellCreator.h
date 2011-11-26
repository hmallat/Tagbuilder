/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_LIST_CELL_CREATOR_H_
#define _TAG_LIST_CELL_CREATOR_H_

#include <MAbstractCellCreator>
#include <MContentItem>

class MainPage;

class TagListCellCreator : public MAbstractCellCreator<MContentItem>
{

public:

	TagListCellCreator(MainPage *page);

	void updateCell(const QModelIndex &index, MWidget *cell) const;

private:

	MainPage *m_page;

};

#endif /* _TAG_LIST_CELL_CREATOR_H_ */
