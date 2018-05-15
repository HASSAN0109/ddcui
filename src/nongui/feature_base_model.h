/* feature_base_model.h - Records all VCP feature values for a single monitor */

/* <copyright>
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

#ifndef FEATURE_BASE_MODEL_H
#define FEATURE_BASE_MODEL_H

#include <QtCore/QObject>

#include "ddcutil_types.h"

#include "nongui/ddc_error.h"
#ifdef FEATURE_CHANGE_OBSERVER
#include "nongui/feature_change_observer.h"
#endif
#include "nongui/feature_value.h"
#include "nongui/vcprequest.h"

// typedef void (*NotifyFeatureChanged)(uint8_t feature_code);

class Monitor;

/** The UI independent portion of the QT feature data model.
 *  Records all VCP values for a single monitor.
 */
class FeatureBaseModel : public QObject
{
    Q_OBJECT

public:
    FeatureBaseModel(Monitor * monitor);

    FeatureValue *
         modelVcpValueFind(uint8_t feature_code);
    FeatureValue *
         modelVcpValueFilteredFind(uint8_t feature_code);
    FeatureValue *
         modelVcpValueAt(int ndx) const;
    int  modelVcpValueCount(void) const;
    void modelVcpValueSet(
             uint8_t                     feature_code,
             DDCA_Display_Ref            dref,   // unnecessary, can get from monitor
             DDCA_Feature_Metadata       metadata,
             DDCA_Non_Table_Vcp_Value *  feature_value);
    void modelVcpValueUpdate(
             uint8_t                     feature_code,
             uint8_t                     sh,
             uint8_t                     sl);
    void modelMccsVersionSet(DDCA_MCCS_Version_Spec vspec);
    DDCA_MCCS_Version_Spec
         mccsVersionSpec();
    void modelStartInitialLoad(void);
    void modelEndInitialLoad(void);
    void setStatusMsg(QString msg);
    void setCapabilities(
             DDCA_Status          ddcrc,
             char *               capabilities_string,
             DDCA_Capabilities *  parsed_capabilities);
    void dbgrpt();
#ifdef FEATURE_CHANGE_OBSERVER
 // void addFeatureChangedObserver(NotifyFeatureChanged func);
    void addFeatureChangeObserver(FeatureChangeObserver *observer);
#endif
    void setFeatureList(DDCA_Feature_List featureList);
    void setFeatureChecked(uint8_t featureCode);

    // *** Public Member Variables ***
    const char *        _cls;    // className

    Monitor *           _monitor;
    DDCA_Feature_List   _featuresToShow;
    DDCA_Feature_List   _featuresChecked;

    DDCA_Status         _caps_status = -999;    // a value that's undefined
    char *              _caps_string = NULL;
    DDCA_Capabilities * _parsed_caps = NULL;

signals:
    void signalStartInitialLoad(void);
    void signalEndInitialLoad(void);
    void signalFeatureAdded(FeatureValue fv);
    // void signalFeatureUpdated(char feature_code);
    void signalFeatureUpdated3(const char * caller, uint8_t feature_code, uint8_t sh, uint8_t sl);
    void signalVcpRequest(VcpRequest * rqst);  // used to call into monitor
    void signalModelError(uint8_t featureCode, QString msg);
    void signalDdcError(DdcError erec);
    void signalStatusMsg(QString msg);

public slots:
    void onDdcError(DdcError& erec);

protected:
    void notifyFeatureChangeObservers(uint8_t feature_code) ;

private:
    int modelVcpValueIndex(uint8_t feature_code);

    QVector<FeatureValue*> * _featureValues;
    DDCA_MCCS_Version_Spec   _vspec;

#ifdef FEATURE_CHANGE_OBSERVER
    QVector<FeatureChangeObserver*> * _featureChangeObservers;
#endif
};

#endif // FEATURE_BASE_MODEL_H