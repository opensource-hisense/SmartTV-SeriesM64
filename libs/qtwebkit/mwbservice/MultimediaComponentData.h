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

#ifndef MultimediaComponentData_h
#define MultimediaComponentData_h

#include <string>

class MultimediaComponentData {

public:
    enum Component {
        COMPONENT_TYPE_AUDIO = 1,
        COMPONENT_TYPE_SUBTITLE = 2
    };

public:
    MultimediaComponentData()
        : m_type(COMPONENT_TYPE_AUDIO)
        , m_encoding("")
        , m_language("")
        , m_audioChannels(0)
        , m_hearingImpaired(false)
        , m_index(-1)
        , m_contextID(0)
        , m_isAudioDescription(false)
    {
    }

    MultimediaComponentData(const std::string& encoding, const std::string& language, int audioChannels, int index, unsigned int contextID, bool isAudioDescription)
        : m_type(COMPONENT_TYPE_AUDIO)
        , m_encoding(encoding)
        , m_language(language)
        , m_audioChannels(audioChannels)
        , m_index(index)
        , m_contextID(contextID)
        , m_isAudioDescription(isAudioDescription)
    {
    }

    MultimediaComponentData(const std::string& encoding, const std::string& language, bool hearingImpaired, int index, unsigned int contextID)
        : m_type(COMPONENT_TYPE_SUBTITLE)
        , m_encoding(encoding)
        , m_language(language)
        , m_hearingImpaired(hearingImpaired)
        , m_index(index)
        , m_contextID(contextID)
        , m_isAudioDescription(false)
    {
    }

    virtual ~MultimediaComponentData()
    {
    }

    Component type() const { return m_type; }
    void setType(Component type) { m_type = type; }

    const std::string& encoding() const { return m_encoding; }
    void setEncoding(const std::string& encoding) { m_encoding = encoding; }

    const std::string& language() const { return m_language; }
    void setLanguage(const std::string& language) { m_language = language; }

    int audioChannels() const { return m_audioChannels; } // audio only
    void setAudioChannels(int audioChannels) { m_audioChannels = audioChannels; }

    bool hearingImpaired() const { return m_hearingImpaired; } // subtitle only
    void setHearingImpaired(bool hearingImpaired) { m_hearingImpaired = hearingImpaired; }

    int index() const { return m_index; }
    void setIndex(int idx) { m_index = idx; }

    unsigned int contextID() const { return m_contextID; }
    void setContextID(unsigned int contextID) { m_contextID = contextID; }

    bool isAudioDescription() const { return m_isAudioDescription; } // audio only
    void setIsAudioDescription(bool isAudioDescription) { m_isAudioDescription = isAudioDescription; }

private:
    Component m_type;
    std::string m_encoding;
    std::string m_language;
    int m_audioChannels;
    bool m_hearingImpaired;
    int m_index;
    unsigned int m_contextID;
    bool m_isAudioDescription;
};

#endif // MultimediaComponentData_h
