#include "MyFrame.h"
#include "MyGLCanvas.h"
#include "ControlPanel.h"


MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "wxGLCanvas Example", wxDefaultPosition, wxSize(800, 600)) {
    wxPanel* mainPanel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    
    // Create GL canvas first (with null control panel temporarily)
    MyGLCanvas* glCanvas = new MyGLCanvas(mainPanel);
    // , nullptr);
    
    // Then create control panel with reference to glCanvas
    wxPanel* controlPanel = new ControlPanel(mainPanel, glCanvas);
    
    // Now set the control panel reference in the canvas
    glCanvas->SetControlPanel(controlPanel);
    
    mainSizer->Add(glCanvas, 1, wxEXPAND);
    mainSizer->Add(controlPanel, 0, wxEXPAND | wxLEFT, 5);
    
    mainPanel->SetSizer(mainSizer);
    SetClientSize(800, 600);
}