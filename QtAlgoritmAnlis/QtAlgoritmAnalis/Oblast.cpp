#include "Oblast.h"


Oblast::Oblast()
{
}
Oblast::~Oblast()
{
}

void Oblast::set_oblast(int const *mass_minimumov_first, int const *mass_minimumov_second, int n_coordinat, int const chislo_otreskov, int const* vevlet_granic, bool const col = false)
{
	if (mass_minimumov_first != 0 && mass_minimumov_second != 0)
	{
		chislo_elementov = chislo_otreskov;
		first_gr.resize(chislo_elementov);
		second_gr.resize(chislo_elementov);
		for (int i = 0; i <chislo_elementov; i++)
		{
			first_gr[i] = *(mass_minimumov_first + i);
			second_gr[i] = *(mass_minimumov_second + i);
		}
		if (col == false)
		{
			first_col = *vevlet_granic;
			end_col = *(vevlet_granic + chislo_elementov - 1);
			first_row = end_row = n_coordinat;
		}
		else
		{
			first_col = end_col = n_coordinat;
			first_row = *vevlet_granic;
			end_row = *(vevlet_granic + chislo_elementov - 1);
		}
	}
}
int Oblast::get_first_gr(int const i)
{
	return first_gr[i];
}
int Oblast::get_second_gr(int const i)
{
	return second_gr[i];
}
void Oblast::reset_obl_new(int const *mass_first, int const *mass_second, int const first_element, int const end_element, int const coordinate, int const *mass_max, bool const col = false)
{
	chislo_elementov = end_element - first_element + 1;
	first_gr.resize(chislo_elementov);
	second_gr.resize(chislo_elementov);
	int j{ 0 };
	for (int i = first_element; i <= end_element; i++)
	{
		first_gr[j] = *(mass_first + i);
		second_gr[j] = *(mass_second + i);
		j++;
	}
	if (col == false)
	{
		if (*(mass_max + first_element) < first_col)
			first_col = *(mass_max + first_element);
		if (*(mass_max + end_element)>end_col)
			end_col = *(mass_max + end_element);
		end_row = coordinate;
	}
	else
	{
		if (*(mass_max + first_element) < first_row)
			first_row = *(mass_max + first_element);
		if (*(mass_max + end_element)>end_row)
			end_row = *(mass_max + end_element);
		end_col = coordinate;
	}
}
void Oblast::set_first_row(int const a)
{
	first_row = a;
}
void Oblast::set_end_row(int const a)
{
	end_row = a;
}
void Oblast::set_first_col(int const a)
{
	first_col = a;
}
void Oblast::set_end_col(int const a)
{
	end_col = a;
}
int Oblast::get_end_row()
{
	return end_row;
}
int Oblast::get_first_row()
{
	return first_row;
}
int Oblast::get_end_col()
{
	return end_col;
}
int Oblast::get_first_col()
{
	return first_col;
}
int Oblast::get_razmer()
{
	return chislo_elementov;
}
void Oblast::draw_obl(cv::Mat in_out)
{
	for (int i = first_col; i < end_col; i++)
	{
		in_out.at<uchar>(first_row, i) = 255;
		in_out.at<uchar>(end_row, i) = 255;
	}
	for (int i = first_row; i < end_row; i++)
	{
		in_out.at<uchar>(i, first_col) = 255;
		in_out.at<uchar>(i, end_col) = 255;
	}
}
void Oblast::normalizade_row()
{
	end_col *= 2;
	first_col *= 2;
}
void Oblast::normalizade_col()
{
	first_row *= 2;
	end_row *= 2;
}
double Oblast::fraktal(cv::Mat isxod, int const iteracii = 20 )
{
	cv::Mat oblast_interesa;
	cv::Rect oblst(first_col, first_row, end_col - first_col + 1, end_row - first_row + 1);
	isxod(oblst).copyTo(oblast_interesa);
	Canny(oblast_interesa, oblast_interesa, 100, 200, 5);
	double summa_razmer_okna{ 0 }, summa_N{ 0 }, summa_razmer_okna_v_2{ 0 }, summa_razmer_oka_na_N{ 0 };
	for (int razmer_okna{ 2 }; razmer_okna <= 2 * iteracii; razmer_okna += 2)
	{
		if (end_row - first_row < 1 || end_col - first_col < 1)
			return 0;
		int mnothitel_cols{ 0 }, mnothitel_rows{ 0 };
		int N{ 0 };
		int end_col_oblasti{ oblast_interesa.cols - 1 }, end_row_oblasti{ oblast_interesa.rows - 1 };
		double col_okon_row{ (double)oblast_interesa.rows / (double)razmer_okna }, col_okon_col{ (double)oblast_interesa.cols / (double)razmer_okna };
		bool nepolnoe_row{ false };
		if ((int)col_okon_row < col_okon_row)
			nepolnoe_row = true;
		bool nepolnoe_col{ false };
		if ((int)col_okon_col < col_okon_col)
			nepolnoe_col = true;

		for (int i{ 0 }; (int)col_okon_row > mnothitel_rows;)
		{
			int j{ 0 };
			for (; (int)col_okon_col > mnothitel_cols;)
			{
				if (oblast_interesa.at<uchar>(i, j) == 255)
				{
					++mnothitel_cols;
					j = mnothitel_cols*razmer_okna;
					i = mnothitel_rows*razmer_okna;
					++N;
				}
				else if (j == (mnothitel_cols + 1)*razmer_okna - 1 && i == (mnothitel_rows + 1)*razmer_okna - 1)
				{
					i = mnothitel_rows*razmer_okna;
					++mnothitel_cols;
					j = mnothitel_cols*razmer_okna;
				}
				else if (j == (mnothitel_cols + 1)*razmer_okna - 1)
				{
					++i;
					j = mnothitel_cols*razmer_okna;
				}
				else
					++j;
			}
			++mnothitel_rows;
			i = mnothitel_rows*razmer_okna;
			mnothitel_cols = 0;
			j = 0;
		}
		mnothitel_cols = 0;
		mnothitel_rows = 0;
		int ostatok_cols{ oblast_interesa.cols - (int)col_okon_col*razmer_okna };
		int start_pixel{ oblast_interesa.cols - ostatok_cols }, stop_pixel{ oblast_interesa.cols - 1 };
		for (int i{ 0 }; i < oblast_interesa.rows && (int)col_okon_row > mnothitel_rows;)
		{
			int j{ start_pixel };
			for (; j <= stop_pixel && (int)col_okon_row > mnothitel_rows;)
			{
				if (oblast_interesa.at<uchar>(i, j) == 255)
				{
					++mnothitel_rows;
					j = start_pixel;
					i = mnothitel_rows*razmer_okna;
					++N;
				}
				else if (j == stop_pixel && i == (mnothitel_rows + 1)*razmer_okna - 1)
				{
					++mnothitel_rows;
					i = mnothitel_rows*razmer_okna;
					j = start_pixel;
				}
				else if (j == stop_pixel)
				{
					++i;
					j = start_pixel;
				}
				else
					++j;
			}
			++i;
		}
		mnothitel_cols = 0;
		mnothitel_rows = 0;
		int ostatok_rows{ oblast_interesa.rows - (int)col_okon_row*razmer_okna };
		int start_pixel_row{ oblast_interesa.rows - ostatok_rows }, stop_pixel_row{ oblast_interesa.rows - 1 };
		for (int i{ 0 }; i < oblast_interesa.cols && (int)col_okon_col > mnothitel_cols;)
		{
			int j{ start_pixel_row };
			for (; j <= stop_pixel_row && (int)col_okon_col > mnothitel_cols;)
			{
				if (oblast_interesa.at<uchar>(j, i) == 255)
				{
					++mnothitel_cols;
					j = start_pixel_row;
					i = mnothitel_cols*razmer_okna;
					++N;
				}
				else if (j == stop_pixel_row && i == (mnothitel_cols + 1)*razmer_okna - 1)
				{
					++mnothitel_cols;
					i = mnothitel_cols*razmer_okna;
					j = start_pixel_row;
				}
				else if (j == stop_pixel_row)
				{
					++i;
					j = start_pixel_row;
				}
				else
					++j;
			}
			++i;
		}
		for (int j{ start_pixel_row }; j < stop_pixel_row;)
		{

			for (int i{ start_pixel }; j < stop_pixel_row&& i <= stop_pixel;)
			{
				if (oblast_interesa.at<uchar>(j, i) == 255)
				{
					++N;
					j = stop_pixel_row;
					break;
				}
				else if (i == stop_pixel)
				{
					i = start_pixel;
					++j;
				}
				else
					++i;
			}
			j++;
		}

		double logN{ log10(N) }, logOkna{ log10(razmer_okna) };
		summa_razmer_okna += logOkna;
		summa_N += logN;
		summa_razmer_okna_v_2 += logOkna*logOkna;
		summa_razmer_oka_na_N += logN*logOkna;
	}
	double fraktalnay_razmernost{ (iteracii*summa_razmer_oka_na_N - summa_N*summa_razmer_okna) / (iteracii*summa_razmer_okna_v_2 - summa_razmer_okna*summa_razmer_okna) };
	return -fraktalnay_razmernost;
}