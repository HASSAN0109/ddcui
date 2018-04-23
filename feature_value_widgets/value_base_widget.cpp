/* value_base_widget.cpp */

#include "feature_value_widgets/value_base_widget.h"
#include "feature_value_widgets/value_abstract_widget.h"

#include <QtCore/QMargins>

ValueBaseWidget::ValueBaseWidget(QWidget *parent)
     : ValueAbstractWidget(parent)
{
   // will this get subclass name or "ValueBaseWidget"?  ValueBaseWidget
   _cls                    = strdup(metaObject()->className());

   // setFrameStyle(QFrame::Box);
   // int m_left, m_right, m_top, m_bottom;
   // getContentsMargins(&m_left, &m_top, &m_right, &m_bottom);
   // printf("(ValueBaseWidget::ValueBaseWidget) Before reset, margins: left=%d, top=%d, right=%d, bottom=%d)\n",
   //        m_left, m_top, m_right, m_bottom);
   // returns 1,1,1,1
   //                 l  t  r   b
   setContentsMargins(0, 0, 0, 0);
   // getContentsMargins(&m_left, &m_top, &m_right, &m_bottom);
   // printf("(ValueBaseWidget::ValueBaseWidget) After reset, margins: left=%d, top=%d, right=%d, bottom=%d)\n",
   //        m_left, m_top, m_right, m_bottom);


   //QMargins margins = contentsMargins();
   //printf("(ValueBaseWidget::ValueBaseWidget) margins: left=%d, top=%d, right=%d, bottom=%d)\n",
   //       margins.m_left, margins.m_top, margins.m_right, margins.m_bottom);

   // try setting frame style at this level,
   setFrameStyle(QFrame::Sunken | QFrame::Panel);
}


void ValueBaseWidget::setFeatureValue(const FeatureValue &fv) {
   printf("(%s::%s) feature_code=0x%02x\n", _cls, __func__, fv.featureCode()); fflush(stdout);
    _feature_code    = fv.featureCode();
    _dref            = fv.dref();
    _finfo           = fv.finfo();
    // _vspec           = fv._vspec;
    // _feature_flags   = fv._feature_flags;
    _vspec           = fv.vspec();
    _feature_flags   = fv.flags();
    _mh              = fv._value.mh;
    _ml              = fv._value.ml;
    _sh              = fv._value.sh;
    _sl              = fv._value.sl;
    _value           = fv._value;
}

// hack to give ValueNcWidget access to parsed capabilities
void ValueBaseWidget::setBaseModel(FeatureBaseModel * model) {
   printf("(%s::%s) feature_code=0x%02x, model=%p\n",
         _cls, __func__, _feature_code, model); fflush(stdout);
   _baseModel = model;
}


void ValueBaseWidget::setCurrentValue(uint16_t newval) {
    _sh = newval >> 8;
    _sl = newval & 0xff;
    _value.sh = _sh;
    _value.sl = _sl;
}


uint16_t ValueBaseWidget::getCurrentValue() {
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
