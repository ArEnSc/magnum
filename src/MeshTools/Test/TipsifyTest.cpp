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

#include "TipsifyTest.h"

#include <QtTest/QTest>

#include "MeshBuilder.h"
#include "MeshTools/Tipsify.h"

QTEST_APPLESS_MAIN(Magnum::MeshTools::Test::TipsifyTest)

using namespace std;

namespace Magnum { namespace MeshTools { namespace Test {

/*

 0 ----- 1 ----- 2 ----- 3
  \ 0  /  \ 7  /  \ 2  /  \
   \  / 11 \  / 13 \  / 12 \
    4 ----- 5 ----- 6 ----- 7
   /  \ 3  /  \ 8  /  \ 5  /
  / 14 \  / 9  \  / 15 \  /
 8 ----- 9 ---- 10 ---- 11          18 ---- 17
  \ 4  /  \ 1  /  \ 17 /  \           \ 18  /
   \  / 16 \  / 10 \  / 6  \           \  /
    12 ---- 13 ---- 14 ---- 15          16

*/

TipsifyTest::TipsifyTest(QObject* parent): QObject(parent) {
    unsigned int vertices[19]; /* who cares */
    static const unsigned int indices[] = {
        4, 1, 0,
        10, 9, 13,
        6, 3, 2,
        9, 5, 4,
        12, 9, 8,
        11, 7, 6,

        14, 15, 11,
        2, 1, 5,
        10, 6, 5,
        10, 5, 9,
        13, 14, 10,
        1, 4, 5,

        7, 3, 6,
        6, 2, 5,
        9, 4, 8,
        6, 10, 11,
        13, 9, 12,
        14, 11, 10,

        16, 17, 18
    };
    builder.setData(vertices, indices, 19, 19*3);
}

void TipsifyTest::buildAdjacency() {
    vector<unsigned int> liveTriangleCount, neighborOffset, neighbors;
    Tipsify(builder).buildAdjacency(liveTriangleCount, neighborOffset, neighbors);

    QVERIFY((liveTriangleCount == vector<unsigned int>{
        1, 3, 3, 2,
        4, 6, 6, 2,
        2, 6, 6, 4,
        2, 3, 3, 1,
        1, 1, 1
    }));

    QVERIFY((neighborOffset == vector<unsigned int>{
        0, 1, 4, 7,
        9, 13, 19, 25,
        27, 29, 35, 41,
        45, 47, 50, 53,
        54, 55, 56, 57
    }));

    QVERIFY((neighbors == vector<unsigned int>{
        0,
        0, 7, 11,
        2, 7, 13,
        2, 12,

        0, 3, 11, 14,
        3, 7, 8, 9, 11, 13,
        2, 5, 8, 12, 13, 15,
        5, 12,

        4, 14,
        1, 3, 4, 9, 14, 16,
        1, 8, 9, 10, 15, 17,
        5, 6, 15, 17,

        4, 16,
        1, 10, 16,
        6, 10, 17,
        6,

        18, 18, 18
    }));
}

void TipsifyTest::tipsify() {
    MeshTools::tipsify(builder, 3);

    QVERIFY((builder.indices() == vector<unsigned int>{
        4, 1, 0,
        9, 5, 4,
        1, 4, 5,
        9, 4, 8,
        12, 9, 8,
        13, 9, 12,
        10, 9, 13,
        13, 14, 10,
        10, 6, 5,
        10, 5, 9,
        6, 10, 11,
        14, 11, 10,
        6, 3, 2,
        11, 7, 6,
        7, 3, 6,
        6, 2, 5,
        2, 1, 5,
        14, 15, 11, /* from dead-end vertex stack */
        16, 17, 18 /* arbitrary vertex */
    }));
}

}}}
