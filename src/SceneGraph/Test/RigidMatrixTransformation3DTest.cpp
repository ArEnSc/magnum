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

#include <TestSuite/Tester.h>

#include "SceneGraph/RigidMatrixTransformation3D.h"
#include "SceneGraph/Scene.h"

namespace Magnum { namespace SceneGraph { namespace Test {

typedef Object<RigidMatrixTransformation3D<>> Object3D;
typedef Scene<RigidMatrixTransformation3D<>> Scene3D;

class RigidMatrixTransformation3DTest: public Corrade::TestSuite::Tester {
    public:
        explicit RigidMatrixTransformation3DTest();

        void fromMatrix();
        void toMatrix();
        void compose();
        void inverted();

        void setTransformation();
        void translate();
        void rotate();
        void reflect();
        void normalizeRotation();
};

RigidMatrixTransformation3DTest::RigidMatrixTransformation3DTest() {
    addTests({&RigidMatrixTransformation3DTest::fromMatrix,
              &RigidMatrixTransformation3DTest::toMatrix,
              &RigidMatrixTransformation3DTest::compose,
              &RigidMatrixTransformation3DTest::inverted,

              &RigidMatrixTransformation3DTest::setTransformation,
              &RigidMatrixTransformation3DTest::translate,
              &RigidMatrixTransformation3DTest::rotate,
              &RigidMatrixTransformation3DTest::reflect,
              &RigidMatrixTransformation3DTest::normalizeRotation});
}

void RigidMatrixTransformation3DTest::fromMatrix() {
    Matrix4 m = Matrix4::rotationX(Deg(17.0f))*Matrix4::translation({1.0f, -0.3f, 2.3f});
    CORRADE_COMPARE(RigidMatrixTransformation3D<>::fromMatrix(m), m);
}

void RigidMatrixTransformation3DTest::toMatrix() {
    Matrix4 m = Matrix4::rotationX(Deg(17.0f))*Matrix4::translation({1.0f, -0.3f, 2.3f});
    CORRADE_COMPARE(RigidMatrixTransformation3D<>::toMatrix(m), m);
}

void RigidMatrixTransformation3DTest::compose() {
    Matrix4 parent = Matrix4::rotationX(Deg(17.0f));
    Matrix4 child = Matrix4::translation({1.0f, -0.3f, 2.3f});
    CORRADE_COMPARE(RigidMatrixTransformation3D<>::compose(parent, child), parent*child);
}

void RigidMatrixTransformation3DTest::inverted() {
    Matrix4 m = Matrix4::rotationX(Deg(17.0f))*Matrix4::translation({1.0f, -0.3f, 2.3f});
    CORRADE_COMPARE(RigidMatrixTransformation3D<>::inverted(m)*m, Matrix4());
}

void RigidMatrixTransformation3DTest::setTransformation() {
    /* Dirty after setting transformation */
    Object3D o;
    o.setClean();
    o.rotateX(Deg(17.0f));
    CORRADE_VERIFY(o.isDirty());

    /* Scene cannot be transformed */
    Scene3D s;
    s.setClean();
    s.rotateX(Deg(17.0f));
    CORRADE_VERIFY(!s.isDirty());
    CORRADE_COMPARE(s.transformationMatrix(), Matrix4());
}

void RigidMatrixTransformation3DTest::translate() {
    {
        Object3D o;
        o.rotateX(Deg(17.0f));
        o.translate({1.0f, -0.3f, 2.3f});
        CORRADE_COMPARE(o.transformationMatrix(), Matrix4::translation({1.0f, -0.3f, 2.3f})*Matrix4::rotationX(Deg(17.0f)));
    } {
        Object3D o;
        o.rotateX(Deg(17.0f));
        o.translate({1.0f, -0.3f, 2.3f}, TransformationType::Local);
        CORRADE_COMPARE(o.transformationMatrix(), Matrix4::rotationX(Deg(17.0f))*Matrix4::translation({1.0f, -0.3f, 2.3f}));
    }
}

void RigidMatrixTransformation3DTest::rotate() {
    {
        Object3D o;
        o.translate({1.0f, -0.3f, 2.3f});
        o.rotateX(Deg(17.0f))
            ->rotateY(Deg(25.0f))
            ->rotateZ(Deg(-23.0f))
            ->rotate(Deg(96.0f), Vector3(1.0f/Constants::sqrt3()));
        CORRADE_COMPARE(o.transformationMatrix(),
            Matrix4::rotation(Deg(96.0f), Vector3(1.0f/Constants::sqrt3()))*
            Matrix4::rotationZ(Deg(-23.0f))*
            Matrix4::rotationY(Deg(25.0f))*
            Matrix4::rotationX(Deg(17.0f))*
            Matrix4::translation({1.0f, -0.3f, 2.3f}));
    } {
        Object3D o;
        o.translate({1.0f, -0.3f, 2.3f});
        o.rotateX(Deg(17.0f), TransformationType::Local)
            ->rotateY(Deg(25.0f), TransformationType::Local)
            ->rotateZ(Deg(-23.0f), TransformationType::Local)
            ->rotate(Deg(96.0f), Vector3(1.0f/Constants::sqrt3()), TransformationType::Local);
        CORRADE_COMPARE(o.transformationMatrix(),
            Matrix4::translation({1.0f, -0.3f, 2.3f})*
            Matrix4::rotationX(Deg(17.0f))*
            Matrix4::rotationY(Deg(25.0f))*
            Matrix4::rotationZ(Deg(-23.0f))*
            Matrix4::rotation(Deg(96.0f), Vector3(1.0f/Constants::sqrt3())));
    }
}

void RigidMatrixTransformation3DTest::reflect() {
    {
        Object3D o;
        o.rotateX(Deg(17.0f));
        o.reflect(Vector3(-1.0f/Constants::sqrt3()));
        CORRADE_COMPARE(o.transformationMatrix(), Matrix4::reflection(Vector3(-1.0f/Constants::sqrt3()))*Matrix4::rotationX(Deg(17.0f)));
    } {
        Object3D o;
        o.rotateX(Deg(17.0f));
        o.reflect(Vector3(-1.0f/Constants::sqrt3()), TransformationType::Local);
        CORRADE_COMPARE(o.transformationMatrix(), Matrix4::rotationX(Deg(17.0f))*Matrix4::reflection(Vector3(-1.0f/Constants::sqrt3())));
    }
}

void RigidMatrixTransformation3DTest::normalizeRotation() {
    Object3D o;
    o.rotateX(Deg(17.0f));
    o.normalizeRotation();
    CORRADE_COMPARE(o.transformationMatrix(), Matrix4::rotationX(Deg(17.0f)));
}

}}}

CORRADE_TEST_MAIN(Magnum::SceneGraph::Test::RigidMatrixTransformation3DTest)