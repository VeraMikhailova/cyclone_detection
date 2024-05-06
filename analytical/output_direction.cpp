#include <bits/stdc++.h>

using namespace std;
using namespace filesystem;

constexpr double PI = 3.14159265358979323846;

int main() {
	cout << "Input a name of initial image (.jpg) and label (.txt) (after using "
		"the model or after using analytical script). Results will be in "
		"file direction.txt.\nJPG: "
		<< flush;
	string name_jpg, name_txt;
	cin >> name_jpg;
	cout << "TXT: " << flush;
	cin >> name_txt;
	if (!exists(name_jpg) || !exists(name_txt)) {
		cout << "These files do not exist!\n" << flush;
		return 0;
	}
	if(exists("tmp.jpg")){
		remove("tmp.jpg");
	}
	copy(name_jpg, "tmp.jpg");

	//яркость пикселя
	double c;

	system("python3 convert_cv.py");
	ifstream ifsimg("tmp1.txt");
	ofstream ofs("direction.txt");
	int IMG_H;
	ifsimg >> IMG_H;
	vector<vector<double>> img(IMG_H);
	for (int i = 0; i < IMG_H; ++i) {
		for (int j = 0; j < IMG_H; ++j) {
			ifsimg >> c;
			img[i].push_back(c);
		}
	}
	ifstream ifstxt(name_txt);
	string text;
	while (getline(ifstxt, text, ',')) {

		getline(ifstxt, text, ',');
		string tmp;
		for (int j = 0; j < text.size(); ++j) {
			if (text[j] != ' ') {
				tmp.push_back(text[j]);
			}
		}
		int X = stoi(tmp);
		getline(ifstxt, text, ',');
		tmp="";
		for (int j = 0; j < text.size(); ++j) {
			if (text[j] != ' ') {
				tmp.push_back(text[j]);
			}
		}
		int Y = stoi(tmp);
		getline(ifstxt, text, ',');
		tmp="";
		for (int j = 0; j < text.size(); ++j) {
			if (text[j] != ' ') {
				tmp.push_back(text[j]);
			}
		}
		double W = stoi(tmp);
		getline(ifstxt, text);
		tmp="";
		for (int j = 0; j < text.size(); ++j) {
			if (text[j] != ' ') {
				tmp.push_back(text[j]);
			}
		}
		double H = stoi(tmp);
		//cout<<X<<","<<Y<<endl;
		//cout<<W<<"x"<<H<<endl;

		double r = W * W + H * H;
		r/=4;

		//шаг сектора
		double b = 2 * PI / 100;
		double x, y;
		vector<double> sum(100);
		for (int i = 0; i < IMG_H; ++i) {
			for (int j = 0; j < IMG_H; ++j) {
				if(j == X && i == Y){
					continue;
				}
				x = j - X;
				y = i - Y;
				if(x * x + y * y > r){
					continue;
				}
				double a = atan2(y, x);
				if (a < 0) {
					a += 2 * PI;
				}
				sum[(int)(a / b)] += img[i][j];
			}
		}
		double max_sector = 0;
		int imax = 0;
		for (int i = 0; i < 100; ++i) {
			if (sum[i] > max_sector) {
				max_sector = sum[i];
				imax = i;
			}
		}

		double angle = (imax - 1) * b + b / 2;

		ofs << to_string(angle) << '\n';
	}

	return 0;
}
