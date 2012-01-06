/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "BookmarkSelectionPage.h"
#include "BookmarkSelectionPageListCellCreator.h"
#include "BookmarkSelectionPageListModel.h"
#include "LabelOrList.h"

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new BookmarkSelectionPageListCellCreator;
}

BookmarkSelectionPage::BookmarkSelectionPage(QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_model(new BookmarkSelectionPageListModel(this))
{
}

BookmarkSelectionPage::~BookmarkSelectionPage(void)
{
}

void BookmarkSelectionPage::createContent(void)
{
	createCommonContent(m_model,
			    _getCreator,
			    tr("<h1>No bookmarks to select</h1>"),
			    tr("Select the bookmark to use"),
			    false,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(bookmarkSelected(const QModelIndex &)));

	connect(m_model, SIGNAL(ready()), this, SLOT(clearBusy()));
	if (m_model->fetch() == true) {
		setBusy();
	} else {
		/* TODO: message box or some other indication */
	}
}

void BookmarkSelectionPage::bookmarkSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->bookmark(which)));
}
