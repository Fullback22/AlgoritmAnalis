
#include"funk.h"
void f_itog_obedinenie(std::vector<Oblast> &in_rows, std::vector<Oblast> &in_cols, std::vector <Oblast> &out_obl, int proc_nalosheniy , double mnoth_dlin )
{
	for (int j = 0; j<in_rows.size(); j++)
	{
		int n_del_col{ 0 }, n_del_row{ 0 };
		double dlina = (in_rows[j].get_end_col() - in_rows[j].get_first_col());
		double shirina = in_rows[j].get_end_row() - in_rows[j].get_first_row();
		double dlina_80 = (dlina / 100) * proc_nalosheniy;
		double shirina_80 = (shirina / 100) * proc_nalosheniy;
		bool iz_col{ false }, iz_row{ false }, obed{ false };
		std::vector <int> iz_cols;
		for (int i = 0; i < in_cols.size(); i++)
		{
			if (in_rows[j].get_end_row()  < in_cols[i].get_first_row() || in_rows[j].get_first_row()  > in_cols[i].get_end_row() ||
				in_rows[j].get_first_col()  > in_cols[i].get_end_col() || in_rows[j].get_end_col()  < in_cols[i].get_first_col())
			{
			}
			else
			{
				bool itog_iz_obl_cols{ false };
				bool itog_iz_obl_rows{ false };
				bool obl_rows_net{ false };
				bool obed_1{ false }, obed_2{ false };

				double dlina_c = in_cols[i].get_end_col() - in_cols[i].get_first_col();
				double shirina_c = (in_cols[i].get_end_row() - in_cols[i].get_first_row());
				double dlina_c_80 = double(dlina_c / 100) * proc_nalosheniy;
				double shirina_c_80 = double(shirina_c / 100) * proc_nalosheniy;

				if (dlina_c < dlina_80 && shirina_c<shirina_80)
				{
					if ((in_cols[i].get_end_row() - shirina_c_80 > in_rows[j].get_first_row() && (in_cols[i].get_first_col() + dlina_c_80 < in_rows[j].get_end_col() ||
						in_cols[i].get_end_col() - dlina_c_80 > in_rows[j].get_first_col())) ||
						(in_cols[i].get_first_row() + shirina_c_80 < in_rows[j].get_end_row() && (in_cols[i].get_first_col() + dlina_c_80 < in_rows[j].get_end_col() ||
						in_cols[i].get_end_col() - dlina_c_80 > in_rows[j].get_first_col())))
					{
						itog_iz_obl_cols = true;
					}
					else
					{
						obl_rows_net = true;
					}

				}
				else
				{
					if ((in_rows[j].get_end_row() - shirina_80 > in_cols[i].get_first_row() && (in_rows[j].get_first_col() + dlina_80 < in_cols[i].get_end_col() ||
						in_rows[j].get_end_col() - dlina_80 > in_cols[i].get_first_col())) ||
						(in_rows[j].get_first_row() + shirina_80 < in_cols[i].get_end_row() && (in_rows[j].get_first_col() + dlina_80 < in_cols[i].get_end_col() ||
						in_rows[j].get_end_col() - dlina_80 > in_cols[i].get_first_col())))
						obed_1 = true;
					else
						obl_rows_net = true;
				}
				if (dlina_c > dlina*mnoth_dlin)
				{
					itog_iz_obl_rows = true;
				}
				if (shirina < shirina_c_80 && dlina<dlina_c_80)
					itog_iz_obl_rows = true;
				else if ((in_cols[i].get_end_row() - shirina_c_80 > in_rows[j].get_first_row() && (in_cols[i].get_first_col() + dlina_c_80 < in_rows[j].get_end_col() ||
					in_cols[i].get_end_col() - dlina_c_80 > in_rows[j].get_first_col())) ||
					(in_cols[i].get_first_row() + shirina_c_80 < in_rows[j].get_end_row() && (in_cols[i].get_first_col() + dlina_c_80 < in_rows[j].get_end_col() ||
					in_cols[i].get_end_col() - dlina_c_80 > in_rows[j].get_first_col())))
					obed_2 = true;
				else
					obl_rows_net = true;
				if (shirina > shirina_c*mnoth_dlin)
				{
					obed_1 = false;
					obed_2 = false;
					itog_iz_obl_cols = true;
				}

				if (obed_1 == true && obed_2 == true)
				{
					obed = true;
					if (in_cols[i].get_first_row() < in_rows[j].get_first_row())
						in_rows[j].set_first_row(in_cols[i].get_first_row());
					if (in_rows[j].get_end_row() < in_cols[i].get_end_row())
						in_rows[j].set_end_row(in_cols[i].get_end_row());
					if (in_rows[j].get_first_col()>in_cols[i].get_first_col())
						in_rows[j].set_first_col(in_cols[i].get_first_col());
					if (in_rows[j].get_end_col() < in_cols[i].get_end_col())
						in_rows[j].set_end_col(in_cols[i].get_end_col());
				}
				else if (itog_iz_obl_cols == true)
				{
					iz_col = true;
					iz_cols.emplace_back(i);
				}
				else if (itog_iz_obl_rows == true)
					iz_row = true;
			}
		}

		if (obed == true || iz_row == true)
		{
			out_obl.emplace_back(in_rows[j]);
		}
		else if (iz_col == true)
		{
			for (int i = 0; i < iz_cols.size(); i++)
			{
				out_obl.emplace_back(in_cols[iz_cols[i]]);
			}
		}
	}
}
void f_poisk_granic_vevlet(cv::Mat *in_image, std::vector<int> &vevlet_preobr, std::vector<int> &maximum_out,
	int const coor_row = 0, int const  coor_col = 0, bool const col = false, int const porog = 7)
{
	int n_coor{ 1 };
	int end_for{ in_image->cols };
	vevlet_preobr.resize(in_image->cols / 2);
	if (col == true)
	{
		n_coor = 0;
		end_for = in_image->rows;
		vevlet_preobr.resize(in_image->rows / 2);
	}
	int coordinata[2]{coor_row, coor_col};
	int next_coordinata[2]{coor_row, coor_col};
	++next_coordinata[n_coor];
	for (int i{ 0 }; next_coordinata[n_coor]<end_for; i++)
	{
		vevlet_preobr[i] = abs((in_image->at<uchar>(coordinata[0], coordinata[1]) - in_image->at<uchar>(next_coordinata[0], next_coordinata[1])) / 2);
		if (vevlet_preobr[i]>porog)
		{
			vevlet_preobr[i] = porog;
			maximum_out.emplace_back(coordinata[n_coor] / 2);
		}
		next_coordinata[n_coor] += 2;
		coordinata[n_coor] += 2;
	}
}
void f_poisk_minimumov(std::vector<int>const &vevlet, std::vector<int> &coor_maximumov, std::vector<int> &minimum_first, std::vector<int> &minimum_second)
{
	if (coor_maximumov.size() != 0)
	{
		int short start_point{ 0 };
		int short point_first{ 0 };
		int short point_second{ 0 };
		bool end_first{ false };
		bool end_second{ false };
		minimum_first.resize(coor_maximumov.size());
		minimum_second.resize(coor_maximumov.size());
		for (int short j{ 0 }; j<coor_maximumov.size();)
		{
			start_point = coor_maximumov[j];
			end_first = false;
			end_second = false;
			for (int h = 0; end_first == false || end_second == false; h++)
			{
				if (end_first == false)
				{
					if (start_point - h <= 0)
					{
						end_first = true;
						point_first = 0;
						minimum_first[j] = point_first;
					}
					else if (vevlet[start_point - h - 1] < vevlet[start_point - h])
					{
						point_first = start_point - 1 - h;
					}
					else if (vevlet[start_point - h - 1] > vevlet[start_point - h])
					{
						end_first = true;
						minimum_first[j] = point_first;
					}
				}

				if (end_second == false)
				{
					if (start_point + h >= (vevlet.size() - 1))
					{
						end_second = true;
						point_second = vevlet.size() - 1;
						minimum_second[j] = point_second;
					}
					else if (vevlet[start_point + h + 1] < vevlet[start_point + h])
					{
						point_second = start_point + 1 + h;
					}
					else if (vevlet[start_point + h + 1] > vevlet[start_point + h])
					{
						end_second = true;
						minimum_second[j] = point_second;
					}
				}
			}
			++j;
		}
	}
}
bool f_init_oblasti(std::vector<Oblast> &start_obl, std::vector<int> *mass_ferst, std::vector <int> *mass_second, int const coordinate, int chislo_elementov, std::vector<int> *vevlet_granic, bool col = false)
{
	if (mass_ferst->size() > 0)
	{
		start_obl.resize(mass_ferst->size());
		for (int i{ 0 }; i < mass_ferst->size(); i++)
			start_obl[i].set_oblast(mass_ferst->data() + i, mass_second->data() + i, coordinate,chislo_elementov, vevlet_granic->data() + i, col);
		return true;
	}
	else
		return false;
}
void f_poisk_sosedey(std::vector<Oblast> &old_oblast, int const *new_oblast_first, int const *new_oblast_second, int const coordinate, std::vector<int> const *vevlet_grnic, bool col = false)
{
	if (old_oblast.empty() == false && vevlet_grnic->empty() == false)
	{
		int n_oblasti{ 0 }, n_elementa{ 0 };
		int first_mass{ 0 }, second_mass{ 0 }, first_smeshenie{ 0 }, second_smeshenie{ 0 };
		int first_element(-1), end_element(0);
		for (bool i = false; i == false;)
		{
			if (old_oblast[n_oblasti].get_first_gr(n_elementa) > *(new_oblast_second + second_mass) &&
				old_oblast[n_oblasti].get_second_gr(n_elementa) > *(new_oblast_first + second_mass) && n_elementa == 0)
			{
				if (n_elementa == 0)
				{
					if (first_element != -1)
					{
						old_oblast[n_oblasti - 1].reset_obl_new(new_oblast_first, new_oblast_second, first_element, end_element, coordinate, vevlet_grnic->data(), col);
						first_element = -1;
						end_element = 0;
					}
					old_oblast.emplace(old_oblast.begin() + n_oblasti);
					old_oblast[n_oblasti].set_oblast(new_oblast_first + second_mass, new_oblast_second + second_mass, coordinate, 1, vevlet_grnic->data() + second_mass, col);
					second_smeshenie++;
					n_oblasti++;
					n_elementa = 0;
					if (second_mass != vevlet_grnic->size() - 1)
						second_mass = 0 + second_smeshenie;
					else
						i = true;
				}
				else
				{
					if (first_element == -1)
					{
						first_element = second_mass;
						end_element = second_mass;
					}
					else
						end_element = second_mass;
					second_smeshenie++;
					if (second_mass != vevlet_grnic->size() - 1)
						second_mass = 0 + second_smeshenie;
					else
					{
						if (first_element != -1)
							old_oblast[n_oblasti].reset_obl_new(new_oblast_first, new_oblast_second, first_element, end_element, coordinate, vevlet_grnic->data(), col);
						i = true;
					}

				}
			}

			else if (old_oblast[n_oblasti].get_first_gr(n_elementa) < *(new_oblast_second + second_mass) &&
				old_oblast[n_oblasti].get_second_gr(n_elementa) < *(new_oblast_first + second_mass))
			{
				if (n_elementa < old_oblast[n_oblasti].get_razmer() - 1)
					n_elementa++;
				else

				if (n_oblasti != old_oblast.size() - 1)
				{
					if (first_element != -1)
					{
						old_oblast[n_oblasti].reset_obl_new(new_oblast_first, new_oblast_second, first_element, end_element, coordinate, vevlet_grnic->data(), col);
						first_element = -1;
						end_element = 0;
					}
					n_elementa = 0;
					n_oblasti++;
				}

				else if (n_elementa == old_oblast[n_oblasti].get_razmer() - 1 && n_oblasti == old_oblast.size() - 1)
				{
					if (first_element != -1)
					{
						old_oblast[n_oblasti].reset_obl_new(new_oblast_first, new_oblast_second, first_element, end_element, coordinate, vevlet_grnic->data(), col);
						first_element = -1;
						end_element = 0;
					}
					for (; second_mass != vevlet_grnic->size();)
					{
						old_oblast.emplace(old_oblast.begin() + n_oblasti);
						old_oblast[n_oblasti].set_oblast(new_oblast_first + second_mass, new_oblast_second + second_mass, coordinate, 1, vevlet_grnic->data() + second_mass, col);
						second_mass++;
					}
					if (second_mass == vevlet_grnic->size())
						i = true;
				}
			}
			else
			{
				if (first_element == -1)
				{
					first_element = second_mass;
					end_element = second_mass;
				}
				else
					end_element = second_mass;
				second_smeshenie++;
				if (second_mass != vevlet_grnic->size() - 1)
				{
					second_mass = 0 + second_smeshenie;
				}
				else
				{
					if (first_element != -1)
					{
						old_oblast[n_oblasti].reset_obl_new(new_oblast_first, new_oblast_second, first_element, end_element, coordinate, vevlet_grnic->data(), col);
						first_element = -1;
						end_element = 0;
					}
					i = true;
				}
			}
		}
	}
}
void f_filtr_oblstei(std::vector<Oblast> &old_oblast, std::vector<Oblast> &out_oblasti, int const coordinate, int max_coordinate, bool col = false)
{
	for (int n_obl{ 0 }; n_obl < old_oblast.size(); n_obl++)
	{
		if (coordinate == max_coordinate - 1)
		{
			out_oblasti.emplace_back(old_oblast[n_obl]);
		}
		if (col == true)
		{
			if (old_oblast[n_obl].get_end_col() < coordinate)
			{
				out_oblasti.emplace_back(old_oblast[n_obl]);
				old_oblast.erase(old_oblast.begin() + n_obl);
			}
		}
		else
		{
			if (old_oblast[n_obl].get_end_row() < coordinate)
			{
				out_oblasti.emplace_back(old_oblast[n_obl]);
				old_oblast.erase(old_oblast.begin() + n_obl);
			}
		}
	}
	if (coordinate == max_coordinate - 1)
		old_oblast.clear();
}
void f_obed_oblastei(std::vector<Oblast> &oblasti_in, std::vector<Oblast> &oblasti_out)
{
	bool del{ true };
	for (;;)
	{
		int n_del{ 0 };
		if (del == true)
		{
			for (int i = 0; i < oblasti_in.size(); i++)
			{
				if (oblasti_in[i].get_end_row() - oblasti_in[i].get_first_row() == 0 || oblasti_in[i].get_end_col() - oblasti_in[i].get_first_col() == 0)
				{
					oblasti_in.erase(oblasti_in.begin() + i);
					i--;
				}
			}
			del = false;
		}

		for (int i{ 1 }; i < oblasti_in.size(); i++)
		{
			if ((oblasti_in[0].get_end_row() + 1  < oblasti_in[i].get_first_row()) || oblasti_in[0].get_first_row() - 1  > oblasti_in[i].get_end_row() ||
				oblasti_in[0].get_first_col() - 1 > oblasti_in[i].get_end_col() || oblasti_in[0].get_end_col() + 1  < oblasti_in[i].get_first_col())
			{
			}
			else
			{
				if (oblasti_in[i].get_first_row() < oblasti_in[0].get_first_row())
					oblasti_in[0].set_first_row(oblasti_in[i].get_first_row());
				if (oblasti_in[0].get_end_row() < oblasti_in[i].get_end_row())
					oblasti_in[0].set_end_row(oblasti_in[i].get_end_row());
				if (oblasti_in[0].get_first_col()>oblasti_in[i].get_first_col())
					oblasti_in[0].set_first_col(oblasti_in[i].get_first_col());
				if (oblasti_in[0].get_end_col() < oblasti_in[i].get_end_col())
					oblasti_in[0].set_end_col(oblasti_in[i].get_end_col());
				oblasti_in.erase(oblasti_in.begin() + i);
				n_del++;
			}
		}

		if (n_del == 0)
		{
			if (oblasti_in.size() != 0)
			{
				oblasti_out.emplace_back(oblasti_in[0]);
				oblasti_in.erase(oblasti_in.begin());
			}
			if (oblasti_in.size() == 0)
				break;
		}
	}
}
void f_poisk_oblasti_po_coordinate(cv::Mat const *isxod, std::vector<Oblast> &out_oblasti, int porog, bool col, int start_row, int start_col)
{
	cv::Mat in_image;
	isxod->copyTo(in_image);
	bool init_obl{ false };
	std::vector<int> maxim;
	std::vector<int> vevlet;
	std::vector<int> minimum_f, minimum_s;
	std::vector<Oblast> test;
	std::vector<Oblast> close_oblasti;
	int start_coordinate[2]{ start_row, start_col };
	int stop_coordinade[2]{in_image.rows, in_image.cols};
	int n_cor{ 0 };
	if (col == true)
		n_cor = 1;
	for (int w = start_coordinate[n_cor]; w <stop_coordinade[n_cor]; w++)

	{
		minimum_f.clear();
		minimum_s.clear();
		maxim.clear();
		vevlet.clear();
		if (col == false)
			f_poisk_granic_vevlet(&in_image, vevlet, maxim, w, start_col, false,porog);
		else
			f_poisk_granic_vevlet(&in_image, vevlet, maxim, start_row, w, true,porog);
		f_poisk_minimumov(vevlet, maxim, minimum_f, minimum_s);
		if (maxim.size() == 0)
			init_obl = false;
		if (init_obl == false && maxim.size() != 0)
			init_obl = f_init_oblasti(test, &minimum_f, &minimum_s, w, 1, &maxim, col);
		f_poisk_sosedey(test, minimum_f.data(), minimum_s.data(), w, &maxim, col);
		if (col == false)
			f_filtr_oblstei(test, close_oblasti, w, in_image.rows, col);
		else
			f_filtr_oblstei(test, close_oblasti, w, in_image.cols, col);
	}
	f_obed_oblastei(close_oblasti, out_oblasti);
	if (col == false)
	{
		for (int i{ 0 }; i < out_oblasti.size(); i++)
		{
			out_oblasti[i].normalizade_row();
		}
	}
	else
	{
		for (int i{ 0 }; i < out_oblasti.size(); i++)
		{
			out_oblasti[i].normalizade_col();
		}
	}
}
void f_analis(Analis &out_name, cv::Mat const * isod, cv::Mat const *temp, cv::Mat const *out, int frame)
{
	imshow("frane_out", *out);
	imshow("frane_temp", *temp);
	imshow("frane_isxod", *isod);
	bool next{ false };
	cv::setMouseCallback("frane_out", myMouseCallback_2, (void*)out);
	char c = cv::waitKey();
	if (c == ' ')
	{
		next = true;
	}
	cv::destroyWindow("frane_out");
	cv::destroyWindow("frane_temp");
	cv::destroyWindow("frane_isxod");
	std::cout << "chislo celi:";
	int Nc{ 0 };
	std::cin >> Nc;
	int N1{ 0 };
	N1 = countNonZero(*out);
	int Nl;
	Nl = N1 - Nc;

}
void myMouseCallback_2(int event, int x, int y, int flag, void* param)
{
	static int i{ 0 };
	static int first_point[2]{0, 0};
	int second_point[2]{0, 0};
	cv::Mat* img;
	img = (cv::Mat*)param;
	if (i == 0 && event == 1)
	{
		first_point[0] = x;
		first_point[1] = y;
		++i;
		event = 0;
	}
	if (i == 1 && event == 1)
	{
		second_point[0] = x;
		second_point[1] = y;
		i++;
		event = 0;
	}
	if (i == 2)
	{
		cv::Point p1, p2;
		p1.x = first_point[0];
		p1.y = first_point[1];
		p2.x = second_point[0];
		p2.y = second_point[1];
		std::cout<<count_zero(p1, p2, *img)<<std::endl;
		i = 0;
	}
}
int count_zero(cv::Point &p_1,cv::Point &p_2, cv::Mat& out_img)
{
	cv::Rect oblst(p_1, p_2);
	cv::Mat new_img;
	new_img = out_img(oblst);
	return cv::countNonZero(new_img);
}

//void myMouseCallback_1(int event, int x, int y, int flag, void* param)
//{
//	static int i{ 0 };
//	static int first_point[2]{0, 0};
//	int second_point[2]{0, 0};
//	WriteToFile* wr;
//	wr = (WriteToFile*)param;
//	if (i == 0 && event == 1)
//	{
//		first_point[0] = x;
//		first_point[1] = y;
//		++i;
//		event = 0;
//	}
//	if (i == 1 && event == 1)
//	{
//		second_point[0] = x;
//		second_point[1] = y;
//		i++;
//		event = 0;
//	}
//	if (i == 2)
//	{
//		wr->write_true(first_point, second_point);
//		cout << "OK" << endl;
//		i = 0;
//	}
//}