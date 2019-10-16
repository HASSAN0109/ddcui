/* vcprequest.cpp - VcpRequestQueue and the classes that populate it */

// Copyright (C) 2018 Sanford Rockowitz <rockowitz@minsoft.com>
// SPDX-License-Identifier: GPL-2.0-or-later

// #include <string.h>

#include "nongui/vcprequest.h"

/* VcpRequest and subclasses */

VcpRequest::VcpRequest(VcpRequestType type)
    : _type(type)
{
     // _type = type;
}

VcpRequest::~VcpRequest()
{
}


VcpCapRequest::VcpCapRequest()
    : VcpRequest::VcpRequest(VcpRequestType::RQCapabilities)
{
}

LoadDfrRequest::LoadDfrRequest()
    : VcpRequest::VcpRequest(VcpRequestType::RQLoadDfr)
{
}



VcpGetRequest::VcpGetRequest(DDCA_Vcp_Feature_Code featureCode, bool reportUnsupported)
    : VcpRequest::VcpRequest(VcpRequestType::RQGetVcp)
    , _featureCode(featureCode)
    , _reportUnsupported(reportUnsupported)
{
    // _featureCode = feature_code;
}


VcpSetRequest::VcpSetRequest(DDCA_Vcp_Feature_Code featureCode, uint8_t newSh, uint8_t newSl, bool writeOnly)
    :  VcpRequest::VcpRequest(VcpRequestType::RQSetVcp)
    , _featureCode(featureCode)
    , _newSh(newSh)
    , _newSl(newSl)
    , _writeOnly(writeOnly)
{
    // _featureCode = feature_code;
    // _newval = newval;
    // _writeOnly = writeOnly;
}


VcpStartInitialLoadRequest::VcpStartInitialLoadRequest(void)
    : VcpRequest::VcpRequest(VcpRequestType::RQStartInitialLoad)
{
}


VcpEndInitialLoadRequest::VcpEndInitialLoadRequest(void)
    : VcpRequest::VcpRequest(VcpRequestType::RQEndInitialLoad)
{
}


/* VcpRequestQueue */

VcpRequestQueue::VcpRequestQueue()
{
    _queue = QQueue<VcpRequest*>();
}

void VcpRequestQueue::put(VcpRequest * request) {
    _mutex.lock();
    _queue.enqueue(request);
    _queueNonempty.wakeOne();
    _mutex.unlock();
}

VcpRequest * VcpRequestQueue::pop() {
    _mutex.lock();
    if (_queue.empty())
        _queueNonempty.wait(&_mutex);

    VcpRequest * rqst = _queue.dequeue();
    // to do: scan queue for superceding requests
    // if (this->m_queue.empty())
    // do something with queueNonempty?

    _mutex.unlock();
    return rqst;
}

void VcpRequestQueue::dbgrpt() {
   _mutex.lock();
   int ct = _queue.size();
   printf("Queue contains %d entries", ct);
   for (int ndx = 0; ndx < ct; ndx++) {
      VcpRequest * rqst = _queue.at(ndx);
      printf("   type: %d\n", rqst->_type);

   }
    _mutex.unlock();

}

// VcpRequest VcpRequestQueue::peek() { }

