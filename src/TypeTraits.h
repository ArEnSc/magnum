#ifndef Magnum_TypeTraits_h
#define Magnum_TypeTraits_h
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
 * @brief Enum Magnum::Type, class Magnum::TypeOf, Magnum::TypeInfo, Magnum::TypeTraits
 */

#include "AbstractImage.h"

namespace Magnum {

/**
@brief Traits class for plain OpenGL types

@copydetails Math::TypeTraits

Where it makes sense, this class extends Math::TypeTraits with OpenGL-specific
traits.
*/
#ifdef DOXYGEN_GENERATING_OUTPUT
template<class T> struct TypeTraits: public Math::TypeTraits<T> {
    /**
     * @brief OpenGL plain type ID
     *
     * Returns e.g. Type::UnsignedInt for GLuint.
     */
    constexpr inline static Type type();

    /**
     * @brief OpenGL type ID for indices
     *
     * Implemented only in types which can be used for vertex indices (all
     * unsigned types). This function is not present for types unusable for
     * vertex indices, like GLfloat or GLint.
     */
    constexpr inline static Type indexType();

    /**
     * @brief OpenGL type ID for images
     *
     * Implemented only in types which can be used for image data, like
     * GLubyte. This function is not present for types unusable for image data,
     * like GLdouble and Matrix3.
     */
    constexpr inline static AbstractImage::ComponentType imageType();

    /**
     * @brief Size of plain OpenGL type
     *
     * Returns sizeof(GLfloat) for GLfloat, but also sizeof(GLfloat) for
     * Vector3. See count().
     */
    constexpr inline static size_t size();

    /**
     * @brief Count of plain elements in this type
     *
     * Returns 1 for plain OpenGL types like GLint, but e.g. 3 for Vector3.
     */
    constexpr inline static size_t count();
};
#else
template<class T> struct TypeTraits {};
#endif

/** @brief OpenGL plain types */
enum class Type: GLenum {
    UnsignedByte = GL_UNSIGNED_BYTE,    /**< Unsigned byte (char) */
    Byte = GL_BYTE,                     /**< Byte (char) */
    UnsignedShort = GL_UNSIGNED_SHORT,  /**< Unsigned short */
    Short = GL_SHORT,                   /**< Short */
    UnsignedInt = GL_UNSIGNED_INT,      /**< Unsigned int */
    Int = GL_INT,                       /**< Int */
    Float = GL_FLOAT,                   /**< Float */
    Double = GL_DOUBLE                  /**< Double */
};

/**
@brief Class for converting Type enum values to types

When you want to use TypeTraits on type specified only as enum value, you can
use this class to convert it into type, for example these two statements
are equivalent:
@code
type = TypeTraits<TypeOf<Type::UnsignedByte>::Type>::imageType();
type = TypeTraits<GLubyte>::imageType();
@endcode
*/
template<Type T> class TypeOf {
    #ifdef DOXYGEN_GENERATING_OUTPUT
    typedef U Type; /**< @brief Type */
    #endif
};

/**
@brief Functor for runtime information about given type

TypeTraits alone allows to get information about given type only at compile
time, this class alows to get some information also at runtime with tiny
performance loss.
*/
struct MAGNUM_EXPORT TypeInfo {
    /**
     * @brief Size of given type
     *
     * These two lines provide the same information, one at compile time,
     * one at runtime:
     * @code
     * size_t size = TypeTraits<TypeOf<Type::UnsignedByte>::size();
     * size_t size = TypeInfo::sizeOf(Type::UnsignedByte);
     * @endcode
     */
    static size_t sizeOf(Type type);

    /**
     * @brief Whether the type is integral
     * @return true for (un)signed byte, short and integer, false otherwise.
     */
    static bool isIntegral(Type type);
};

/** @todo Other texture types, referenced in glTexImage2D function manual */
/** @todo Using Vector3 for textures? */

#ifndef DOXYGEN_GENERATING_OUTPUT
template<> struct TypeOf<Type::UnsignedByte> { typedef GLubyte Type; };
template<> struct TypeOf<Type::Byte> { typedef GLbyte Type; };
template<> struct TypeOf<Type::UnsignedShort> { typedef GLushort Type; };
template<> struct TypeOf<Type::Short> { typedef GLshort Type; };
template<> struct TypeOf<Type::UnsignedInt> { typedef GLuint Type; };
template<> struct TypeOf<Type::Int> { typedef GLint Type; };
template<> struct TypeOf<Type::Float> { typedef GLfloat Type; };
template<> struct TypeOf<Type::Double> { typedef GLdouble Type; };

template<> struct TypeTraits<GLubyte>: public Math::TypeTraits<unsigned char> {
    inline constexpr static Type type() { return Type::UnsignedByte; }
    inline constexpr static Type indexType() { return Type::UnsignedByte; }
    inline constexpr static AbstractImage::ComponentType imageType() { return AbstractImage::ComponentType::UnsignedByte; }
    inline constexpr static size_t size() { return sizeof(GLubyte); }
    inline constexpr static size_t count() { return 1; }
};

template<> struct TypeTraits<GLbyte>: public Math::TypeTraits<char> {
    inline constexpr static Type type() { return Type::Byte; }
    /* Can not be used for indices */
    inline constexpr static AbstractImage::ComponentType imageType() { return AbstractImage::ComponentType::Byte; }
    inline constexpr static size_t size() { return sizeof(GLbyte); }
    inline constexpr static size_t count() { return 1; }
};

template<> struct TypeTraits<GLushort>: public Math::TypeTraits<unsigned short> {
    inline constexpr static Type type() { return Type::UnsignedShort; }
    inline constexpr static Type indexType() { return Type::UnsignedShort; }
    inline constexpr static AbstractImage::ComponentType imageType() { return AbstractImage::ComponentType::UnsignedShort; }
    inline constexpr static size_t size() { return sizeof(GLushort); }
    inline constexpr static size_t count() { return 1; }
};

template<> struct TypeTraits<GLshort>: public Math::TypeTraits<short> {
    inline constexpr static Type type() { return Type::Short; }
    /* Can not be used for indices */
    inline constexpr static AbstractImage::ComponentType imageType() { return AbstractImage::ComponentType::Short; }
    inline constexpr static size_t size() { return sizeof(GLshort); }
    inline constexpr static size_t count() { return 1; }
};

template<> struct TypeTraits<GLuint>: public Math::TypeTraits<unsigned int> {
    inline constexpr static Type type() { return Type::UnsignedInt; }
    inline constexpr static Type indexType() { return Type::UnsignedInt; }
    inline constexpr static AbstractImage::ComponentType imageType() { return AbstractImage::ComponentType::UnsignedInt; }
    inline constexpr static size_t size() { return sizeof(GLuint); }
    inline constexpr static size_t count() { return 1; }
};

template<> struct TypeTraits<GLint>: public Math::TypeTraits<int> {
    inline constexpr static Type type() { return Type::Int; }
    /* Can not be used for indices */
    inline constexpr static AbstractImage::ComponentType imageType() { return AbstractImage::ComponentType::Int; }
    inline constexpr static size_t size() { return sizeof(GLint); }
    inline constexpr static size_t count() { return 1; }
};

template<> struct TypeTraits<GLfloat>: public Math::TypeTraits<float> {
    inline constexpr static Type type() { return Type::Float; }
    /* Can not be used for indices */
    inline constexpr static AbstractImage::ComponentType imageType() { return AbstractImage::ComponentType::Float; }
    inline constexpr static size_t size() { return sizeof(GLfloat); }
    inline constexpr static size_t count() { return 1; }
};

template<> struct TypeTraits<GLdouble>: public Math::TypeTraits<double> {
    inline constexpr static Type type() { return Type::Double; }
    /* Can not be used for indices */
    /* Can not be used for images */
    inline constexpr static size_t size() { return sizeof(GLdouble); }
    inline constexpr static size_t count() { return 1; }
};

template<class T, size_t vectorSize> struct TypeTraits<Math::Vector<T, vectorSize>> {
    inline constexpr static Type type() { return TypeTraits<T>::type(); }
    /* Can not be used for indices */
    /* Can not be used for images */
    inline constexpr static size_t size() { return sizeof(T); }
    inline constexpr static size_t count() { return vectorSize; }
};

template<class T> struct TypeTraits<Math::Vector2<T>>: public TypeTraits<Math::Vector<T, 2>> {};
template<class T> struct TypeTraits<Math::Vector3<T>>: public TypeTraits<Math::Vector<T, 3>> {};
template<class T> struct TypeTraits<Math::Vector4<T>>: public TypeTraits<Math::Vector<T, 4>> {};

template<class T, size_t matrixSize> struct TypeTraits<Math::Matrix<T, matrixSize>> {
    inline constexpr static Type type() { return TypeTraits<T>::type(); }
    /* Can not be used for indices */
    /* Can not be used for images */
    inline constexpr static size_t size() { return sizeof(T); }
    inline constexpr static size_t count() { return matrixSize*matrixSize; }
};

template<class T> struct TypeTraits<Math::Matrix3<T>>: public TypeTraits<Math::Matrix<T, 3>> {};
template<class T> struct TypeTraits<Math::Matrix4<T>>: public TypeTraits<Math::Matrix<T, 4>> {};
#endif

}

#endif
