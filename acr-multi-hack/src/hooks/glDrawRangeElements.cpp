#include <pch.h>
#include <base.h>

void __stdcall Base::Hooks::glDrawRangeElements(GLenum mode,
    GLuint start,
    GLuint end,
    GLsizei count,
    GLenum type,
    const void* indices)
{
    if (mode == GL_TRIANGLE_STRIP) {
        //glColor3f(255, 0, 0);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDisable(GL_DEPTH_TEST);
    }

    Data::o_glDrawElements(mode, start, end, count, type, indices);

    if (mode == GL_TRIANGLE_STRIP)
    {
        glEnable(GL_DEPTH_TEST);
    }
}