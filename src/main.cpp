#include <memory>
#include <wx/glcanvas.h>
#ifdef __WXGTK__
    #include <GL/gl.h>
#else
    #include <OpenGL/gl.h>
#endif
#include "MyApp.h"


wxIMPLEMENT_APP(MyApp);
