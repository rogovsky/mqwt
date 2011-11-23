/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_ITEM_H
#define QWT_PLOT_ITEM_H

#include "qwt_global.h"
#include "qwt_text.h"
#include "qwt_legend_data.h"
#include <qrect.h>
#include <qlist.h>

class QPainter;
class QwtScaleMap;
class QwtScaleDiv;
class QwtPlot;

/*!
  \brief Base class for items on the plot canvas

  A plot item is "something", that can be painted on the plot canvas,
  or only affects the scales of the plot widget. They can be categorized as:

  - Representator\n
    A "Representator" is an item that represents some sort of data
    on the plot canvas. The different representator classes are organized
    according to the characteristics of the data:
    - QwtPlotMarker
      Represents a point or a horizontal/vertical coordinate
    - QwtPlotCurve
      Represents a series of points
    - QwtPlotSpectrogram ( QwtPlotRasterItem )
      Represents raster data
    - ...

  - Decorators\n
    A "Decorator" is an item, that displays additional information, that
    is not related to any data:
    - QwtPlotGrid
    - QwtPlotScaleItem
    - QwtPlotSvgItem
    - ...

  Depending on the QwtPlotItem::ItemAttribute flags, an item is included
  into autoscaling or has an entry on the legnd.

  Before misusing the existing item classes it might be better to
  implement a new type of plot item
  ( don't implement a watermark as spectrogram ).
  Deriving a new type of QwtPlotItem primarily means to implement
  the YourPlotItem::draw() method.

  \sa The cpuplot example shows the implementation of additional plot items.
*/

class QWT_EXPORT QwtPlotItem
{
public:
    /*!
        \brief Runtime type information

        RttiValues is used to cast plot items, without
        having to enable runtime type information of the compiler.
     */
    enum RttiValues
    {
        //! Unspecific value, that can be used, when it doesn't matter
        Rtti_PlotItem = 0,

        //! For QwtPlotGrid
        Rtti_PlotGrid,

        //! For QwtPlotScaleItem
        Rtti_PlotScale,

        //! For QwtPlotLegendItem
        Rtti_PlotLegend,

        //! For QwtPlotMarker
        Rtti_PlotMarker,

        //! For QwtPlotCurve
        Rtti_PlotCurve,

        //! For QwtPlotSpectroCurve
        Rtti_PlotSpectroCurve,

        //! For QwtPlotIntervalCurve
        Rtti_PlotIntervalCurve,

        //! For QwtPlotHistogram
        Rtti_PlotHistogram,

        //! For QwtPlotSpectrogram
        Rtti_PlotSpectrogram,

        //! For QwtPlotSvgItem
        Rtti_PlotSVG,

        //! For QwtPlotTradingCurve
        Rtti_PlotTradingCurve,

        //! For QwtPlotBarChart
        Rtti_PlotBarChart,

        //! For QwtPlotMultiBarChart
        Rtti_PlotMultiBarChart,
        /*! 
           Values >= Rtti_PlotUserItem are reserved for plot items
           not implemented in the Qwt library.
         */
        Rtti_PlotUserItem = 1000
    };

    /*!
       \brief Plot Item Attributes

       Various aspects of a plot widget depend on the attributes of
       the attached plot items. If and how a single plot item 
       participates in these updates depends on its attributes.
       
       \sa setItemAttribute(), testItemAttribute(), ItemInterest
     */
    enum ItemAttribute
    {
        //! The item is represented on the legend.
        Legend = 0x01,

        /*!
           The boundingRect() of the item is included in the
           autoscaling calculation.
         */
        AutoScale = 0x02,

        /*!
           The item needs extra space to display something outside
           its bounding rectangle. 
           \sa getCanvasMarginHint()
         */
        Margins = 0x04
    };

    //! Plot Item Attributes
    typedef QFlags<ItemAttribute> ItemAttributes;

    /*!
       \brief Plot Item Interests

       Plot items might depend on the situation of the corresponding
       plot widget. By enabling an interest the plot item will be
       notified, when the corrsponding attribute of the plot widgets
       has changed.

       \sa setItemAttribute(), testItemAttribute(), ItemInterest
     */
    enum ItemInterest
    {
        /*! 
           The item is interested in updates of the scales
           \sa updateScaleDiv()
         */
        ScaleInterest = 0x01,

        /*! 
           The item is interested in updates of the legend ( of other items )
           This flag is intended for items, that want to implement a legend
           for displaying entries of other plot item.

           \note If the plot item wants to be represented on a legend
                 enable QwtPlotItem::Legend instead.

           \sa updateLegend()
         */
        LegendInterest = 0x02
    };

    //! Plot Item Interests
    typedef QFlags<ItemInterest> ItemInterests;

    //! Render hints
    enum RenderHint
    {
        //! Enable antialiasing
        RenderAntialiased = 0x1,

        /*!
          Certain painter operations are faster, when using integers
          than floats ( f.e raster paint engine: drawPolyline() ) - even
          if the corresponding paint engine doesn't support floats at all.
          So a plot item might decide to transform coordinates into integers 
          for these paint engines.

          By setting RenderFloats this optimization can be suppressed.

          \note For paint engines, that can use floats ( SVG, PDF ) this
                flag has no effect, because these are rendered always
                as floats.

          \sa QwtPainter::roundingAlignment()
         */
        RenderFloats = 0x2
    };

    //! Render hints
    typedef QFlags<RenderHint> RenderHints;

    explicit QwtPlotItem( const QwtText &title = QwtText() );
    virtual ~QwtPlotItem();

    void attach( QwtPlot *plot );
    void detach();

    QwtPlot *plot() const;

    void setTitle( const QString &title );
    void setTitle( const QwtText &title );
    const QwtText &title() const;

    virtual int rtti() const;

    void setItemAttribute( ItemAttribute, bool on = true );
    bool testItemAttribute( ItemAttribute ) const;

    void setItemInterest( ItemInterest, bool on = true );
    bool testItemInterest( ItemInterest ) const;

    void setRenderHint( RenderHint, bool on = true );
    bool testRenderHint( RenderHint ) const;

    void setLegendIdentifierSize( const QSize & );
    QSize legendIdentifierSize() const;

    double z() const;
    void setZ( double z );

    void show();
    void hide();
    virtual void setVisible( bool );
    bool isVisible () const;

    void setAxes( int xAxis, int yAxis );

    void setXAxis( int axis );
    int xAxis() const;

    void setYAxis( int axis );
    int yAxis() const;

    virtual void itemChanged();
    virtual void legendChanged();

    /*!
      \brief Draw the item

      \param painter Painter
      \param xMap Maps x-values into pixel coordinates.
      \param yMap Maps y-values into pixel coordinates.
      \param canvasRect Contents rect of the canvas in painter coordinates
    */
    virtual void draw( QPainter *painter,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect ) const = 0;

    virtual QRectF boundingRect() const;

    virtual void getCanvasMarginHint( 
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasSize,
        double &left, double &top, double &right, double &bottom) const;

    virtual void updateScaleDiv( 
        const QwtScaleDiv&, const QwtScaleDiv& );

    virtual void updateLegend( const QwtPlotItem *,
        const QList<QwtLegendData> & );

    QRectF scaleRect( const QwtScaleMap &, const QwtScaleMap & ) const;
    QRectF paintRect( const QwtScaleMap &, const QwtScaleMap & ) const;

    virtual QList<QwtLegendData> legendData() const;

    virtual void drawLegendIdentifier( int index,
        QPainter *, const QRectF & ) const;

private:
    // Disabled copy constructor and operator=
    QwtPlotItem( const QwtPlotItem & );
    QwtPlotItem &operator=( const QwtPlotItem & );

    class PrivateData;
    PrivateData *d_data;
};

Q_DECLARE_OPERATORS_FOR_FLAGS( QwtPlotItem::ItemAttributes )
Q_DECLARE_OPERATORS_FOR_FLAGS( QwtPlotItem::ItemInterests )
Q_DECLARE_OPERATORS_FOR_FLAGS( QwtPlotItem::RenderHints )

#endif