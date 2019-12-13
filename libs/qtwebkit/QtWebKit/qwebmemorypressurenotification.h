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

#ifndef qwebmemorypressurenotification_h
#define qwebmemorypressurenotification_h

#include "qwebkitglobal.h"

#include <QObject>
#include <QScopedPointer>

class QWEBKIT_EXPORT QWebMemoryPressureNotificationClient {
public:
    QWebMemoryPressureNotificationClient() { }
    virtual void notifyLowMemory() = 0;
};

class QWebMemoryPressureNotificationPrivate;
class QWEBKIT_EXPORT QWebMemoryPressureNotification {
public:
    explicit QWebMemoryPressureNotification(QWebMemoryPressureNotificationClient*);
    ~QWebMemoryPressureNotification();
private:
    Q_DISABLE_COPY(QWebMemoryPressureNotification)

    QScopedPointer<QWebMemoryPressureNotificationPrivate> m_private;
};

#endif
