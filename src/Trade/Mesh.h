#ifndef Magnum_Trade_Mesh_h
#define Magnum_Trade_Mesh_h
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

/** @file
 * @brief Class Magnum::Trade::Mesh
 */

#include "../Mesh.h"

namespace Magnum { namespace Trade {

/**
@brief %Mesh

Provides access to mesh data and additional information, such as primitive
type.
*/
class MAGNUM_EXPORT Mesh {
    Mesh(const Mesh& other) = delete;
    Mesh(Mesh&& other) = delete;
    Mesh& operator=(const Mesh& other) = delete;
    Mesh& operator=(Mesh&& other) = delete;

    public:
        /**
         * @brief Constructor
         * @param   primitive   Primitive
         */
        inline Mesh(Magnum::Mesh::Primitive primitive, std::vector<unsigned int>* indices, std::vector<std::vector<Vector3>*> vertices, std::vector<std::vector<Vector3>*> normals, std::vector<std::vector<Vector2>*> textureCoords2D): _primitive(primitive), _indices(indices), _vertices(vertices), _normals(normals), _textureCoords2D(textureCoords2D) {}

        /** @brief Destructor */
        ~Mesh();

        /** @brief Primitive */
        inline Magnum::Mesh::Primitive primitive() const { return _primitive; }

        /**
         * @brief Indices
         * @return Indices or nullptr if the mesh is not indexed.
         */
        inline std::vector<unsigned int>* indices() { return _indices; }

        /** @brief Count of vertex arrays */
        inline size_t vertexArrayCount() { return _vertices.size(); };

        /**
         * @brief Vertices
         * @param id    ID of vertex data array
         * @return Vertices or nullptr if there is no vertex array with given
         *      ID.
         */
        inline std::vector<Vector3>* vertices(size_t id) { return _vertices[id]; }

        /** @brief Count of normal arrays */
        inline size_t normalArrayCount() { return _normals.size(); };

        /**
         * @brief Normals
         * @param id    ID of normal data array
         * @return Vertices or nullptr if there is no normal array with given
         *      ID.
         */
        inline std::vector<Vector3>* normals(size_t id) { return _normals[id]; }

        /** @brief Count of 2D texture coordinate arrays */
        inline size_t textureCoords2DArrayCount() { return _textureCoords2D.size(); };

        /**
         * @brief 2D texture coordinates
         * @param id    ID of texture coordinates array
         * @return Texture coordinates or nullptr if there is no texture
         *      coordinates array with given ID.
         */
        inline std::vector<Vector2>* textureCoords2D(size_t id) { return _textureCoords2D[id]; }

    private:
        Magnum::Mesh::Primitive _primitive;
        std::vector<unsigned int>* _indices;
        std::vector<std::vector<Vector3>*> _vertices,
            _normals;
        std::vector<std::vector<Vector2>*> _textureCoords2D;
};

}}

#endif
