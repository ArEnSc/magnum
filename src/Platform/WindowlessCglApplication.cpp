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

#include "WindowlessCglApplication.h"

#include <Utility/Assert.h>
#include <Utility/Debug.h>

#include "Context.h"

#define None 0L // redef Xlib nonsense

namespace Magnum { namespace Platform {

/** @todo Delegating constructor when support for GCC 4.6 is dropped */

WindowlessCglApplication::WindowlessCglApplication(const Arguments&, const Configuration& configuration): c(nullptr) {
    createContext(configuration);
}

#ifndef DOXYGEN_GENERATING_OUTPUT
WindowlessCglApplication::WindowlessCglApplication(const Arguments&): c(nullptr) {
    createContext();
}
#endif

WindowlessCglApplication::WindowlessCglApplication(const Arguments&, std::nullptr_t): c(nullptr) {}

void WindowlessCglApplication::createContext() { createContext({}); }

void WindowlessCglApplication::createContext(const Configuration& configuration) {
    if(!tryCreateContext(configuration)) std::exit(1);
}

bool WindowlessCglApplication::tryCreateContext(const Configuration&) {
    CORRADE_ASSERT(!c, "Platform::WindowlessCglApplication::tryCreateContext(): context already created", false);


    display = XOpenDisplay(nullptr);

    /* Check version */
    GLuint major, minor;
    CGLGetVersion(display, &major, &minor);
    if(major == 3 && minor < 2) {
        Error() << "Platform::WindowlessCglApplication::tryCreateContext(): CGL version 2.1 or greater is required";
        return false;
    }

    int configCount = 0;
    CGLPixelFormatAttribute pfAttributes[4] = {
    kCGLPFAAccelerated,   
    kCGLPFAOpenGLProfile, 
    (CGLPixelFormatAttribute) kCGLOGLPVersion_3_2_Core,
    (CGLPixelFormatAttribute) 0
    };

    CGLError = CGLChoosePixelFormat(pfAttributes, ,&configCount)

    /* Choose config */
    //int configCount = 0;
    //static const int fbAttributes[] = { None };
    GLXFBConfig* configs = glXChooseFBConfig(display, DefaultScreen(display), fbAttributes, &configCount);
    if(!configCount) {
        Error() << "Platform::WindowlessCglApplication::tryCreateContext(): no supported framebuffer configuration found";
        return false;
    }

    GLint contextAttributes[] = {
        #ifdef MAGNUM_TARGET_GLES
        GLX_CONTEXT_MAJOR_VERSION_ARB, 2,
        GLX_CONTEXT_MINOR_VERSION_ARB, 0,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_ES2_PROFILE_BIT_EXT,
        #endif
        0
    };

    /** @todo Use some extension wrangler for this, not GLEW, as it apparently needs context to create context, yo dawg wtf. */
    PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB = (PFNGLXCREATECONTEXTATTRIBSARBPROC) glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
    context = glXCreateContextAttribsARB(display, configs[0], nullptr, True, contextAttributes);
    if(!context) {
        Error() << "Platform::WindowlessCglApplication::tryCreateContext(): cannot create context";
        return false;
    }

    /* Create pbuffer */
    int pbufferAttributes[] = {
        GLX_PBUFFER_WIDTH,  32,
        GLX_PBUFFER_HEIGHT, 32,
        None
    };
    pbuffer = glXCreatePbuffer(display, configs[0], pbufferAttributes);

    XFree(configs);

    /* Set OpenGL context as current */
    if(!glXMakeContextCurrent(display, pbuffer, pbuffer, context)) {
        Error() << "Platform::WindowlessCglApplication::tryCreateContext(): cannot make context current";
        return false;
    }

    c = new Context;
    return true;
}

WindowlessCglApplication::~WindowlessCglApplication() {
    delete c;

    glXMakeCurrent(display, None, nullptr);
    glXDestroyContext(display, context);
}

}}
