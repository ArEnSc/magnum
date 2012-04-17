/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "Query.h"

using namespace Corrade::Utility;

namespace Magnum {

bool AbstractQuery::resultAvailable() {
    GLuint result;
    glGetQueryObjectuiv(query, GL_QUERY_RESULT_AVAILABLE, &result);
    return result == GL_TRUE;
}

template<> bool AbstractQuery::result<bool>() {
    GLuint result;
    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &result);
    return result == GL_TRUE;
}

template<> GLuint AbstractQuery::result<GLuint>() {
    GLuint result;
    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &result);
    return result;
}

template<> GLint AbstractQuery::result<GLint>() {
    GLint result;
    glGetQueryObjectiv(query, GL_QUERY_RESULT, &result);
    return result;
}

template<> GLuint64 AbstractQuery::result<GLuint64>() {
    GLuint64 result;
    glGetQueryObjectui64v(query, GL_QUERY_RESULT, &result);
    return result;
}

template<> GLint64 AbstractQuery::result<GLint64>() {
    GLint64 result;
    glGetQueryObjecti64v(query, GL_QUERY_RESULT, &result);
    return result;
}

void Query::begin(Query::Target target) {
    glBeginQuery(castToUnderlyingType(target), query);
    this->target = new Target(target);
}

void Query::end() {
    if(!target) return;

    glEndQuery(castToUnderlyingType(*target));
    delete target;
    target = nullptr;
}

void SampleQuery::begin(SampleQuery::Target target) {
    glBeginQuery(castToUnderlyingType(target), query);
    this->target = new Target(target);
}

void SampleQuery::end() {
    if(!target) return;

    glEndQuery(castToUnderlyingType(*target));
    delete target;
    target = nullptr;
}

}
