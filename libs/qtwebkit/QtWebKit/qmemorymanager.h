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

#ifndef qmemorymanager_h
#define qmemorymanager_h

#include "qwebkitglobal.h"

#include <QObject>

class QMemoryStatistics;
class QPageMemoryStatistics;
class QWebPage;

class QWEBKIT_EXPORT QMemoryManager : public QObject {
    Q_OBJECT

public:
    QMemoryManager(QObject* parent = 0);

    void garbageCollectorCollect();
    QMemoryStatistics getMemoryStatistics(bool print = false);
    QPageMemoryStatistics getPageMemoryStatistics(QWebPage*);

    /*!
        Start MemoryMonitor to record memory usage.

        MemoryMonitor will try to load symbol from the input path.
        Symbol file is created by execute "readelf -s -W" to the
        library or binary file which contains function symbol info.
    */
    static bool startMemoryMonitor(const QString& pathToSymbolFile);

    /*!
        Dump the MemoryMonitor result in standard output
    */
    static void dumpMemoryMonitorResult();

public slots:
    void memoryStatistics(QMemoryStatistics&);
};

class QWEBKIT_EXPORT QMemoryStatistics {
public:
    unsigned long long total;
    unsigned long long JSC;
    unsigned long CachedCSS;
    unsigned long CachedFonts;
    unsigned long CachedImages;
    unsigned long CachedScripts;
    unsigned long CachedSVG;
    unsigned long CachedXSLT;
    unsigned long long processPrivate;
    unsigned long long processShared;
};

class QWEBKIT_EXPORT QPageMemoryStatistics {
public:
    unsigned long long total;
    unsigned long DOM;
    unsigned long CSS;
    unsigned long loader;
    unsigned long rendertree;
    unsigned long other;
    unsigned long fonts;
    unsigned long images;
    unsigned long scripts;
    unsigned long SVG;
    unsigned long XSLT;
};

#endif /* qmemorymanager_h */
