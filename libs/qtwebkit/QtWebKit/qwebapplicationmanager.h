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

#ifndef qwebapplicationmanager_h
#define qwebapplicationmanager_h

#include "qwebkitglobal.h"
#include <QObject>
#include <QtCore/qrect.h>
#include <QtGui/qevent.h>

class QWebApplication;
class QWebPage;
class QWidget;

namespace WebCore {
namespace HbbTV {
class ApplicationManager;
class ApplicationManagerQt;
}
}

class QWEBKIT_EXPORT QWebApplicationManager : public QObject {
    Q_OBJECT
public:

    QWebApplication* createApplication(const QRect&, QWebApplication* = 0, bool = false);
    void destroyApplication(QWebApplication*);

    QWebApplication* findByID(unsigned int) const;
    QWebApplication* topmostApplication() const;
    QWebApplication* topmostVisibleApplication() const;
    QWebApplication* rootApplication() const;

public slots:
    void topmostApplication(QWebApplication*&);

private:
    friend class WebCore::HbbTV::ApplicationManager;
    friend QWebApplicationManager& webAppMgr();

    QWebApplicationManager();
    ~QWebApplicationManager();

    WebCore::HbbTV::ApplicationManagerQt* m_impl;
};

QWEBKIT_EXPORT QWebApplicationManager& webAppMgr();

#endif // qwebapplicationmanager_h
