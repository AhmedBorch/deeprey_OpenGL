#include "MyGLCanvas.h"
#include <GL/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <wx/dcclient.h> 
#include <wx/panel.h>  


wxBEGIN_EVENT_TABLE(MyGLCanvas, wxGLCanvas)
    EVT_PAINT(MyGLCanvas::OnPaint)
    EVT_SIZE(MyGLCanvas::OnSize)
    EVT_LEFT_DOWN(MyGLCanvas::OnMouseDown)
    EVT_LEFT_UP(MyGLCanvas::OnMouseUp)
wxEND_EVENT_TABLE()

// Constructor
MyGLCanvas::MyGLCanvas(wxWindow* parent)
    : wxGLCanvas(parent, wxID_ANY, nullptr, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),      
    m_context(new wxGLContext(this)),
    m_texture(0),
    m_imgWidth(0),
    m_imgHeight(0),
    m_initialized(false),
    m_buttonPressed(false),
    m_showControls(false),
    m_rotationAngle(0.0f)
    {}


MyGLCanvas::~MyGLCanvas() {
    if (m_texture) {
        glDeleteTextures(1, &m_texture);
    }
    delete m_context;
}

void MyGLCanvas::LoadTexture() {
    SetCurrent(*m_context);
    
    // Load the image
    int channels;
    unsigned char* data = nullptr;
    GLuint tempTexture = 0;  // Use temporary variable for safety

    try {
        // Try block starts here - wraps all operations that might fail
        data = stbi_load("../resources/play1.png", 
                        &m_imgWidth, &m_imgHeight, &channels, STBI_rgb_alpha);
        if (!data) {
            const char* error = stbi_failure_reason();
            throw std::runtime_error(std::string("Failed to load image: ") + (error ? error : "Unknown error"));
        }

        // Generate texture
        glGenTextures(1, &tempTexture);
        if (tempTexture == 0) {
            throw std::runtime_error("Failed to generate OpenGL texture");
        }

        glBindTexture(GL_TEXTURE_2D, tempTexture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Upload texture data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imgWidth, m_imgHeight, 
                    0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // Only commit to member variable if everything succeeded
        m_texture = tempTexture;
        tempTexture = 0;  // Prevent cleanup in catch block

        stbi_image_free(data);
        data = nullptr;
    }
    catch(const std::exception& e) {
        // Cleanup resources if an error occurred
        if (tempTexture) {
            glDeleteTextures(1, &tempTexture);
        }
        if (data) {
            stbi_image_free(data);
        }
        
        // Log the error and rethrow
        wxLogError("%s", e.what());
        throw;  // Propagate the exception to the caller
    }
}
void MyGLCanvas::RenderButton() {
    if (!m_texture) return;

    // Enable textures and blending
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Get window size
    wxSize size = GetSize();
    
    // Define button position and size (in pixels)
    float aspect = (float)m_imgHeight/m_imgWidth ;
    float buttonWidth = 100.0f;
    float buttonHeight = buttonWidth * aspect;  // Maintain aspect ratio
    float buttonX = 0.0f;
    float buttonY = (size.GetHeight() - buttonHeight) / 2.0f;

    // start using the original window width, after it's assigned a value
    if (canvasWidth>0.0f) {
        buttonX = (canvasWidth - buttonWidth) / 2.0f;
    }
    else{
        buttonX = (size.GetWidth() - buttonWidth) / 2.0f;
    }

    // Store initial position only once
    if (!m_initialButtonPosSet) {
        buttonX = (size.GetWidth() - buttonWidth) / 2.0f;
        m_originalButtonRect = wxRect(buttonX, buttonY, buttonWidth, buttonHeight);
        m_initialButtonPosSet = true;

        // get the initial width of the canvas
        canvasWidth = size.GetWidth();
    }

    // Always use original position for click detection
    m_buttonRect = m_originalButtonRect;
    // Convert to OpenGL coordinates (-1 to 1)
    float glX1 = (buttonX / canvasWidth) * 2.0f - 1.0f;
    float glY1 = 1.0f - (buttonY / size.GetHeight()) * 2.0f;
    float glX2 = ((buttonX + buttonWidth) / canvasWidth) * 2.0f - 1.0f;
    float glY2 = 1.0f - ((buttonY + buttonHeight) / size.GetHeight()) * 2.0f;

    
    glBindTexture(GL_TEXTURE_2D, m_texture);
    if (m_buttonPressed) {
        // Darken the button when pressed
        glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
    } else {
        // Force white base color
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }


    // Draw textured quad
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(glX1, glY2);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(glX2, glY2);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(glX2, glY1);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(glX1, glY1);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void MyGLCanvas::OnPaint(wxPaintEvent& WXUNUSED(event)) {
    SetCurrent(*m_context);
    wxPaintDC dc(this);

    if (!m_initialized) {
        InitGL();
        LoadTexture();
        m_initialized = true;
    }

    glClearColor(0.1f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // rotation,wire frame settings
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef(m_rotationAngle, 0.0f, 0.0f, 1.0f);
    
    if (m_showWireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    // Draw triagle
    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex2f(-0.5f, -0.5f);
        glColor3f(0, 1, 0); glVertex2f(0.5f, -0.5f);
        glColor3f(0, 0, 1); glVertex2f(0.0f, 0.5f);
    glEnd();

    // Draw a colored quad
    glBegin(GL_QUADS);
    glColor3f(1, 0, 0); glVertex2f(-0.2f, -0.2f);
    glColor3f(0, 1, 0); glVertex2f(-0.2f, 0.2f);
    glColor3f(0, 0, 1); glVertex2f(0.2f, 0.2f);
    glColor3f(0, 1, 0); glVertex2f(0.2f, -0.2f);
    glEnd();

    // Render the button
    RenderButton();

    glFlush();
    SwapBuffers();
}


void MyGLCanvas::OnSize(wxSizeEvent& event) {
    Refresh();
    event.Skip();
}

void MyGLCanvas::InitGL() {
    SetCurrent(*m_context);
    
    // Basic OpenGL initialization
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Set up the viewport
    wxSize size = GetClientSize();
    glViewport(0, 0, size.GetWidth(), size.GetHeight());
}

void MyGLCanvas::OnMouseDown(wxMouseEvent& event) {
    wxPoint mousePos = event.GetPosition();
    
    // Convert to client coordinates of the parent (accounting for panel)
    wxPoint clientPos = GetParent()->ScreenToClient(ClientToScreen(mousePos));
    
    if (m_originalButtonRect.Contains(clientPos)) {
        m_buttonPressed = true;
        Refresh();
    }
    event.Skip();
}

void MyGLCanvas::OnMouseUp(wxMouseEvent& event) {
    if (m_buttonPressed) {
        m_buttonPressed = false;
        wxPoint mousePos = event.GetPosition();
        wxPoint clientPos = GetParent()->ScreenToClient(ClientToScreen(mousePos));
        
        if (m_originalButtonRect.Contains(clientPos)) {
            m_showControls = !m_showControls;
            m_controlPanel->Show(m_showControls);
            GetParent()->Layout();
        }
        Refresh();
    }
    event.Skip();
}

void MyGLCanvas::SetRotation(float angle) {
    m_rotationAngle = angle;
    Refresh();
}

void MyGLCanvas::SetWireframe(bool show) {
    m_showWireframe = show;
    Refresh();
}

void MyGLCanvas::SetControlPanel(wxPanel* panel) {
    m_controlPanel = panel;
    m_controlPanel->Hide();
}
