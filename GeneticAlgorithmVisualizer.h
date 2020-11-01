#ifndef GENETIC_ALGORITHM_DRAW_H
#define GENETIC_ALGORITHM_DRAW_H
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <stdio.h>
#include <stdlib.h>
#define RENDER_EPOCH_EVERY 50
#include "GeneticAlgorithm.h"

// Object identifiers
enum{
	ID_MAINPANEL = 1,
	ID_EPOINPUT = 11,
	ID_POPINPUT = 12,
	ID_HOFINPUT = 13,
	ID_MUTINPUT = 14,
	ID_NEWINPUT = 15,
	ID_EPOTEXT = 21,
	ID_POPTEXT = 22,
	ID_HOFTEXT = 23,
	ID_MUTTEXT = 24,
	ID_NEWTEXT = 25,
	ID_RESETBUTTON = 31,
	ID_GOTOBUTTON = 32,
	ID_NEXTBUTTON = 33,
	ID_UPDATECANVAS = 101,
};

// Class declarations
class DrawApp : public wxApp{
	public:
    	virtual bool OnInit();
};

class Canvas2D : public wxGLCanvas {
	public:
    	Canvas2D(wxFrame* parent, wxSize size);
    	~Canvas2D();
    	void Redraw();
    private:
		wxGLContext *context;
};

class MainWindow : public wxFrame{
	private:
		// wxWidgets version 2.9
		DECLARE_EVENT_TABLE();
	public:
    	MainWindow();
    	~MainWindow();
	private:
    	void OnExit(wxCommandEvent& event);
	    void OnAbout(wxCommandEvent& event);
	    void OnSize(wxSizeEvent& event);
	    void OnMaximize(wxMaximizeEvent& event);
	    void ResetGA(wxCommandEvent& event);
	    void GotoGA(wxCommandEvent& event);
	    void NextGA(wxCommandEvent& event);
	    void UpdateEvent(wxThreadEvent& event);
	    void UpdateState();
	    float Clamp(float value, float min, float max);
	    char statusBarReport[256];
		wxMenu* menu;
		wxMenuBar* menuBar;
		wxPanel *mainPanel;
		wxTextCtrl *epoInput,*popInput,*hofInput,*mutInput,*newInput;
		wxStaticText *epoText,*popText,*hofText,*mutText,*newText;
		wxButton *resetButton,*gotoButton,*nextButton;
		Canvas2D* canvas;
		GeneticAlgorithm* ga;
		float** points;
		float bestResult;
		unsigned int numPoints,epoch,lastUpdate;
		friend class Canvas2D;
};

class ProcessingThread : public wxThread{
	public:
		ProcessingThread(MainWindow *frame,GeneticAlgorithm* ga,float** points,float* bestResult,unsigned int *numPoints,unsigned int *epoch,unsigned int targetEpoch);
		virtual void* Entry();
	private:
		MainWindow *frame;
		GeneticAlgorithm* ga;
		float **points,*bestResult;
		unsigned int *numPoints,*epoch, targetEpoch;
};
#endif
