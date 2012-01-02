/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#include "CreatePage.h"
#include "TextPage.h"
#include "TagTypeListModel.h"
#include "TagTypeListCellCreator.h"
#include "LabelOrList.h"
#include "UIUtil.h"

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new TagTypeListCellCreator;
}

CreatePage::CreatePage(QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_model(new TagTypeListModel(this))
{
}

CreatePage::~CreatePage(void)
{
}

void CreatePage::createContent(void)
{
	createCommonContent(m_model,
			    _getCreator,
			    tr("<h1>No tag type to select</h1>"),
			    tr("Select tag type"),
			    false,
			    false);

	connect(m_list, SIGNAL(itemClicked(const QModelIndex &)),
		this, SLOT(tagTypeSelected(const QModelIndex &)));
}

void CreatePage::tagTypeSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model != 0 ? m_model->name(which) : ""));
}

