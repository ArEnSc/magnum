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

#include "Camera.h"
#include "Scene.h"

using namespace std;

namespace Magnum {

Camera::Camera(Object* parent): Object(parent), _aspectRatioPolicy(Extend) {}

void Camera::setOrthographic(GLfloat size, GLfloat near, GLfloat far) {
    _near = near;
    _far = far;

    /* Scale the volume down so it fits in (-1, 1) in all directions */
    GLfloat xyScale = 2/size;
    GLfloat zScale = 2/(far-near);
    rawProjectionMatrix = Matrix4::scaling({xyScale, xyScale, -zScale});

    /* Move the volume on z into (-1, 1) range */
    rawProjectionMatrix = Matrix4::translation(Vector3::zAxis(-1-near*zScale))*rawProjectionMatrix;

    fixAspectRatio();
}

void Camera::setPerspective(GLfloat fov, GLfloat near, GLfloat far) {
    _near = near;
    _far = far;

    /* First move the volume on z in (-1, 1) range */
    rawProjectionMatrix = Matrix4::translation(Vector3::zAxis(2*far*near/(far+near)));

    /* Then apply magic perspective matrix (with reversed Z) */
    static Matrix4 a(1.0f, 0.0f,  0.0f,  0.0f,
                     0.0f, 1.0f,  0.0f,  0.0f,
                     0.0f, 0.0f,  -1.0f,  -1.0f,
                     0.0f, 0.0f,  0.0f,  0.0f);
    rawProjectionMatrix = a*rawProjectionMatrix;

    /* Then scale the volume down so it fits in (-1, 1) in all directions */
    GLfloat xyScale = 1/tan(fov/2);
    GLfloat zScale = 1+2*near/(far-near);
    rawProjectionMatrix = Matrix4::scaling({xyScale, xyScale, zScale})*rawProjectionMatrix;

    /* And... another magic */
    rawProjectionMatrix.set(3, 3, 0);

    fixAspectRatio();
}

void Camera::setViewport(const Math::Vector2<GLsizei>& size) {
    glViewport(0, 0, size.x(), size.y());

    _viewport = size;
    fixAspectRatio();
}

void Camera::setClean() {
    if(!isDirty()) return;
    _cameraMatrix = absoluteTransformation().inversed();
    Object::setClean();
}

void Camera::fixAspectRatio() {
    /* Don't divide by zero */
    if(_viewport.x() == 0 || _viewport.y() == 0) {
        _projectionMatrix = rawProjectionMatrix;
        return;
    }

    /* Extend on larger side = scale larger side down */
    if(_aspectRatioPolicy == Extend) {
        _projectionMatrix = ((_viewport.x() > _viewport.y()) ?
            Matrix4::scaling({static_cast<GLfloat>(_viewport.y())/_viewport.x(), 1, 1}) :
            Matrix4::scaling({1, static_cast<GLfloat>(_viewport.x())/_viewport.y(), 1})
        )*rawProjectionMatrix;

    /* Clip on smaller side = scale smaller side up */
    } else if(_aspectRatioPolicy == Clip) {
        _projectionMatrix = ((_viewport.x() > _viewport.y()) ?
            Matrix4::scaling({1, static_cast<GLfloat>(_viewport.x())/_viewport.y(), 1}) :
            Matrix4::scaling({static_cast<GLfloat>(_viewport.y())/_viewport.x(), 1, 1})
        )*rawProjectionMatrix;

    /* Don't preserve anything */
    } else _projectionMatrix = rawProjectionMatrix;
}

void Camera::setClearColor(const Magnum::Vector4& color) {
    glClearColor(color.r(), color.g(), color.b(), color.a());
    _clearColor = color;
}

void Camera::draw() {
    /** @todo Clear only set features */
    glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    /* Recursively draw child objects */
    drawChildren(scene(), cameraMatrix());
}

void Camera::drawChildren(Object* object, const Matrix4& transformationMatrix) {
    for(set<Object*>::const_iterator it = object->children().begin(); it != object->children().end(); ++it) {
        /* Transformation matrix for the object */
        Matrix4 matrix = transformationMatrix*(*it)->transformation();

        /* Draw the object and its children */
        (*it)->draw(matrix, this);
        drawChildren(*it, matrix);
    }
}

}
