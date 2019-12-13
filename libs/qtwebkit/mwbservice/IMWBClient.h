/*
 * Copyright (C) 2012 MStar Semiconductor, Inc.  All rights reserved.
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

#ifndef IMWBClient_h
#define IMWBClient_h

#include <binder/IInterface.h>
#include <string>

// ----------------------------------------------------------------------------

class IMWBClient: public android::IInterface {
public:
    DECLARE_META_INTERFACE(MWBClient);

    /**
     * Fired when a new window is created in WebKit
     * \param windowId Id of the newly created window
     */
    virtual void notifyNewWindow(int windowId) = 0;

    /**
     * Fired when a window has been closed
     * @param windowId Id of the closed window
     */
    virtual void notifyWindowClosed(int windowId) = 0;

    /**
     * Fired when a window has started loading an URL
     * @param windowId Id of the window
     * @param url url of the page being loaded
     */
    virtual void notifyLoadStarted(int windowId, const std::string& url) = 0;

    /**
     * Inform the loading progress of a window
     * @param windowId Id of the window
     * @param progress loading progress from 0 to 100 (percentage)
     */
    virtual void notifyLoadProgress(int windowId, int progress) = 0;

    /**
     * Fired when a page load has been completed
     * @param windowId Id of the window
     */
    virtual void notifyLoadComplete(int windowId) = 0;

    /**
     * Fired when a page load has failed
     * @param windowId Id of the window
     * @param error
     * @param statusCode
     */
    virtual void notifyLoadFailed(int windowId, const std::string& error, int statusCode) = 0;

    /**
     * Fired when the window is painted
     * @param windowId Id of the window
     * @param x
     * @param y
     * @param width
     * @param height
     */
    virtual void notifyWindowPainted(int windowId, int x, int y, int w, int h) = 0;

    /**
    * Notify client about key event processing result
    * @param keyEventId unique id identifying the key event
    * @param consumed indicate whether the key event has been consumed
    */
    virtual void notifyKeyEventConsumed(int keyEventId, bool consumed) = 0;

    /**
     * Fired when a page/frame is laid out the first time
     * @param windowId Id of the window
     */
    virtual void notifyInitialLayoutCompleted(int windowId) = 0;
};


// ----------------------------------------------------------------------------

class BnMWBClient: public android::BnInterface<IMWBClient> {
public:
    virtual android::status_t onTransact(uint32_t code,
                                         const android::Parcel& data,
                                         android::Parcel* reply,
                                         uint32_t flags = 0);
};

#endif /* IMWBClient_h */
