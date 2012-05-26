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
