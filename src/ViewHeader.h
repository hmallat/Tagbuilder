/*
 * This file is part of NFC tag creator
 *
 * Copyright (C) 2011 Hannu Mallat. All rights reserved. 
 * Contact: Hannu Mallat <hmallat@gmail.com>
 *
 */

#ifndef _VIEW_HEADER_H_
#define _VIEW_HEADER_H_

#include <MStylableWidget>

class MLabel;
class MProgressIndicator;
class QGraphicsLayout;

class ViewHeader : public MStylableWidget
{

	Q_OBJECT;

public:

	ViewHeader(const QString text, QGraphicsItem *parent = 0);

public Q_SLOTS:

	void setBusy(void);

	void clearBusy(void);

protected:

	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);

private:

	Q_DISABLE_COPY(ViewHeader);

	QGraphicsLayout *createLayout(void);

	MLabel *labelWidget(void);

	MProgressIndicator *progressWidget(void);

	QString m_text;

	MLabel *m_label;

	MProgressIndicator *m_progress;

	QGraphicsLayout *m_layout;

};

#endif /* _VIEW_HEADER_H_ */
