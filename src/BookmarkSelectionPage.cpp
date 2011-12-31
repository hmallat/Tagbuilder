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

#include <QSparqlConnection>
#include <QSparqlQuery>
#include <QSparqlResult>
#include <QSparqlError>

#include <MDebug>

static MAbstractCellCreator<MWidgetController> *_getCreator(void)
{
	return new BookmarkSelectionPageListCellCreator;
}

BookmarkSelectionPage::BookmarkSelectionPage(QGraphicsItem *parent)
	: SelectionPage(parent),
	  m_model(new BookmarkSelectionPageListModel(this)),
	  m_connection(new QSparqlConnection("QTRACKER",
					     QSparqlConnectionOptions(),
					     this))
{
	QSparqlQuery q = QSparqlQuery("select ?b where { ?b a nfo:Bookmark }");
	m_result = m_connection->exec(q);
	if (m_result == 0 || m_result->hasError() == true) {
		mDebug(__func__) << "Failed to execute query: "
				 << ((m_result == 0) 
				     ? "Null result"
				     : m_result->lastError().message());
	} else {
		connect(m_result, SIGNAL(finished()),
			this, SLOT(queryFinished()));
	}
}

BookmarkSelectionPage::~BookmarkSelectionPage(void)
{
}

void BookmarkSelectionPage::queryFinished(void)
{
	if (m_result->hasError() == true) {
		mDebug(__func__) << "Failed to execute query: "
				 << m_result->lastError().message();
	} else {
		mDebug(__func__) << "Query returned " 
				 << m_result->size() 
				 << " rows. ";
		m_result->first();
		while (m_result->isValid() == true) {
			QString s = m_result->stringValue(0);
			mDebug(__func__) << s;
			m_result->next();
		}
	}
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
}

void BookmarkSelectionPage::bookmarkSelected(const QModelIndex &which)
{
	dismiss();
	Q_EMIT(selected(m_model->bookmark(which)));
}
