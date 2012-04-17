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

#include "SceneTest.h"

#include <QtTest/QTest>

#include "Scene.h"

QTEST_APPLESS_MAIN(Magnum::Test::SceneTest)

namespace Magnum { namespace Test {

void SceneTest::transformation() {
    Scene scene;

    Object* scenePointer = &scene;
    scenePointer->setTransformation(Matrix4::translation({1.0f, 1.0f, 1.0f}));
    QVERIFY(scene.transformation() == Matrix4());
}

void SceneTest::parent() {
    Scene scene;

    QVERIFY(scene.parent() == &scene);

    /* Scene parent cannot be changed */
    Object* scenePointer = &scene;
    Object object;
    scenePointer->setParent(&object);
    QVERIFY(scene.parent() == &scene);
    QVERIFY(scene.children().empty());
    QVERIFY(object.children().empty());
}

}}
