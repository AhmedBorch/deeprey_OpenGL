#pragma once
#include <wx/panel.h>

class MyGLCanvas;
class wxSlider;
class wxCheckBox;


class ControlPanel : public wxPanel {
public:
    ControlPanel(wxWindow* parent, MyGLCanvas* glCanvas)
        : wxPanel(parent), m_glCanvas(glCanvas) {
        
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        
        // Rotation slider
        m_rotationSlider = new wxSlider(this, wxID_ANY, 0, 0, 360, 
                                      wxDefaultPosition, wxDefaultSize,
                                      wxSL_HORIZONTAL | wxSL_LABELS);
        m_rotationSlider->Bind(wxEVT_SLIDER, &ControlPanel::OnRotationChange, this);
        sizer->Add(new wxStaticText(this, wxID_ANY, "Rotation:"), 0, wxALL, 5);
        sizer->Add(m_rotationSlider, 0, wxEXPAND | wxALL, 5);
        
        // Wireframe checkbox
        m_wireframeCheck = new wxCheckBox(this, wxID_ANY, "Show Wireframe");
        m_wireframeCheck->Bind(wxEVT_CHECKBOX, &ControlPanel::OnWireframeToggle, this);
        sizer->Add(m_wireframeCheck, 0, wxALL, 5);
        
        SetSizer(sizer);
    }

private:
    MyGLCanvas* m_glCanvas;
    wxSlider* m_rotationSlider;
    wxCheckBox* m_wireframeCheck;
    
    void OnRotationChange(wxCommandEvent& event) {
        m_glCanvas->SetRotation(m_rotationSlider->GetValue());
    }
    
    void OnWireframeToggle(wxCommandEvent& event) {
        m_glCanvas->SetWireframe(event.IsChecked());
    }
};