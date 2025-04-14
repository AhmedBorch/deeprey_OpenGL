#include <memory>
#include <GL/glew.h> 
#include <wx/glcanvas.h>
#ifdef __WXGTK__
    #include <GL/gl.h>
#else
    // #include <OpenGL/gl.h>
    #include <GL/gl.h>
#endif
#include "MyApp.h"


wxIMPLEMENT_APP(MyApp);
