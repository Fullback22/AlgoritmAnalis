#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "iostream"
#include <math.h>
#include <vector>
#include <iterator>
#include "Oblast.h"
#include "Analis.h"


void f_itog_obedinenie(std::vector<Oblast> &, std::vector<Oblast> &, std::vector <Oblast> &, int proc_nalosheniy = 80, double mnoth_dlin = 1.2);
void f_poisk_granic_vevlet(cv::Mat *, std::vector<int> &, std::vector<int> &, int const, int const, bool const, int const);
void f_poisk_minimumov(std::vector<int>const &, std::vector<int> &, std::vector<int> &, std::vector<int> &);
bool f_init_oblasti(std::vector<Oblast> &, std::vector<int> *, std::vector <int> *, int const, int, std::vector<int> *, bool const);
void f_poisk_sosedey(std::vector<Oblast> &, int const *, int const *, int const, std::vector<int> const *, bool const);
void f_filtr_oblstei(std::vector<Oblast> &, std::vector<Oblast> &, int const, int, bool const);
void f_obed_oblastei(std::vector<Oblast> &, std::vector<Oblast> &);
void f_poisk_oblasti_po_coordinate(cv::Mat const *, std::vector<Oblast> &,int porog=7, bool col = false, int start_row = 0, int start_col = 0);
void f_analis(Analis &out_name, cv::Mat const * isod, cv::Mat const *temp, cv::Mat const *out,int frame);
void myMouseCallback_2(int event, int x, int y, int flag, void* param);
int count_zero(cv::Point &p_1, cv::Point &p_2, cv::Mat& out_img);
//void myMouseCallback_1(int event, int x, int y, int flag, void* param);