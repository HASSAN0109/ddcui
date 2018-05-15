/* features_scrollarea_contents.h
 *
 * <copyright>
 * Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * </endcopyright>
 */

#ifndef FEATURES_SCROLLAREA_CONTENTS_H_
#define FEATURES_SCROLLAREA_CONTENTS_H_

#include "nongui/feature_value.h"
#include "nongui/feature_base_model.h"

#include "feature_scrollarea/feature_widget.h"
#include "unused/page_change_observer.h"

class QScrollArea;
class QVBoxLayout;


class FeaturesScrollAreaContents : public QWidget {
    Q_OBJECT

public:
    FeaturesScrollAreaContents(QWidget *parent = Q_NULLPTR);
    virtual ~FeaturesScrollAreaContents();

    void setModel(FeatureBaseModel * baseModel);
    void setContainingScrollArea(QScrollArea * scrollArea);

    // void addPageChangeObserver(PageChangeObserver * observer);
    // void notifyPageChangeObservers(int pageno);

signals:
    void showCentralWidgetByWidget(QWidget * widget);
    // void showCentralWidgetByPageno(int pageno);

public slots:
#ifdef MAYBE_UNUSED
   void featureAdded(FeatureValue fv);
#endif
#ifdef OLD
   void featureUpdated(char feature_code);
#endif
   void startInitialLoad(void);
   void endInitialLoad();

private:
   const char *        _cls;    // className
   FeatureBaseModel *  _baseModel = NULL;
   QScrollArea *       _containingScrollArea = NULL;
   QVector<PageChangeObserver*> * _pageChangeObservers;

   // quick and dirty for now, eventually replace by hash
   FeatureWidget * _widgets[256] = {0};
};

#endif /* FEATURES_SCROLLAREA_CONTENTS_H_ */