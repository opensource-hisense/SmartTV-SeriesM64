/*
    Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#ifndef QWEBSETTINGS_H
#define QWEBSETTINGS_H

#include "qwebkitglobal.h"

#include <QtCore/qstring.h>
#include <QtGui/qpixmap.h>
#include <QtGui/qicon.h>
#include <QtCore/qshareddata.h>

namespace WebCore {
    class Settings;
}

class QWebPage;
class QWebPluginDatabase;
class QWebSettingsPrivate;
QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

class QWEBKIT_EXPORT QWebSettings {
public:
    enum FontFamily {
        StandardFont,
        FixedFont,
        SerifFont,
        SansSerifFont,
        CursiveFont,
        FantasyFont
    };
    enum WebAttribute {
        AutoLoadImages,
        JavascriptEnabled,
        JavaEnabled,
        PluginsEnabled,
        PrivateBrowsingEnabled,
        JavascriptCanOpenWindows,
        JavascriptCanAccessClipboard,
        DeveloperExtrasEnabled,
        LinksIncludedInFocusChain,
        ZoomTextOnly,
        PrintElementBackgrounds,
        OfflineStorageDatabaseEnabled,
        OfflineWebApplicationCacheEnabled,
        LocalStorageEnabled,
#if defined(QT_DEPRECATED) || defined(qdoc)
        LocalStorageDatabaseEnabled = LocalStorageEnabled,
#endif
        LocalContentCanAccessRemoteUrls,
        DnsPrefetchEnabled,
        XSSAuditingEnabled,
        AcceleratedCompositingEnabled,
        DebugBordersEnabled,
        SpatialNavigationEnabled,
        LocalContentCanAccessFileUrls,
        TiledBackingStoreEnabled,
        FrameFlatteningEnabled,
        SiteSpecificQuirksEnabled,
        JavascriptCanCloseWindows,
        DisableJavascriptCloseWindows,
        WebGLEnabled,
        CSSRegionsEnabled,
        HyperlinkAuditingEnabled,
        CSSGridLayoutEnabled,
        HbbTVEnabled,
        RequiresFullScreenForVideoPlayback,
        JavaScriptAutoStopLoop,
        DefaultPageActionEnabled,
        CookieEnabled,
        WebSecurityEnabled,
        NpapiDisplayNoneHackEnabled,
        LocalStorageForceDeleteEnabled,
        FullScreenAPIEnabled,
        XHRTimeoutEnabled
    };
    enum WebGraphic {
        MissingImageGraphic,
        MissingPluginGraphic,
        DefaultFrameIconGraphic,
        TextAreaSizeGripCornerGraphic,
        DeleteButtonGraphic,
        InputSpeechButtonGraphic,
        SearchCancelButtonGraphic,
        SearchCancelButtonPressedGraphic
    };
    enum FontSize {
        MinimumFontSize,
        MinimumLogicalFontSize,
        DefaultFontSize,
        DefaultFixedFontSize
    };
    enum ThirdPartyCookiePolicy {
        AlwaysAllowThirdPartyCookies,
        AlwaysBlockThirdPartyCookies,
        AllowThirdPartyWithExistingCookies
    };

    static QWebSettings *globalSettings();

    void setFontFamily(FontFamily which, const QString &family);
    QString fontFamily(FontFamily which) const;
    void resetFontFamily(FontFamily which);

    void setFontSize(FontSize type, int size);
    int fontSize(FontSize type) const;
    void resetFontSize(FontSize type);

    void setAttribute(WebAttribute attr, bool on);
    bool testAttribute(WebAttribute attr) const;
    void resetAttribute(WebAttribute attr);

    void setUserStyleSheetUrl(const QUrl &location);
    QUrl userStyleSheetUrl() const;

    void setDefaultTextEncoding(const QString &encoding);
    QString defaultTextEncoding() const;

    void setMaximumDecodedImageSize(unsigned);
    unsigned maximumDecodedImageSize() const;

    void setShowDisplayOfInsecureContentLog(bool);
    bool showDisplayOfInsecureContentLog() const;

    static void setIconDatabasePath(const QString &location);
    static QString iconDatabasePath();
    static void clearIconDatabase();
    static QIcon iconForUrl(const QUrl &url);

    //static QWebPluginDatabase *pluginDatabase();

    static void setWebGraphic(WebGraphic type, const QPixmap &graphic);
    static QPixmap webGraphic(WebGraphic type);

    static void setMaximumPagesInCache(int pages);
    static int maximumPagesInCache();
    static void setObjectCacheCapacities(int cacheMinDeadCapacity, int cacheMaxDead, int totalCapacity);

    static void setOfflineStoragePath(const QString& path);
    static QString offlineStoragePath();
    static void setOfflineStorageDefaultQuota(qint64 maximumSize);
    static qint64 offlineStorageDefaultQuota();
    static void setOfflineStorageFreeSpacePercentage(qint64);
    static qint64 offlineStorageFreeSpacePercentage();

    static void setOfflineWebApplicationCachePath(const QString& path);
    static QString offlineWebApplicationCachePath();
    static void setOfflineWebApplicationCacheQuota(qint64 maximumSize);
    static qint64 offlineWebApplicationCacheQuota();

    void setLocalStoragePath(const QString& path);
    QString localStoragePath() const;

    static void clearMemoryCaches();
    static void setLowMemoryThreshold(quint64);
    static void setWebcoreLowMemoryThreshold(quint64);
    static void setJsEngineLowMemoryThreshold(quint64);
    static void setMemorySecurityThreshold(size_t);
    static quint64 lowMemoryThreshold();
    static size_t memorySecurityThreshold();

    static void enablePersistentStorage(const QString& path = QString());

    void setThirdPartyCookiePolicy(ThirdPartyCookiePolicy);
    QWebSettings::ThirdPartyCookiePolicy thirdPartyCookiePolicy() const;
    static void setActiveSelectionBackgroundColor(const QColor&);
    static QColor activeSelectionBackgroundColor();
    static void setInactiveSelectionBackgroundColor(const QColor&);
    static QColor inactiveSelectionBackgroundColor();
    static void setActiveSelectionForegroundColor(const QColor&);
    static QColor activeSelectionForegroundColor();
    static void setInactiveSelectionForegroundColor(const QColor&);
    static QColor inactiveSelectionForegroundColor();

    static void setActiveTextSearchHighlightColor(const QColor&);
    static QColor activeTextSearchHighlightColor();
    static void setInactiveTextSearchHighlightColor(const QColor&);
    static QColor inactiveTextSearchHighlightColor();

    static void setFocusRingColor(const QColor&);
    static QColor focusRingColor();

    static void setJavaScriptLoopTimeout(unsigned);
    static unsigned javaScriptLoopTimeout();
#if defined(ENABLE_HBBTV) && ENABLE_HBBTV
    static void setDownloadPath(const QString&);
    static QString downloadPath();
    static QString downloadDatabasePath();
#endif

#if defined(ENABLE_WIDGET_ENGINE) && ENABLE_WIDGET_ENGINE
    static void setWidgetEngineInstallationPath(const QString&);
    static QString widgetEngineInstallationPath();

    static void setWidgetEngineInstallationQuota(quint64);
    static quint64 widgetEngineInstallationQuota();

    static void setWidgetEngineStoragePath(const QString&);
    static QString widgetEngineStoragePath();
#endif

    static void setSSLCACertificatesPath(const QString&);
    static QString sslCACertificatesPath();
    static void setSSLClientCertificate(const QString&);
    static QString sslClientCertificate();
    static void setSSLClientPrivateKey(const QString&);
    static QString sslClientPrivateKey();

    void setMaximumContentLength(unsigned);
    unsigned maximumContentLength() const;

    void setMediaPlayerKeyUnitSeekEnabled(bool);
    bool mediaPlayerKeyUnitSeekEnabled() const;

    void setRemoteInspectorPort(int);
    int remoteInspectorPort() const;

    static void addDomainToVirtualKeyboardBlacklist(const QUrl&);
    static void removeDomainFromVirtualKeyboardBlacklist(const QUrl&);
    static void clearVirtualKeyboardBlacklist();

#if defined(ENABLE_HBBTV) && ENABLE_HBBTV
    void setAITRescheduleDelay(unsigned);
    unsigned aitRescheduleDelay() const;

    void setAITRescheduleAfterBroadcastServiceResumed(bool);
    bool aitRescheduleAfterBroadcastServiceResumed() const;

    enum AITTransportProtocolPreference { AITOrderPreferred, HTTPPreferred, DVBPreferred };
    void setAITTransportProtocolPreference(AITTransportProtocolPreference);
    AITTransportProtocolPreference aitTransportProtocolPreference() const;

    static void addDomainToHbbTVSecurityWhitelist(const QUrl&);
    static void removeDomainFromHbbTVSecurityWhitelist(const QUrl&);
    static void clearHbbTVSecurityWhitelist();

    static void addURLToHbbTVTraditionalViewList(const QUrl&);
    static void removeURLFromHbbTVTraditionalViewList(const QUrl&);
    static void clearHbbTVTraditionalViewList();
    static std::vector<std::string> getHbbTVTraditionalViewList();

    static void setCurrentLoadURL(const QUrl&);
    static std::string getCurrentLoadURL();

    enum VirtualKeyboardBehavior { VirtualKeyboardDisabled, VirtualKeyboardPopupOnFocus, VirtualKeyboardPopupOnEnter };
    VirtualKeyboardBehavior virtualKeyboardBehavior();
    void setVirtualKeyboardBehavior(VirtualKeyboardBehavior);

    void setEnforceSingleApplicationModel(bool);
    bool enforceSingleApplicationModel() const;

    void setShouldRescheduleAITInErrorHTTPStatus(bool);
    bool shouldRescheduleAITInErrorHTTPStatus() const;

    void setIgnoreAuthenticationRequired(bool);
    bool ignoreAuthenticationRequired() const;

    void setLoadBlankPageBeforeApplicationDestroy(bool);
    bool loadBlankPageBeforeApplicationDestroy() const;
#endif

    static void addURLToRedirect(const QString&, const QString&);

    inline QWebSettingsPrivate* handle() const { return d; }

private:
    friend class QWebPagePrivate;
    friend class QWebSettingsPrivate;

    Q_DISABLE_COPY(QWebSettings)

    QWebSettings();
    QWebSettings(WebCore::Settings *settings);
    ~QWebSettings();

    QWebSettingsPrivate *d;
};

#endif
