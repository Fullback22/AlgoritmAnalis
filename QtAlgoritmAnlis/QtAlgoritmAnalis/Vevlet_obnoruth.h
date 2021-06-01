#pragma once
#include "Oblast.h"
#include "funk.h"
#include "Cel.h" 
#include "Analis.h"
class Vevlet_obnoruth
{
protected:
	std::vector<Cel> celi;
	std::vector<Oblast> isxod_oblasti;
	std::vector<Oblast> template_obl;
	cv::Mat obrobat_img;
	int procent_naloshenia;
	int mnothitel_dlin;
	int vivlet_porog;
	bool fractalEnable;
	int medianBlurSize;
	int gausianBlurSize;
public:
	Vevlet_obnoruth();
	Vevlet_obnoruth(cv::Mat &, int mnothitel = 1.2, int procent=80,int porog=7);
	~Vevlet_obnoruth();
	void poisk();
	void set_img(cv::Mat*);
	void clean();
	//Oblast* get_element(int);
	int chislo_obektov();
	int chislo_templ_obl();
	void fractal_filter(int chislo_iteracii = 20);
	void korrelycion_pois(double porog = 0.8, int n_celi = 0);
	void draw(int n_celi=0);
	void set_temp_obl();
	void draw_cel(cv::Mat &img);
	int find_object();
	void write(Analis &out);
	void setParams(int vevletThresh, int medianBlurSize, int gausianBlurSize, bool fractalEnable);
	void getPredictCoordinate(std::vector<int>& pred_x, std::vector<int>& pred_y, std::vector<int>& pred_width, std::vector<int>& pred_heigth);
};

