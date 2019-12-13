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

#ifndef qmultimediahandler_p_h
#define qmultimediahandler_p_h

#include "MediaComponentNotificationClient.h"
#include "MediaComponentSource.h"

#include "qwebkitglobal.h"

class QMultimediaHandler;


class QMultimediaHandlerPrivate : public WebCore::MediaComponentNotificationClient {

    friend class QMultimediaHandler;
private:
    QMultimediaHandler* m_wrapper;

    // this basic implementation handle only one MediaComponentSource (= only one media player)
    RefPtr<WebCore::MediaComponentSource> m_source;

public:
    QMultimediaHandlerPrivate(QMultimediaHandler*);
    virtual ~QMultimediaHandlerPrivate();

    virtual void notifyVideoComponentsChanged(const WTF::Vector<WebCore::MediaComponent>& components, int currentComponentIndex, WebCore::MediaComponentSource*);
    virtual void notifyAudioComponentsChanged(const WTF::Vector<WebCore::MediaComponent>& components, int currentComponentIndex, WebCore::MediaComponentSource*);
    virtual void notifySubtitleComponentsChanged(const WTF::Vector<WebCore::MediaComponent>& components, int currentComponentIndex, WebCore::MediaComponentSource*);

    virtual void addMediaComponentSource(PassRefPtr<WebCore::MediaComponentSource>);
    virtual void removeMediaComponentSource(PassRefPtr<WebCore::MediaComponentSource>);
};

#endif // qmultimediahandler_p_h
