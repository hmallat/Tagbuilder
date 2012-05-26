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

#include "OrganizerItemHandler.h"

#include <QOrganizerItemLocation>
#include <QVersitProperty>

#include <MDebug>

OrganizerItemHandler::OrganizerItemHandler(void)
{
}

OrganizerItemHandler::~OrganizerItemHandler()
{
}

void 
OrganizerItemHandler::detailProcessed(const QOrganizerItem &item, 
				      const QOrganizerItemDetail &detail, 
				      const QVersitDocument &document, 
				      QSet<QString> *processedFields, 
				      QList<QVersitProperty> *toBeRemoved, 
				      QList<QVersitProperty> *toBeAdded)
{
	Q_UNUSED(item); 
	Q_UNUSED(document);
	Q_UNUSED(toBeRemoved);

	mDebug(__func__) << "Detail=" << detail.definitionName();
	for(QSet<QString>::const_iterator i = processedFields->begin();
	    i != processedFields->end();
	    i++) {
		QString field = *i;
		mDebug(__func__) << "Processed=" << field;
	}

	if (detail.definitionName() == QOrganizerItemLocation::DefinitionName &&
	    processedFields->size() == 0) {
		/* Default exporter has not processed Location */
		QOrganizerItemLocation location = 
			static_cast<QOrganizerItemLocation>(detail);

		QVersitProperty prop;
		prop.setName("LOCATION");
		prop.setValueType(QVersitProperty::PlainType);
		prop.setValue(QVariant(location.label()));
		toBeAdded->append(prop);
	}
}

void OrganizerItemHandler::itemProcessed(const QOrganizerItem &item, 
					 QVersitDocument *document)
{
	Q_UNUSED(item); 
	Q_UNUSED(document);
}

void 
OrganizerItemHandler::propertyProcessed(const QVersitDocument &document, 
					const QVersitProperty &property, 
					const QOrganizerItem &item, 
					bool *alreadyProcessed, 
					QList<QOrganizerItemDetail> *updatedDetails)
{
	Q_UNUSED(document);
	Q_UNUSED(item);

	if (property.name() != "LOCATION" || 
	    property.valueType() != QVersitProperty::PlainType ||
	    *alreadyProcessed == true) {
		return;
	}

	QOrganizerItemLocation location;
	location.setLabel(property.variantValue().toString());
	updatedDetails->append(location);
	*alreadyProcessed = true;
}

void 
OrganizerItemHandler::subDocumentProcessed(const QVersitDocument &topLevel, 
					   const QVersitDocument &subDocument, 
					   QOrganizerItem *item)
{
	Q_UNUSED(topLevel);
	Q_UNUSED(subDocument);
	Q_UNUSED(item);
}

