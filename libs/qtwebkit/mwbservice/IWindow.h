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

#ifndef IWindow_h
#define IWindow_h

#include <binder/IInterface.h>
#include <string>

class PageMemoryStats;

class IWindow: public android::IInterface {
public:
    DECLARE_META_INTERFACE(Window)

    /**
     * Ask the window to load the given page
     * Note: asynchronous
     * @param url url of the page to load
     */
    virtual void loadURL(const std::string& url) = 0;

    /**
     * Ask the window to stop the current loading process
     * Note: asynchronous
     */
    virtual void stopLoading() = 0;

    /**
     * Ask the window to go back according to the navigation history
     * Note: asynchronous
     */
    virtual void goBack() = 0;

    /**
     * Ask the window to go forward according to the navigation history
     * Note: asynchronous
     */
    virtual void goForward() = 0;

    /**
     * Get the window title
     * Note: synchronous
     */
    virtual std::string getTitle() = 0;

    /**
     * Close the window
     * Note: asynchronous
     */
    virtual void close() = 0;

    /**
     * Get the id of the window
     * Note: synchronous
     */
    virtual int getWindowId() = 0;

    /**
     * Set the geometry of the window
     * Note: asynchronous
     */
    virtual void setRect(int x, int y, int width=-1, int height=-1) = 0; // -1 means unchanged

    /**
     * Set the window focused
     * Note: asynchronous
     */
    virtual void setFocus(bool focused) = 0;

    /**
     * Repaint the window
     * Note: asynchronous
     */
    virtual void repaint() = 0;

    /**
     * Get memory statistics of window webview
     */
    virtual PageMemoryStats getMemoryStatistics() = 0;
};


class BnWindow: public android::BnInterface<IWindow> {
public:
    virtual android::status_t onTransact(uint32_t code, const android::Parcel& data, android::Parcel* reply, uint32_t flags = 0);
};


#endif // IWindow_h
