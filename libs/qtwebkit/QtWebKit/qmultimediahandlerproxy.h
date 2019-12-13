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

#ifndef qmultimediahandlerproxy_h
#define qmultimediahandlerproxy_h

#include "MediaComponentSource.h"
#include "qwebkitglobal.h"
#include <QVector>

class QMultimediaComponent;

struct MultimediaComponentContextProxy {
    QVector<QMultimediaComponent>& components;
    int& currentComponentIndex;
    RefPtr<WebCore::MediaComponentSource> source;
};

struct MultimediaComponentProxy {
    const QMultimediaComponent& component;
    bool& result;
    RefPtr<WebCore::MediaComponentSource> source;
};

struct BooleanResult {
    bool& result;
    RefPtr<WebCore::MediaComponentSource> source;
};

struct BooleanInputAndResult {
    bool input;
    bool& result;
};

struct BooleanInputAndResultWithSource {
    bool input;
    bool& result;
    RefPtr<WebCore::MediaComponentSource> source;
};

class QWEBKIT_EXPORT QMultimediaHandlerProxy {

public:
    static void selectAudioComponentProxy(void* context);
    static void selectSubtitleComponent(void* context);

    static void getAudioComponents(void* context);
    static void getSubtitleComponents(void* context);

    static void removeAudio(void* context);
    static void removeSubtitles(void* context);

    static void setSubtitlesEnable(void* context);

    static void addMultimediaClientProxy(void* context);
    static void removeMultimediaClientProxy(void* context);

    static void forceStopAllMediaPlayers(void* context);
    static void mediaPlayersCanAccessScarceSystemResources(void* context);

public:
    QMultimediaHandlerProxy();
    virtual ~QMultimediaHandlerProxy();

};

#endif // qmultimediahandlerproxy_h
