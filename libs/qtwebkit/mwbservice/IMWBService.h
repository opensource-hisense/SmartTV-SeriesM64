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

#ifndef IMWBService_h
#define IMWBService_h

#include "IWindow.h"
#include "MWBWindowOptions.h"

#include <binder/IInterface.h>
#include <string>
#include <utils/KeyedVector.h>
#include <vector>

#define MWBSERVICE_NAME "com.mstar.MWBService"

class IMWBClient;
class IMWBMMClient;
class MemoryStats;
class MultimediaComponentData;

class IMWBService: public android::IInterface {
public:
    DECLARE_META_INTERFACE(MWBService);

    /**
     * Send a Key event to the browser process
     * @param keyEventId an id used to track Key event status send to events clients
     * @param keyCode a DFB keycode in DFBInputDeviceKeySymbol enum
     * @param isKeyPress indicate whether the event is a key press of not
     * @param modifiers indicate modifiers with a combination of DFBInputDeviceModifierMask flags
     * @param isRepeat indicate whether this is repeated key
     */
    virtual void sendKeyEvent(int keyEventId, int keyCode, bool isKeyPress, int modifiers = 0, bool isRepeat = false) = 0;

    /**
     * Register a client for Events notifications
     * @param client the client to be notified
     */
    virtual void addClient(const android::sp<IMWBClient>&) = 0;

    /**
     * Unregister a client that was previously registered for Events notifications
     * @param client the client to be unregistered
     */
    virtual void removeClient(const android::sp<IMWBClient>&) = 0;

    /**
     * Register a MM client for Events notifications
     * @param client the MM client to be notified
     */
    virtual void registerMMClient(const android::sp<IMWBMMClient>&) = 0;

    /**
     * Unregister a MM client that was previously registered for Events notifications
     * @param client the MM client to be unregistered
     */
    virtual void removeMMClient(const android::sp<IMWBMMClient>&) = 0;

    /**
     * Ask the WebKit engine to create a new window
     * Note: synchronous
     * @param url url of the page to load
     * @param x x coordinate of the window
     * @param y y coordinate of the window
     * @param width width of the window
     * @param height height of the window
     * @param options options of the window
     * @return the newly created window as an Object implementing the IWindow interface
     */
    virtual android::sp<IWindow> createWindow(const std::string& url, int x = -1, int y = -1, int width = -1, int height = -1, MWBWindowOptions * = 0) = 0;

    /**
     * Retrieve the current list of opened window in WebKit
     * Note: synchronous
     * @return a KeyedVector object that contains tuples {window id, IWindow}
     */
    virtual android::KeyedVector<int, android::sp<IWindow> > getWindowList() = 0;

    /**
     * Get a window from the given id
     * Note: synchronous
     * @param windowId the given window id
     * @return an object implementing IWindow if the window exists, 0 otherwise
     */
    virtual android::sp<IWindow> getWindowById(int windowId) = 0;

    /**
     * Ask the browser to be closed
     * Note: asynchronous
     */
    virtual void exitBrowser() = 0;

    /**
     * Get an environment variable from the browser process environment.
     * @param name of the environment variable
     * @return The environment variable value if set, an empty string otherwise.
     **/
    virtual std::string getBrowserEnv(const std::string& name) = 0;

    /**
     * Change or add an environment variable to the browser process environment.
     * The value is always overwritten for an existing environment variable.
     * @param name for the environment variable
     * @param value for the environment variable
     **/
    virtual void setBrowserEnv(const std::string& name, const std::string& value) = 0;

    /**
    * Request to switch to given audio component (can be issued by anyone ) accessing MWBService
    * @param index : selected component from requester
    * @ return: true if audio change succeeded, false otherwise
    **/
    virtual bool selectAudioComponent(const MultimediaComponentData&) = 0;

    /**
    * Request to switch to given subtitle component (can be issued by anyone ) accessing MWBService
    * @param index : selected component from requester
    * @ return: true if subtitle change succeeded, false otherwise
    **/
    virtual bool selectSubtitleComponent(const MultimediaComponentData&) = 0;

    /**
    * Request to remove the audio
    * @ return: true if audio change succeeded, false otherwise
    **/
    virtual bool removeAudio() = 0;

    /**
    * Request to remove the subtitles
    * @ return: true if subtitles change succeeded, false otherwise
    **/
    virtual bool removeSubtitles() = 0;

    /**
     * Request to show or hide the subtitles
     * @param enable true if the subtitles must be visible, false otherwise
     */
    virtual void setSubtitlesEnable(bool) = 0;

    /**
    * Request to get the audio components list and the current audio index
    * @param components : a vector of MultimediaComponentData available in the current stream
    * @param currentComponentIndex : index of currently used MultimediaComponentData in the vector (-1 if no component used)
    **/
    virtual void getAudioComponents(std::vector<MultimediaComponentData>& components, int& currentComponentIndex) = 0;

    /**
    * Request to get the subtitle components list and the current subtitle index
    * @param components : a vector of MultimediaComponentData available in the current stream
    * @param currentComponentIndex : index of currently used MultimediaComponentData in the vector (-1 if no component used)
    **/
    virtual void getSubtitleComponents(std::vector<MultimediaComponentData>& components, int& currentComponentIndex) = 0;

    /**
    * Stop all media players (synchronous call)
    * @return true if all media players have been released, false otherwise
    **/
    virtual bool forceStopAllMediaPlayers() = 0;

    /**
     * Get various memory statistics
     * @return XXX
     */
    virtual MemoryStats getMemoryStatistics() = 0;

    /**
     * Show or hide the web inspector
     * @param visible : true=shown/false=hided
     */
    virtual void setWebInspectorVisible(bool) = 0;

    /**
     * Trigger a screenshot of the active view
     * @param fullpath : the path where the screenshot will be save (folder + filename)
     */
    virtual void triggerScreenshot(const std::string& fullpath) = 0;

    /**
     * Change layer and window resolution
     * @param width : width of the window/layer
     * @param height : height of the window/layer
     */
    virtual void changeResolution(int width, int height) = 0;

    /**
     * setCookiesEnabled
     * @param visible : true=enable/false=disable
     */
    virtual void setCookiesEnabled(bool) = 0;

    /**
     * Set download path
     * @param path : path where all downloads should be saved
     */
    virtual void setDownloadPath(const std::string&) = 0;

    /**
     * add domain to HbbTV security whitelist
     * @param url url of the page to load
     */
    virtual void addDomainToHbbTVSecurityWhitelist(const std::string& url) = 0;

    /**
     * remove domain from HbbTV security whitelist
     * @param url url of the page to load
     */
    virtual void removeDomainFromHbbTVSecurityWhitelist(const std::string& url) = 0;

    /**
     * clear HbbTV security whitelist
     */
    virtual void clearHbbTVSecurityWhitelist() = 0;
};

class BnMWBService: public android::BnInterface<IMWBService> {
public:
    virtual android::status_t onTransact(uint32_t code, const android::Parcel& data,
            android::Parcel* reply, uint32_t flags = 0);
};

#endif /* IMWBService_h */
