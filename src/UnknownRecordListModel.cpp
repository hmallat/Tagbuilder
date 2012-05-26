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

#include "UnknownRecordListModel.h"

#include <QStringList>

UnknownRecordListModel::UnknownRecordListModel(QObject *parent)
	: MAbstractItemModel(parent)
{
	setGrouped(false);
}

void UnknownRecordListModel::setMessage(const QNdefMessage &message)
{
	Q_EMIT(layoutAboutToBeChanged());

	if (m_message.length() > 0) {
		beginRemoveRows(QModelIndex(), 0, m_message.length() - 1, 
				Util::animateLists);
		m_message.clear();
		endRemoveRows();
	}

	if (message.length() > 0) {
		beginInsertRows(QModelIndex(), 0, message.length() - 1, 
				Util::animateLists);
		m_message = message;
		endInsertRows();
	}

	Q_EMIT(layoutChanged());
}

int UnknownRecordListModel::groupCount(void) const
{
	return 0;
}

int UnknownRecordListModel::rowCountInGroup(int group) const
{
	(void) group;
	return m_message.length();
}

QString UnknownRecordListModel::groupTitle(int group) const
{
	(void) group;
	return "";
}

QVariant UnknownRecordListModel::itemData(int row, 
					  int group, 
					  int role) const
{
	(void) role;
	(void) group;

	QStringList parameters;

	QString rtype;
	QNdefRecord::TypeNameFormat tnf = 
		m_message[row].typeNameFormat();
	if (tnf == QNdefRecord::Empty) {
		rtype.append("empty");
	} else if (tnf == QNdefRecord::NfcRtd) {
		rtype.append("urn:nfc:wkt:");
		rtype.append(QString::fromAscii(m_message[row].type()));
	} else if (tnf == QNdefRecord::Mime) {
		rtype.append("mime:");
		rtype.append(QString::fromAscii(m_message[row].type()));
	} else if (tnf == QNdefRecord::Uri) {
		rtype.append("uri:");
		rtype.append(QString::fromAscii(m_message[row].type()));
	} else if (tnf == QNdefRecord::ExternalRtd) {
		rtype.append("urn:nfc:ext:");
		rtype.append(QString::fromAscii(m_message[row].type()));
	} else if (tnf == QNdefRecord::Unknown) {
		rtype.append("unknown");
	} else {
		rtype.append("unspecified");
	}
	parameters << rtype;

	QByteArray data = m_message[row].payload();
	parameters << tr("%1 bytes data").arg(data.length());

	return qVariantFromValue(parameters);
}
