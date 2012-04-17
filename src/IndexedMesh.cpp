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

#include "IndexedMesh.h"

#include <cassert>

using namespace std;
using namespace Corrade::Utility;

namespace Magnum {

void IndexedMesh::draw() {
    bind();

    /* Finalize the mesh, if it is not already */
    finalize();

    /** @todo Start at given index */
    glDrawElements(castToUnderlyingType(primitive()), _indexCount, castToUnderlyingType(_indexType), nullptr);

    unbind();
}

void IndexedMesh::finalize() {
    if(isFinalized()) return;

    if(!_indexCount) {
        Error() << "IndexedMesh: the mesh has zero index count!";
        assert(0);
    }

    /* Finalize attribute positions */
    Mesh::finalize();

    /* Bind index buffer */
    _indexBuffer.bind();
}

}
