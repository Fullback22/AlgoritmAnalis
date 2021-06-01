#include "Vevlet_obnoruth.h"

Vevlet_obnoruth::Vevlet_obnoruth()
{
	procent_naloshenia = 80;
	mnothitel_dlin = 1.2;
	vivlet_porog = 7;
	isxod_oblasti.resize(0);
}

Vevlet_obnoruth::Vevlet_obnoruth(cv::Mat &isxod,int mnothitel,int procent,int porog)
{
	obrobat_img = isxod;
	procent_naloshenia = procent;
	mnothitel_dlin = mnothitel;
	isxod_oblasti.resize(0);
	vivlet_porog = porog;
}
Vevlet_obnoruth::~Vevlet_obnoruth()
{
}
void Vevlet_obnoruth::poisk()
{
	std::vector<Oblast> obedinee_row;
	std::vector<Oblast> obedinee_col;
	f_poisk_oblasti_po_coordinate(&obrobat_img, obedinee_row,vivlet_porog, false);
	f_poisk_oblasti_po_coordinate(&obrobat_img, obedinee_col,vivlet_porog, true);
	f_itog_obedinenie(obedinee_row, obedinee_col, isxod_oblasti,procent_naloshenia,mnothitel_dlin);
	for (int i{ 0 }; i < isxod_oblasti.size(); i++)
	{
		for (int j{ i + 1 }; j < isxod_oblasti.size(); j++)
		{
			if (isxod_oblasti[i].get_first_col() == isxod_oblasti[j].get_first_col() && isxod_oblasti[i].get_first_row() == isxod_oblasti[j].get_first_row() &&
				isxod_oblasti[i].get_end_col() == isxod_oblasti[j].get_end_col() && isxod_oblasti[i].get_end_row() == isxod_oblasti[j].get_end_row())
			{
				isxod_oblasti.erase(isxod_oblasti.begin() + j);
				--j;
			}
		}
	}
	int a;
}
int Vevlet_obnoruth::chislo_obektov()
{
	return isxod_oblasti.size();
}
void Vevlet_obnoruth::fractal_filter(int chislo_iteracii)
{
	if (fractalEnable)
	{
		for (int i{ 0 }; i < isxod_oblasti.size(); i++)
		{
			double fractal = isxod_oblasti[i].fraktal(obrobat_img, chislo_iteracii);
			if (1.1 >fractal || fractal > 1.8)
			{
				isxod_oblasti.erase(isxod_oblasti.begin() + i);
				--i;
			}
		}
	}
	else
	{
	}
}
void Vevlet_obnoruth::draw(int n)
{
	if (n == 0)
	{
		for (int i{ 0 }; i < isxod_oblasti.size(); i++)
		{
			isxod_oblasti[i].draw_obl(obrobat_img);
		}
	}
	else
		isxod_oblasti[n-1].draw_obl(obrobat_img);
}
void Vevlet_obnoruth::korrelycion_pois(double porog, int n_oblasti)
{
	for (int j{ 0 }; j < celi.size(); j++)
	{
		bool all{ false };
		for (int i{ 0 }; i < isxod_oblasti.size() && all == false; ++i)
		{
			int width{ isxod_oblasti[i].get_end_col() - isxod_oblasti[i].get_first_col() };
			int heigth{ isxod_oblasti[i].get_end_row() - isxod_oblasti[i].get_first_row() };
			cv::Rect oblst(isxod_oblasti[i].get_first_col(), isxod_oblasti[i].get_first_row(), width, heigth);

			if (width > celi[j].get_wigth() && heigth > celi[j].get_length())
			{
				cv::Mat out_img;
				matchTemplate(obrobat_img(oblst), *celi[j].get_img(), out_img, cv::TM_CCOEFF_NORMED);
				for (int idx = 0; idx < out_img.rows; idx++)
				{
					for (int idy = 0; idy < out_img.cols; idy++)
					{
						if (out_img.at<float>(idx, idy) < 0)
							out_img.at<float>(idx, idy) = 0;
					}
				}
				cv::Scalar std, meanImg;
				meanStdDev(out_img, meanImg, std);
				double T{ meanImg[0] + 3.3 * std[0] };
				threshold(out_img, out_img, T, 1, cv::THRESH_BINARY);
				if (cv::countNonZero(out_img) > 0)
				{

					int min_size{ 0 };
					if (width > heigth)
						min_size = heigth;
					else
						min_size = width;

					if (min_size < celi[j].get_wigth() * 3 || min_size < celi[j].get_length() * 3)
					{
						celi[j].detect();
						celi[j].set_cel(&isxod_oblasti[i], &obrobat_img(oblst));
						isxod_oblasti.erase(isxod_oblasti.begin() + i);
						all = true;
						i--;
					}
				}
			}
		}

		for (int i{ 0 }; i<isxod_oblasti.size() && all==false; ++i)
		{
			double leng{ (double)celi[j].get_length() / 100 };
			double wigt{ (double)celi[j].get_wigth() / 100  };
			if (celi[j].get_end_y() - leng*20  < isxod_oblasti[i].get_first_row() || celi[j].get_y() + leng*20> isxod_oblasti[i].get_end_row() ||
				celi[j].get_x() + wigt*20 > isxod_oblasti[i].get_end_col() || celi[j].get_end_x() - wigt*20 < isxod_oblasti[i].get_first_col())
			{
			}
			else
			{
				double dlina = ((double)isxod_oblasti[i].get_end_col() - (double)isxod_oblasti[i].get_first_col()) ;
				double shirina = ((double)isxod_oblasti[i].get_end_row() - (double)isxod_oblasti[i].get_first_row()) ;
				if ((dlina > wigt * 80 && dlina<wigt * 120) || (shirina>leng * 80 && shirina < leng * 120))
				{
					celi[j].set_cel(&isxod_oblasti[i], &obrobat_img);
					celi[j].detect();
					all = true;
					isxod_oblasti.erase(isxod_oblasti.begin() + i);
					--i;
				}
			}
		}

		if (isxod_oblasti.size() == 0 && all == false)
		{
			cv::Mat out_img;
			matchTemplate(obrobat_img, *celi[j].get_img(), out_img, cv::TM_CCOEFF_NORMED);
			for (int idx = 0; idx < out_img.rows; idx++)
			{
				for (int idy = 0; idy < out_img.cols; idy++)
				{
					if (out_img.at<float>(idx, idy) < 0)
						out_img.at<float>(idx, idy) = 0;
				}
			}
			cv::Scalar std, meanImg;
			meanStdDev(out_img, meanImg, std);
			double T{ meanImg[0] + 3.3 * std[0] };
			threshold(out_img, out_img, T, 1, cv::THRESH_BINARY);
			if (cv::countNonZero(out_img) > 0)
			{
				int x{ 0 };
				int y{ 0 };
				for (int str{ 0 }; str < out_img.rows&&x == 0; ++str)
				{
					for (int col{ 0 }; col < out_img.cols&&y == 0; ++col)
					if (out_img.at<float>(str, col) != 0)
					{
						x = str;
						y = col;
						celi[j].set_cel(col, str, obrobat_img);
						celi[j].detect();	
						all = true;
					}
				}
			}
		}
		celi[j].lose_cel(all);
		if (celi[j].get_k() >= 2)
		{
			celi.erase(celi.begin() + j);
			--j;
		}
	}
	
	for (int j{ 0 }; j < isxod_oblasti.size(); j++)
	{			
		int width{ isxod_oblasti[j].get_end_col() - isxod_oblasti[j].get_first_col() };
		int heigth{ isxod_oblasti[j].get_end_row() - isxod_oblasti[j].get_first_row() };
		cv::Rect oblst(isxod_oblasti[j].get_first_col(), isxod_oblasti[j].get_first_row(), width, heigth);
		
		int chislo_sovpodeniy{ 0 };
		for (int i{ 0 }; i < template_obl.size(); ++i)
		{
			cv::Mat out_img;
			int width_temp{ template_obl[i].get_end_col() - template_obl[i].get_first_col() };
			int heigth_temp{ template_obl[i].get_end_row() - template_obl[i].get_first_row() };
			cv::Rect oblst_temp(template_obl[i].get_first_col(), template_obl[i].get_first_row(), width_temp, heigth_temp);
			if (width >= width_temp && heigth >= heigth_temp)
			{
				matchTemplate(obrobat_img(oblst), obrobat_img(oblst_temp), out_img, cv::TM_CCOEFF_NORMED);
				for (int idx = 0; idx < out_img.rows; idx++)
				{
					for (int idy = 0; idy < out_img.cols; idy++)
					{
						if (out_img.at<float>(idx, idy) < 0)
							out_img.at<float>(idx, idy) = 0;
					}
				}
				cv::Scalar std, meanImg;
				meanStdDev(out_img, meanImg, std);
				double T{ meanImg[0] + 1.7 * std[0] };
				threshold(out_img, out_img, T, 1, cv::THRESH_BINARY);
				if (cv::countNonZero(out_img) > 0)
				{
					int min_size{ 0 };
					if (width > heigth)
						min_size = heigth;
					else
						min_size = width;

					if (min_size < width_temp * 3 || min_size < heigth_temp * 3)
					{
						++chislo_sovpodeniy;
						template_obl.erase(template_obl.begin() + i);
						i--;
					}
				}
			}
			else if (width < width_temp && heigth < heigth_temp)
			{
				int chislo_sovpodeni_temp{ 0 };
				for (int j_1{ j }; j_1 < isxod_oblasti.size(); ++j_1)
				{
					int width_1{ isxod_oblasti[j_1].get_end_col() - isxod_oblasti[j_1].get_first_col() };
					int heigth_1{ isxod_oblasti[j_1].get_end_row() - isxod_oblasti[j_1].get_first_row() };
					cv::Rect oblst_1(isxod_oblasti[j_1].get_first_col(), isxod_oblasti[j_1].get_first_row(), width_1, heigth_1);
					if (width_1 < width_temp && heigth_1 < heigth_temp)
					{
						matchTemplate(obrobat_img(oblst_temp), obrobat_img(oblst_1), out_img, cv::TM_CCOEFF_NORMED);
						for (int idx = 0; idx < out_img.rows; idx++)
						{
							for (int idy = 0; idy < out_img.cols; idy++)
							{
								if (out_img.at<float>(idx, idy) < 0)
									out_img.at<float>(idx, idy) = 0;
							}
						}
						cv::Scalar std, meanImg;
						meanStdDev(out_img, meanImg, std);
						double T{ meanImg[0] + 1.7 * std[0] };
						threshold(out_img, out_img, T, 1, cv::THRESH_BINARY);
						if (cv::countNonZero(out_img)>0)
						{
							int min_size{ 0 };
							if (width_temp > heigth_temp)
								min_size = heigth_temp;
							else
								min_size = width_temp;

							if (min_size < width_1 * 3 || min_size < heigth_1 * 3)
							{
								++chislo_sovpodeni_temp;
								isxod_oblasti.erase(isxod_oblasti.begin() + j_1);
								j_1--;
							}
						}
					}
				}
				if (chislo_sovpodeni_temp > 0)
				{
					celi.emplace_back(&template_obl[i], &obrobat_img(oblst_temp));
					template_obl.erase(template_obl.begin() + i);
					--i;
				}
			}
		}	
		if (chislo_sovpodeniy > 0 && isxod_oblasti.size()>0)
			{
				
				if (isxod_oblasti.size() == j)
				{
					celi.emplace_back(&isxod_oblasti[j-1], &obrobat_img(oblst));
					isxod_oblasti.erase(isxod_oblasti.begin() + j - 1);
				}
				else
				{
					celi.emplace_back(&isxod_oblasti[j], &obrobat_img(oblst));
					isxod_oblasti.erase(isxod_oblasti.begin() + j);
				}
					
				--j;
			}
	}
}
void Vevlet_obnoruth::set_temp_obl()
{
	template_obl.clear();
	for (int i{ 0 }; i < isxod_oblasti.size(); ++i)
		template_obl.emplace_back(isxod_oblasti[i]);
}
void Vevlet_obnoruth::set_img(cv::Mat*isxod)
{
	cv::Mat bufer = *isxod;
	bufer.copyTo(obrobat_img);
	if (medianBlurSize >= 3)
		cv::medianBlur(obrobat_img, obrobat_img, medianBlurSize);
	if (gausianBlurSize >= 3)
		cv::GaussianBlur(obrobat_img, obrobat_img, cv::Size(gausianBlurSize, gausianBlurSize), 1);
}
void Vevlet_obnoruth::clean()
{
	isxod_oblasti.clear();
}
int Vevlet_obnoruth::chislo_templ_obl()
{
	return template_obl.size();
}
void Vevlet_obnoruth::draw_cel(cv::Mat &img)
{
	for (int w{ 0 }; w < celi.size(); w++)
	{
		if (celi[w].get_m()==3)
			celi[w].draw(img);
	}
}
int Vevlet_obnoruth::find_object()
{
	int obj{ 0 };
	for (int w{ 0 }; w < celi.size(); w++)
	{
		if (celi[w].get_m() == 3)
			++obj;
	}
	return obj;
}
void Vevlet_obnoruth::write(Analis &out)
{
	/*for (int w{ 0 }; w < celi.size(); w++)
	{
		if (celi[w].get_m() == 3)
			out.write_predict(celi[w].get_x(), celi[w].get_y(), celi[w].get_end_x(), celi[w].get_end_y());
	}	*/
}
void Vevlet_obnoruth::setParams(int vevletThresh_, int medianBlurSize_, int gausianBlurSize_, bool fractalEnable_)
{
	if (vevletThresh_<0)
		vivlet_porog = 1;
	else
		vivlet_porog = vevletThresh_;

	if (medianBlurSize_ > 33)
		medianBlurSize = 33;
	else if (medianBlurSize_ < 1)
		medianBlurSize = 3;
	else if (medianBlurSize_ % 2 == 0)
		medianBlurSize = ++medianBlurSize_;
	else
		medianBlurSize = medianBlurSize_;

	if (gausianBlurSize_ > 33)
		gausianBlurSize = 33;
	else if (gausianBlurSize_ < 1)
		gausianBlurSize = 3;
	else if (gausianBlurSize_ % 2 == 0)
		gausianBlurSize = ++gausianBlurSize_;
	else
		gausianBlurSize = gausianBlurSize_;

	fractalEnable = fractalEnable_;
}

void Vevlet_obnoruth::getPredictCoordinate(std::vector<int>& pred_x, std::vector<int>& pred_y, std::vector<int>& pred_width, std::vector<int>& pred_heigth)
{
	for (size_t i{ 0 }; i < celi.size(); ++i)
	{
		pred_x.push_back(celi[i].get_x());
		pred_y.push_back(celi[i].get_y());
		pred_width.push_back(celi[i].get_wigth());
		pred_heigth.push_back(celi[i].get_length());
	}
}
