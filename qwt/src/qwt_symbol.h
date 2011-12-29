/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_SYMBOL_H
#define QWT_SYMBOL_H

#include "qwt_global.h"
#include <QPolygonF>

class QPainter;
class QPainterPath;
class QRect;
class QSize;
class QBrush;
class QPen;
class QColor;
class QPointF;

//! A class for drawing symbols
class QWT_EXPORT QwtSymbol
{
public:
    /*!
      Symbol Style
      \sa setStyle(), style()
     */
    enum Style
    {
        //! No Style. The symbol cannot be drawn.
        NoSymbol = -1,

        //! Ellipse or circle
        Ellipse,

        //! Rectangle
        Rect,

        //!  Diamond
        Diamond,

        //! Triangle pointing upwards
        Triangle,

        //! Triangle pointing downwards
        DTriangle,

        //! Triangle pointing upwards
        UTriangle,

        //! Triangle pointing left
        LTriangle,

        //! Triangle pointing right
        RTriangle,

        //! Cross (+)
        Cross,

        //! Diagonal cross (X)
        XCross,

        //! Horizontal line
        HLine,

        //! Vertical line
        VLine,

        //! X combined with +
        Star1,

        //! Six-pointed star
        Star2,

        //! Hexagon
        Hexagon,

        /*!
          The symbol is represented by a painter path, where the 
          origin ( 0, 0 ) of the path coordinate system is mapped to
          the position of the symbol.

          The following code defines a symbol drawing an arrow:

  \verbatim
#include <qwt_symbol.h>

QwtSymbol *symbol = new QwtSymbol();

QPen pen( Qt::black, 2 );
pen.setJoinStyle( Qt::MiterJoin );

symbol->setPen( pen );
symbol->setBrush( Qt::red );

QPainterPath path;
path.moveTo( 0, 8 );
path.lineTo( 0, 5 );
path.lineTo( -3, 5 );
path.lineTo( 0, 0 );
path.lineTo( 3, 5 );
path.lineTo( 0, 5 );

QTransform transform;
transform.rotate( -30.0 );
path = transform.map( path );

symbol->setPath( path );

setSize( 10, 14 );
\endverbatim

          \sa setPath(), path()
         */
        Path,

        /*!
         Styles >= QwtSymbol::UserSymbol are reserved for derived
         classes of QwtSymbol that overload drawSymbols() with
         additional application specific symbol types.
         */
        UserStyle = 1000
    };

    /*!
      Depending on the render engine and the complexity of the
      symbol shape it might be faster to render the symbol
      to a pixmap and to paint this pixmap.

      F.e. the raster paint engine is a pure software renderer
      where in cache mode a draw operation usually ends in 
      raster operation with the the backing store, that are usually
      faster, than the algorithms for rendering polygons.
      But the opposite can be expected for graphic pipelines
      that can make use of hardware acceleration.

      The default setting is AutoCache

      \sa setCachePolicy(), cachePolicy()

      \note The policy has no effect, when the symbol is painted 
            to a vector graphics format ( PDF, SVG ).
      \warning Since Qt 4.8 raster is the default backend on X11
     */

    enum CachePolicy
    {
        //! Don't use a pixmap cache
        NoCache,

        //! Always use a pixmap cache
        Cache,

        /*! 
           Use a cache wehn one of the following condiditions is true:

           - The symbol is rendered with the software 
             renderer ( QPaintEngine::Raster )
         */
        AutoCache
    };

public:
    QwtSymbol( Style = NoSymbol );
    QwtSymbol( Style, const QBrush &, const QPen &, const QSize & );
    QwtSymbol( const QPainterPath &, const QBrush &, const QPen & );

    QwtSymbol( const QwtSymbol & );

    virtual ~QwtSymbol();

    QwtSymbol &operator=( const QwtSymbol & );
    bool operator==( const QwtSymbol & ) const;
    bool operator!=( const QwtSymbol & ) const;

    void setCachePolicy( CachePolicy );
    CachePolicy cachePolicy() const;

    void setSize( const QSize & );
    void setSize( int width, int height = -1 );
    const QSize& size() const;

    virtual void setColor( const QColor & );

    void setBrush( const QBrush& b );
    const QBrush& brush() const;

    void setPen( const QPen & );
    const QPen& pen() const;

    void setStyle( Style );
    Style style() const;

    void setPath( const QPainterPath & );
    const QPainterPath &path() const;

    void drawSymbol( QPainter *, const QPointF & ) const;
    void drawSymbols( QPainter *, const QPolygonF & ) const;

    virtual QRect boundingRect() const;
    void invalidateCache();

protected:
    void drawSymbols( QPainter *,
        const QPointF *, int numPoints ) const;

    virtual void renderSymbols( QPainter *,
        const QPointF *, int numPoints ) const;

private:
    class PrivateData;
    PrivateData *d_data;
};

/*!
  \brief Draw the symbol at a specified position

  \param painter Painter
  \param pos Position of the symbol in screen coordinates
*/
inline void QwtSymbol::drawSymbol(
    QPainter *painter, const QPointF &pos ) const
{
    drawSymbols( painter, &pos, 1 );
}

/*!
  \brief Draw symbols at the specified points

  \param painter Painter
  \param points Positions of the symbols in screen coordinates
*/

inline void QwtSymbol::drawSymbols(
    QPainter *painter, const QPolygonF &points ) const
{
    drawSymbols( painter, points.data(), points.size() );
}

#endif
