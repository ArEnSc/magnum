/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "Source.h"

#include <Containers/Array.h>

#include "Audio/Buffer.h"

namespace Magnum { namespace Audio {

/** @todo C++14: use VLA to avoid unnecessary allocations */

Source& Source::setBuffer(Buffer* buffer) {
    alSourcei(_id, AL_BUFFER, buffer ? buffer->id() : 0);
    return *this;
}

namespace {

Containers::Array<ALuint> sourceIds(const std::initializer_list<Source*>& sources) {
    Containers::Array<ALuint> ids(sources.size());
    for(auto it = sources.begin(); it != sources.end(); ++it) {
        CORRADE_INTERNAL_ASSERT(*it);
        ids[it-sources.begin()] = (*it)->id();
    }
    return ids;
}

Containers::Array<ALuint> sourceIds(const std::vector<Source*>& sources) {
    Containers::Array<ALuint> ids(sources.size());
    for(auto it = sources.begin(); it != sources.end(); ++it) {
        CORRADE_INTERNAL_ASSERT(*it);
        ids[it-sources.begin()] = (*it)->id();
    }
    return ids;
}

}

void Source::play(std::initializer_list<Source*> sources) {
    const auto ids = sourceIds(sources);
    alSourcePlayv(ids.size(), ids);
}

void Source::play(const std::vector<Source*>& sources) {
    const auto ids = sourceIds(sources);
    alSourcePlayv(ids.size(), ids);
}

void Source::pause(std::initializer_list<Source*> sources) {
    const auto ids = sourceIds(sources);
    alSourcePausev(ids.size(), ids);
}

void Source::pause(const std::vector<Source*>& sources) {
    const auto ids = sourceIds(sources);
    alSourcePausev(ids.size(), ids);
}

void Source::stop(std::initializer_list<Source*> sources) {
    const auto ids = sourceIds(sources);
    alSourceStopv(ids.size(), ids);
}

void Source::stop(const std::vector<Source*>& sources) {
    const auto ids = sourceIds(sources);
    alSourceStopv(ids.size(), ids);
}

void Source::rewind(std::initializer_list<Source*> sources) {
    const auto ids = sourceIds(sources);
    alSourceRewindv(ids.size(), ids);
}

void Source::rewind(const std::vector<Source*>& sources) {
    const auto ids = sourceIds(sources);
    alSourceRewindv(ids.size(), ids);
}

Debug operator<<(Debug debug, const Source::State value) {
    switch(value) {
        #define _c(value) case Source::State::value: return debug << "Audio::Source::State::" #value;
        _c(Initial)
        _c(Playing)
        _c(Paused)
        _c(Stopped)
        #undef _c
    }

    return debug << "Audio::Source::State::(invalid)";
}

}}
