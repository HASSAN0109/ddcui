/* value_2button_widget.cpp - Widget containing 2 named buttons */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

#include "feature_value_widgets/value_2button_widget.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <QtCore/QRect>
#include <QtGui/QPaintEvent>
#include <QtGui/QRegion>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLayout>
#include <QtWidgets/QPushButton>

#include "base/ddcui_globals.h"
#include "base/debug_utils.h"

#include <ddcutil_c_api.h>


static bool dimensionReportShown = false;

Value2ButtonWidget::Value2ButtonWidget(
     QWidget *parent)
    : ValueBaseWidget(parent)
    , _val1(0)      // set in setButtonDetail()
    , _val2(0)
{
   _cls = strdup(metaObject()->className());
   // TRACE("Starting." );

   QFont nonMonoFont;
   nonMonoFont.setPointSize(9);

   _button1 = new QPushButton();
   _button1->setMaximumSize(60,20);

   _button2 = new QPushButton();
   _button2->setMaximumSize(60,20);

    QSizePolicy* sizePolicy = new QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    // _cb->setHorizontalStretch(0);
    _button1->setSizePolicy(*sizePolicy);
    _button1->setFont(nonMonoFont);
    // _cb->setFrameStyle(QFrame::Sunken | QFrame::Panel);   // not a method
    _button1->setStyleSheet("background-color:white;");
    _button2->setSizePolicy(*sizePolicy);
    _button2->setFont(nonMonoFont);
    _button2>setStyleSheet("background-color:white;color:black;");

    QHBoxLayout * layout = new QHBoxLayout();
    layout->addWidget(_button1);
    layout->addWidget(_button2);
    layout->addStretch(1);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    // setVisible(true);

    if (!dimensionReportShown  && debugLayout ) {
        printf("(%s::%s) ------------------------------------------>\n", _cls, __func__); fflush(stdout);
        reportWidgetDimensions(_button1, _cls, __func__, "_button1");
        reportWidgetDimensions(this, _cls, __func__, "Value2ButtonWidget");
        dimensionReportShown = true;
    }
    if (debugLayout)
       this->setStyleSheet("background-color:cyan;");

    QObject::connect(_button1,  SIGNAL(released()),
                     this,      SLOT(  on_button1_pressed()) );
    QObject::connect(_button2,  SIGNAL(released()),
                     this,      SLOT(  on_button2_pressed()) );

    delete sizePolicy;
}


void Value2ButtonWidget::setButtonDetail(
      QString   name1,
      uint8_t   val1,
      QString   name2,
      uint8_t   val2)
{
   _val1  = val1;
   _val2  = val2;
   _button1->setText(name1);
   _button2->setText(name2);
   // setVisible(true);
}


void Value2ButtonWidget::on_button1_pressed() {
   TRACEF(debugValueWidgetSignals, "Button 1 pressed. Emitting featureValueChanged");
   emit featureValueChanged(_featureCode, 0, _val1);
}


void Value2ButtonWidget::on_button2_pressed() {
   TRACEF(debugValueWidgetSignals, "Button 2 pressed. Emitting featureValueChanged");
   emit featureValueChanged(_featureCode, 0, _val2);
}


