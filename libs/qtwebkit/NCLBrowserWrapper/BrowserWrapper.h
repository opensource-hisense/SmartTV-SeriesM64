
#ifndef BrowserWrapper_h
#define BrowserWrapper_h

#include "NclBrowserUpdateListener.h"

#include <directfb.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

/** Initialize the browser engine
 *
 */
void initBrowserEngine();

/** Shutdown the browser engine
 *
 */
void shutdownBrowserEngine();

/** Creates a new browser to display HTML content
 *
 * \return The ID of the created browser.
 */
int createBrowser();

/** Closes the specified browser releasing any allocated resources
 *
 * \param browserID The desired browser identification
 */
void closeBrowser(int id);

/** Loads the specified URL. Using the specified browser
 *
 * \param browserID The browser identification
 * \param url The url to be loaded
 * \return http error code
 */
int loadBrowserURL(int id, std::string url);

/** Adjusts the presentation size of the browser
 *
 * \param browserID The browser dientification
 * \param width The new width
 * \param height The new height
 */
void setBrowserSize(int id, int width, int height);

/** Sends an input event to the given browser
 *
 * \param browserID The browser identification
 * \param eventType The event type. "0" for key pressed, "1" for key released
 * \param keyCode The corresponding key code
 */
void sendBrowserEvent(int id, int eventType, int keyCode);

/** Retrieve the browser's surface with the rendered content. This function will only be called
 *      after the first call of graphicsUpdated from the BrowserUpdateListener instance
 *
 * \param browserID The browser identification
 * \return the surface of the browser with the rendered content
 */
IDirectFBSurface* getBrowserSurface(int id);

/** Adds a listener to handles the graphics changes in the browser's surface
 *
 * \param browserID The browser identification
 * \param listener a instance of a browser listener to be notified when the browser graphics changes
 */
void addBrowserUpdateListener(int id, NclBrowserUpdateListener *);

/** Notifies the browser when its focused or not
 *
 * \param browserID The browser identification
 * \param focus true if the browser is focused, false otherwise
 */
void setBrowserFocus(int id, bool focus);

/** Clears the browser window and redraws the content
 *
 * \param browserID The browser identification
 */
void refreshBrowser(int id);

#endif

