/* value_base_widget.cpp */

// Copyright (C) 2018-2022 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_base_widget.h"

#include <QtCore/QMargins>

#include "base/core.h"
#include "base/global_state.h"
#include "base/feature_selector.h"

#include "ddcutil_c_api.h"

// bool showValueWidgetResizeEvents = false;

// utility method that provides consistent formatting for comboboxes
QComboBox * ValueBaseWidget::createFormattedComboBox() {
   QComboBox * cb = new QComboBox();

   QSizePolicy cbSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
   cbSizePolicy.setControlType(QSizePolicy::ComboBox);
   cb->setSizePolicy(cbSizePolicy);
   cb->setFont(FeatureValueComboBoxFont);

   // cb->setMaximumHeight(15);  // clips
   cb->setMaximumWidth(320);
   cb->setMinimumWidth(80);

   // _cb->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method
   cb->setStyleSheet("background-color:white;color:black;padding-left:2px;");
   cb->setContentsMargins(0,0,0,0);

   return cb;
}

int ValueBaseWidget::nextId = 0;

ValueBaseWidget::ValueBaseWidget(QWidget *parent)
     : QFrame(parent)             // ValueAbstractWidget(parent)
{
   bool debug = false;
   _cls = strdup(metaObject()->className());  // private to this class
   _id = ++nextId;

   TRACEMCF(debug, "Executing. this._id = %d", _id);
   //QMargins margins = contentsMargins();
   //printf("(ValueBaseWidget::ValueBaseWidget) margins: left=%d, top=%d, right=%d, bottom=%d)\n",
   //       margins.m_left, margins.m_top, margins.m_right, margins.m_bottom);

   // try setting frame style at this level,
   setFrameStyle(QFrame::Sunken | QFrame::Panel);

   TRACEMCF(debug, "Done");
}

ValueBaseWidget::~ValueBaseWidget() {
   bool debug = false;
   TRACEMCF(debug, "Executing. this._id = %d", _id);
   ddca_free_feature_metadata(_finfo);
   // _capVcp ??
   // _dref do not need to free
   free((void*)_cls);
   // delete _cls;  // fails - const
   TRACEMCF(debug, "Done.");
}


void ValueBaseWidget::setFeatureValue(const FeatureValue &fv) {
   bool debug = false;
   TRACEMCF(debug, "ValueBaseWidget. this._id=%d, featureCode=0x%02x, capVcp=%p, ddcrc=%d",
                    _id, fv.featureCode(), fv.capVcp(), fv.ddcrc());

    _featureCode    = fv.featureCode();
    _dref           = fv.dref();
    _finfo          = fv.finfo();
    _capVcp         = fv.capVcp();
    _vspec          = fv.vspec();
    _mh             = fv.val().mh;
    _ml             = fv.val().ml;
    _sh             = fv.val().sh;
    _sl             = fv.val().sl;
    _ddcrc          = fv.ddcrc();
}


#ifdef UNNEEDED
// hack to give ValueNcWidget access to parsed capabilities
void ValueBaseWidget::setBaseModel(FeatureBaseModel * model) {
   printf("(%s::%s) feature_code=0x%02x, model=%p\n",
         _cls, __func__, _featureCode, model); fflush(stdout);
   _baseModel = model;
}
#endif


void ValueBaseWidget::setCurrentShSl(uint16_t newval) {
    _sh = newval >> 8;
    _sl = newval & 0xff;
}


uint16_t ValueBaseWidget::getCurrentShSl() {
    uint16_t result = (_sh << 8) | _sl;
    return result;
}


#ifdef NO
QSize ValueBaseWidget::sizeHint() const {
    int w = 300;
    int h = 20;
    printf("============> (ValueBaseWidget::sizeHint) returning (%d,%d)\n", w,h ); fflush(stdout);
    return QSize(w,h);
}
#endif

#ifdef NO
void ValueBaseWidget::resizeEvent(QResizeEvent * evt)
{
   bool show = false;

   QSize oldSz = evt->oldSize();
   QSize newSz = evt->size();

   static bool resizeEventsShown = false;
   if (showValueWidgetResizeEvents && !resizeEventsShown) {
      show = true;
      resizeEventsShown = true;
   }

   if (show) {
      TRACEC("old size = %d, %d", oldSz.width(), oldSz.height());
      TRACEC("new size = %d, %d", newSz.width(), newSz.height());
   }

   evt->ignore();
}
#endif

