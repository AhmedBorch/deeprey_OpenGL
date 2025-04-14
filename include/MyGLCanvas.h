#pragma once
#include <wx/glcanvas.h>
#include <GL/gl.h>

class wxGLContext;
class wxPanel;


class MyGLCanvas : public wxGLCanvas {
public:
    MyGLCanvas(wxWindow* parent);
    // , wxPanel* controlPanel);
    ~MyGLCanvas();

    // Control panel
    void SetRotation(float angle);
    void SetWireframe(bool show);
    void SetControlPanel(wxPanel* panel);

private:
    wxGLContext* m_context;
    bool m_initialized;

    //button related members
    GLuint m_texture;
    int m_imgWidth, m_imgHeight;
    wxRect m_buttonRect; //button size and position in screen coordinates
    bool m_buttonPressed;
    // memebers for the control panel
    wxPanel* m_controlPanel;
    bool m_showControls = false;
    float m_rotationAngle = 0.0f;
    bool m_showWireframe = false;
    wxRect m_originalButtonRect;  // Stores initial button position
    bool m_initialButtonPosSet = false;
    float canvasWidth = 0.0f;

    void InitGL();
    // texture loading from button png and rendering
    void LoadTexture();
    void RenderButton();
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    // Button Event handlers
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);


    wxDECLARE_EVENT_TABLE();
};