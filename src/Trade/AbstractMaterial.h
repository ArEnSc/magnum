#ifndef Magnum_Trade_AbstractMaterial_h
#define Magnum_Trade_AbstractMaterial_h
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
 * @brief Class Magnum::Trade::AbstractMaterial
 */

namespace Magnum { namespace Trade {

/**
@brief Base class for materials

Subclasses provide access to parameters for given material type.
*/
class MAGNUM_EXPORT AbstractMaterial {
    AbstractMaterial(const AbstractMaterial& other) = delete;
    AbstractMaterial(AbstractMaterial&& other) = delete;
    AbstractMaterial& operator=(const AbstractMaterial& other) = delete;
    AbstractMaterial& operator=(AbstractMaterial&& other) = delete;

    public:
        /** @brief Material type */
        enum Type {
            Phong       /**< Phong shading */
        };

        /**
         * @brief Constructor
         * @param type      Material type
         */
        inline AbstractMaterial(Type type): _type(type) {}

        /** @brief Destructor */
        virtual ~AbstractMaterial() {}

        /** @brief Material type */
        inline Type type() const { return _type; }

    private:
        Type _type;
};

}}

#endif
