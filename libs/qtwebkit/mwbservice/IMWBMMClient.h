/*
 * Copyright (C) 2013 MStar Semiconductor, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of MStar nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY MSTAR AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL MSTAR OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef IMWBMMClient_h
#define IMWBMMClient_h

#include <binder/IInterface.h>
#include <vector>

class MultimediaComponentData;

// ----------------------------------------------------------------------------

class IMWBMMClient: public android::IInterface {
public:
    DECLARE_META_INTERFACE(MWBMMClient);

    /**
    * Callback function, WEBKIT can Update Multiple-Audio-Language information to Requesters
    * when signal changed on Gstreamer (note that this function will be called with an empty vector
    * when video playback stop, this indicate that no audio selection is available)
    * @param components : a vector of MultimediaComponentData available in the current stream
    * @param currentComponentIndex: index of currently used MultimediaComponentData in the vector (-1 if no component used)
    **/
    virtual void notifyAudioComponentsChanged(const std::vector<MultimediaComponentData>& components, int currentComponentIndex) = 0;

    /**
    * Callback function, WEBKIT can Update Subtitle information to Requesters
    * when signal changed on Gstreamer (note that this function will be called with an empty vector
    * when video playback stop, this indicate that no subtitle selection is available)
    * @param components : a vector of MultimediaComponentData available in the current stream
    * @param currentComponentIndex: index of currently used MultimediaComponentData in the vector (-1 if no component used)
    **/
    virtual void notifySubtitleComponentsChanged(const std::vector<MultimediaComponentData>& components, int currentComponentIndex) = 0;
};


// ----------------------------------------------------------------------------

class BnMWBMMClient: public android::BnInterface<IMWBMMClient> {
public:
    virtual android::status_t onTransact(uint32_t code,
                                         const android::Parcel& data,
                                         android::Parcel* reply,
                                         uint32_t flags = 0);
};

#endif /* IMWBMMClient_h */
