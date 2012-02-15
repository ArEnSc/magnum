#ifndef Magnum_MeshTools_Test_SubdivideTest_h
#define Magnum_MeshTools_Test_SubdivideTest_h
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

#include <QtCore/QObject>

namespace Magnum { namespace MeshTools { namespace Test {

class SubdivideTest: public QObject {
    Q_OBJECT

    private slots:
        void subdivide();

    private:
        class Vector1 {
            public:
                static const size_t Size = 1;
                typedef int Type;

                Vector1(): data(0) {}
                Vector1(int i): data(i) {}
                int operator[](size_t i) const { return data; }
                int& operator[](size_t i) { return data; }
                bool operator==(Vector1 i) const { return i.data == data; }
                Vector1 operator-(Vector1 i) const { return data-i.data; }

            private:
                int data;
        };

        inline static Vector1 interpolator(Vector1 a, Vector1 b) { return (a[0]+b[0])/2; }
};

}}}

#endif
