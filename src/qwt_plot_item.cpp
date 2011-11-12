/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_plot_item.h"
#include "qwt_text.h"
#include "qwt_plot.h"
#include "qwt_legend_data.h"
#include "qwt_scale_div.h"
#include <qpainter.h>

static QPixmap qwtLegendIdentifier( 
    const QwtPlotItem *item, int index, const QSize &size )
{
    QPixmap pm( size );
    pm.fill( Qt::transparent );

    QPainter painter( &pm );
    painter.setRenderHint( QPainter::Antialiasing,
        item->testRenderHint( QwtPlotItem::RenderAntialiased ) );

    item->drawLegendIdentifier( index, &painter,
        QRect( 0, 0, size.width(), size.height() ) );

    painter.end();

    return pm;
}

class QwtPlotItem::PrivateData
{
public:
    PrivateData():
        plot( NULL ),
        isVisible( true ),
        attributes( 0 ),
        renderHints( 0 ),
        z( 0.0 ),
        xAxis( QwtPlot::xBottom ),
        yAxis( QwtPlot::yLeft ),
        legendIconSize( 8, 8 )
    {
    }

    mutable QwtPlot *plot;

    bool isVisible;
    QwtPlotItem::ItemAttributes attributes;
    QwtPlotItem::RenderHints renderHints;
    double z;

    int xAxis;
    int yAxis;

    QwtText title;
    QSize legendIconSize;
};

/*!
   Constructor
   \param title Title of the item
*/
QwtPlotItem::QwtPlotItem( const QwtText &title )
{
    d_data = new PrivateData;
    d_data->title = title;
}

//! Destroy the QwtPlotItem
QwtPlotItem::~QwtPlotItem()
{
    attach( NULL );
    delete d_data;
}

/*!
  \brief Attach the item to a plot.

  This method will attach a QwtPlotItem to the QwtPlot argument. It will first
  detach the QwtPlotItem from any plot from a previous call to attach (if
  necessary). If a NULL argument is passed, it will detach from any QwtPlot it
  was attached to.

  \param plot Plot widget
  \sa detach()
*/
void QwtPlotItem::attach( QwtPlot *plot )
{
    if ( plot == d_data->plot )
        return;

    if ( d_data->plot )
        d_data->plot->attachItem( this, false );

    d_data->plot = plot;
    if ( d_data->plot )
        d_data->plot->attachItem( this, true );
}

/*!
   \brief This method detaches a QwtPlotItem from any 
          QwtPlot it has been associated with.

   detach() is equivalent to calling attach( NULL )
   \sa attach()
*/
void QwtPlotItem::detach()
{
    attach( NULL );
}

/*!
   Return rtti for the specific class represented. QwtPlotItem is simply
   a virtual interface class, and base classes will implement this method
   with specific rtti values so a user can differentiate them.

   The rtti value is useful for environments, where the
   runtime type information is disabled and it is not possible
   to do a dynamic_cast<...>.

   \return rtti value
   \sa RttiValues
*/
int QwtPlotItem::rtti() const
{
    return Rtti_PlotItem;
}

//! Return attached plot
QwtPlot *QwtPlotItem::plot() const
{
    return d_data->plot;
}

/*!
   Plot items are painted in increasing z-order.

   \return setZ(), QwtPlotDict::itemList()
*/
double QwtPlotItem::z() const
{
    return d_data->z;
}

/*!
   \brief Set the z value

   Plot items are painted in increasing z-order.

   \param z Z-value
   \sa z(), QwtPlotDict::itemList()
*/
void QwtPlotItem::setZ( double z )
{
    if ( d_data->z != z )
    {
        if ( d_data->plot ) // update the z order
            d_data->plot->attachItem( this, false );

        d_data->z = z;

        if ( d_data->plot )
            d_data->plot->attachItem( this, true );

        itemChanged();
    }
}

/*!
   Set a new title

   \param title Title
   \sa title()
*/
void QwtPlotItem::setTitle( const QString &title )
{
    setTitle( QwtText( title ) );
}

/*!
   Set a new title

   \param title Title
   \sa title()
*/
void QwtPlotItem::setTitle( const QwtText &title )
{
    if ( d_data->title != title )
    {
        d_data->title = title;
        itemChanged();
    }
}

/*!
   \return Title of the item
   \sa setTitle()
*/
const QwtText &QwtPlotItem::title() const
{
    return d_data->title;
}

/*!
   Toggle an item attribute

   \param attribute Attribute type
   \param on true/false

   \sa testItemAttribute(), ItemAttribute
*/
void QwtPlotItem::setItemAttribute( ItemAttribute attribute, bool on )
{
    if ( bool( d_data->attributes & attribute ) != on )
    {
        if ( on )
            d_data->attributes |= attribute;
        else
            d_data->attributes &= ~attribute;

        itemChanged();
    }
}

/*!
   Test an item attribute

   \param attribute Attribute type
   \return true/false
   \sa setItemAttribute(), ItemAttribute
*/
bool QwtPlotItem::testItemAttribute( ItemAttribute attribute ) const
{
    return ( d_data->attributes & attribute );
}

/*!
   Toggle an render hint

   \param hint Render hint
   \param on true/false

   \sa testRenderHint(), RenderHint
*/
void QwtPlotItem::setRenderHint( RenderHint hint, bool on )
{
    if ( ( ( d_data->renderHints & hint ) != 0 ) != on )
    {
        if ( on )
            d_data->renderHints |= hint;
        else
            d_data->renderHints &= ~hint;

        itemChanged();
    }
}

/*!
   Test a render hint

   \param hint Render hint
   \return true/false
   \sa setRenderHint(), RenderHint
*/
bool QwtPlotItem::testRenderHint( RenderHint hint ) const
{
    return ( d_data->renderHints & hint );
}

void QwtPlotItem::setLegendIdentifierSize( const QSize &size )
{
    d_data->legendIconSize = size;
}

QSize QwtPlotItem::legendIdentifierSize() const
{
    return d_data->legendIconSize;
}

void QwtPlotItem::drawLegendIdentifier( int index,
    QPainter *painter, const QRectF &rect ) const
{
    Q_UNUSED( index );
    Q_UNUSED( painter );
    Q_UNUSED( rect );
}

//! Show the item
void QwtPlotItem::show()
{
    setVisible( true );
}

//! Hide the item
void QwtPlotItem::hide()
{
    setVisible( false );
}

/*!
    Show/Hide the item

    \param on Show if true, otherwise hide
    \sa isVisible(), show(), hide()
*/
void QwtPlotItem::setVisible( bool on )
{
    if ( on != d_data->isVisible )
    {
        d_data->isVisible = on;
        itemChanged();
    }
}

/*!
    \return true if visible
    \sa setVisible(), show(), hide()
*/
bool QwtPlotItem::isVisible() const
{
    return d_data->isVisible;
}

/*!
   Update the legend and call QwtPlot::autoRefresh for the
   parent plot.

   \sa updateLegend()
*/
void QwtPlotItem::itemChanged()
{
    if ( d_data->plot )
    {
        d_data->plot->updateLegend( this );
        d_data->plot->autoRefresh();
    }
}

/*!
   Set X and Y axis

   The item will painted according to the coordinates its Axes.

   \param xAxis X Axis
   \param yAxis Y Axis

   \sa setXAxis(), setYAxis(), xAxis(), yAxis()
*/
void QwtPlotItem::setAxes( int xAxis, int yAxis )
{
    if ( xAxis == QwtPlot::xBottom || xAxis == QwtPlot::xTop )
        d_data->xAxis = xAxis;

    if ( yAxis == QwtPlot::yLeft || yAxis == QwtPlot::yRight )
        d_data->yAxis = yAxis;

    itemChanged();
}

/*!
   Set the X axis

   The item will painted according to the coordinates its Axes.

   \param axis X Axis
   \sa setAxes(), setYAxis(), xAxis()
*/
void QwtPlotItem::setXAxis( int axis )
{
    if ( axis == QwtPlot::xBottom || axis == QwtPlot::xTop )
    {
        d_data->xAxis = axis;
        itemChanged();
    }
}

/*!
   Set the Y axis

   The item will painted according to the coordinates its Axes.

   \param axis Y Axis
   \sa setAxes(), setXAxis(), yAxis()
*/
void QwtPlotItem::setYAxis( int axis )
{
    if ( axis == QwtPlot::yLeft || axis == QwtPlot::yRight )
    {
        d_data->yAxis = axis;
        itemChanged();
    }
}

//! Return xAxis
int QwtPlotItem::xAxis() const
{
    return d_data->xAxis;
}

//! Return yAxis
int QwtPlotItem::yAxis() const
{
    return d_data->yAxis;
}

/*!
   \return An invalid bounding rect: QRectF(1.0, 1.0, -2.0, -2.0)
*/
QRectF QwtPlotItem::boundingRect() const
{
    return QRectF( 1.0, 1.0, -2.0, -2.0 ); // invalid
}

void QwtPlotItem::getCanvasMarginHint( const QwtScaleMap &xMap, 
    const QwtScaleMap &yMap, const QRectF &canvasRect,
    double &left, double &top, double &right, double &bottom ) const
{
    Q_UNUSED( xMap );
    Q_UNUSED( yMap );
    Q_UNUSED( canvasRect );

    // use QMargins, when we don't need to support Qt < 4.6 anymore
    left = top = right = bottom = 0.0;
}

QList<QwtLegendData> QwtPlotItem::legendData() const
{
    QwtLegendData data;
            
    QVariant titleValue;
    qVariantSetValue( titleValue, title() );
    data.setValue( QwtLegendData::TitleRole, titleValue );
        
    if ( !legendIdentifierSize().isEmpty() )
    {   
        data.setValue( QwtLegendData::IconRole, 
            qwtLegendIdentifier( this, 0, legendIdentifierSize() ) );
    }   
        
    QList<QwtLegendData> list;
    list += data;

    return list;
}

/*!
   \brief Update the item to changes of the axes scale division

   Update the item, when the axes of plot have changed.
   The default implementation does nothing, but items that depend
   on the scale division (like QwtPlotGrid()) have to reimplement
   updateScaleDiv()

   \param xScaleDiv Scale division of the x-axis
   \param yScaleDiv Scale division of the y-axis

   \sa QwtPlot::updateAxes()
*/
void QwtPlotItem::updateScaleDiv( const QwtScaleDiv &xScaleDiv,
    const QwtScaleDiv &yScaleDiv )
{
    Q_UNUSED( xScaleDiv );
    Q_UNUSED( yScaleDiv );
}

/*!
   \brief Calculate the bounding scale rect of 2 maps

   \param xMap X map
   \param yMap X map

   \return Bounding scale rect of the scale maps, normalized
*/
QRectF QwtPlotItem::scaleRect( const QwtScaleMap &xMap,
    const QwtScaleMap &yMap ) const
{
    return QRectF( xMap.s1(), yMap.s1(),
        xMap.sDist(), yMap.sDist() );
}

/*!
   \brief Calculate the bounding paint rect of 2 maps

   \param xMap X map
   \param yMap X map

   \return Bounding paint rect of the scale maps, normalized
*/
QRectF QwtPlotItem::paintRect( const QwtScaleMap &xMap,
    const QwtScaleMap &yMap ) const
{
    const QRectF rect( xMap.p1(), yMap.p1(),
        xMap.pDist(), yMap.pDist() );

    return rect;
}