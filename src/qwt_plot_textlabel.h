/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_TEXT_LABEL_H
#define QWT_PLOT_TEXT_LABEL_H 1

#include "qwt_global.h"
#include "qwt_plot_item.h"
#include "qwt_text.h"

/*!
  \brief A plot item, which displays a text label

  QwtPlotTextLabel displays a text label aligned to the plot canvas.

  In opposite to QwtPlotMarker the position of the label is unrelated to
  plot coordinates.

  As drawing a text is an expensive operation the label is cached
  in a pixmap to speed up replots.

  \par Example
  The following code shows how to add a title.

\verbatim
    QwtText title( "Plot Title" );
    title.setRenderFlags( Qt::AlignHCenter | Qt::AlignTop );

    QFont font;
    font.setBold( true );
    title.setFont( font );

    QwtPlotTextLabel *titleItem = new QwtPlotTextLabel();
    titleItem->setText( title );
    titleItem->attach( this );
\endverbatim

  \sa QwtPlotMarker
*/

class QWT_EXPORT QwtPlotTextLabel: public QwtPlotItem
{
public:
    QwtPlotTextLabel();
    virtual ~QwtPlotTextLabel();

    virtual int rtti() const;

    void setText( const QwtText & );
    QwtText text() const;

    void setMargin( int margin );
    int margin() const;

    virtual QRectF textRect( const QRectF &, const QSizeF & ) const;

protected:
    virtual void draw( QPainter *,
        const QwtScaleMap &, const QwtScaleMap &,
        const QRectF &) const;

    void invalidateCache();

private:
    class PrivateData;
    PrivateData *d_data;
};

#endif
