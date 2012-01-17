/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _ORGANIZER_ITEM_HANDLER_H_
#define _ORGANIZER_ITEM_HANDLER_H_

#include <QVersitOrganizerExporterDetailHandler>
#include <QVersitOrganizerImporterPropertyHandler>

QTM_USE_NAMESPACE;

class OrganizerItemHandler : 
public QVersitOrganizerExporterDetailHandler,
public QVersitOrganizerImporterPropertyHandler
{

public:

	OrganizerItemHandler(void);

	virtual ~OrganizerItemHandler();

	virtual void detailProcessed(const QOrganizerItem &item, 
				     const QOrganizerItemDetail &detail, 
				     const QVersitDocument &document, 
				     QSet<QString> *processedFields, 
				     QList<QVersitProperty> *toBeRemoved, 
				     QList<QVersitProperty> *toBeAdded);

	virtual void itemProcessed(const QOrganizerItem &item, 
				   QVersitDocument *document);

	virtual void propertyProcessed(const QVersitDocument &document, 
				       const QVersitProperty &property, 
				       const QOrganizerItem &item, 
				       bool *alreadyProcessed, 
				       QList<QOrganizerItemDetail> *updatedDetails);

	virtual void subDocumentProcessed(const QVersitDocument &topLevel, 
					  const QVersitDocument &subDocument, 
					  QOrganizerItem *item);

};

#endif /* _ORGANIZER_ITEM_HANDLER_H_ */
