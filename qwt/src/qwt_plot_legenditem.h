/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_LEGEND_ITEM_H
#define QWT_PLOT_LEGEND_ITEM_H

#include "qwt_global.h"
#include "qwt_plot_item.h"
#include "qwt_legend_data.h"

class QFont;

class QWT_EXPORT QwtPlotLegendItem: public QwtPlotItem
{
public:
    enum BackgroundMode
    {
        ItemBackground,
        LegendBackground
    };

    explicit QwtPlotLegendItem();
    virtual ~QwtPlotLegendItem();

    virtual int rtti() const;

    void setAlignment( Qt::Alignment );
    Qt::Alignment alignment() const;

    void setMaxColumns( uint );
    uint maxColumns() const;

    void setFont( const QFont& );
    QFont font() const;

    void setBorderDistance( int numPixels );
    int borderDistance() const;

    void setBorderRadius( double );
    double borderRadius() const;

    void setBorderPen( const QPen & );
    QPen borderPen() const;

    void setBackgroundBrush( const QBrush & );
    QBrush backgroundBrush() const;

    void setBackgroundMode( BackgroundMode );
    BackgroundMode backgroundMode() const;

    void setTextPen( const QPen & );
    QPen textPen() const;

    virtual void draw( QPainter *p,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &rect ) const;

    void clearLegend();

    virtual void updateLegend( const QwtPlotItem *,
        const QList<QwtLegendData> & );

    QRect geometry( const QRectF &canvasRect ) const;

private:
    void drawBackground( QPainter *, const QRectF &rect ) const;

    class PrivateData;
    PrivateData *d_data;
};

#endif