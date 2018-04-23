/* feature_selector.h */

#ifndef FEATURE_SELECTOR_H
#define FEATURE_SELECTOR_H

#include "ddcutil_types.h"

class FeatureSelector {
public:
   //  FeatureSelector();

    DDCA_Feature_Subset_Id _featureListId           = DDCA_SUBSET_KNOWN;
    bool                   _showUnsupportedFeatures = false;
    bool                   _includeTableFeatures    = false;
    bool                   _respectCapabilities     = false;

    bool operator==(const FeatureSelector& sel2) const;
    bool operator!=(const FeatureSelector& sel2) const;

    void dbgrpt();
};

#endif // FEATURE_SELECTOR_H
