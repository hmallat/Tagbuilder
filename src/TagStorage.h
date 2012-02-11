/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _TAG_STORAGE_H_
#define _TAG_STORAGE_H_

#include <MAbstractItemModel>
#include <QNdefMessage>

class Tag;
class QString;

QTM_USE_NAMESPACE;

class TagStorage : public MAbstractItemModel
{

	class TagStorageImpl;

public:

	static const int NULL_TAG;

	static const int TEMPORARY_TAG;

	static TagStorage *storage(void);

	int count(void) const;

	const Tag *tag(int which) const;

	bool append(const QString &name,
		    const QNdefMessage &message);

	bool update(int which, 
		    const QString &name,
		    const QNdefMessage &message);
	
	bool remove(int which);

	virtual int groupCount(void) const;

	virtual int rowCountInGroup(int group) const;

	virtual QString groupTitle(int group) const;

	virtual QVariant itemData(int row, 
				  int group, 
				  int role = Qt::DisplayRole) const;

private:

	Q_DISABLE_COPY(TagStorage);

	TagStorage(TagStorageImpl *impl, QObject *parent = 0);

	~TagStorage(void);

	TagStorageImpl *m_impl;

};

#endif /* _TAG_STORAGE_H_ */
