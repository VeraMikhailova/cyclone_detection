#include <bits/stdc++.h>

using namespace std;
using namespace filesystem;

constexpr double PI = 3.14159265358979323846;

int main() {
  cout << "Input a name of initial image (.jpg) and label (.txt) (after using "
          "the model or after using analytical script). Results will be in "
          "file direction.txt.\n"
       << flush;
  string name_jpg, name_txt;
  cin >> name_jpg >> name_txt;
  if (!exists(name_jpg) || !exists(name_txt)) {
    cout << "These files do not exist!\n" << flush;
    return 0;
  }
  copy_file(name_jpg, "tmp.jpg");

  //яркость пикселя
  double c;
  system("python3 convert_cv.py");
  ifstream ifsimg("tmp1.txt");
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
      if (text[j] != ' ' && text[j] != ',') {
        tmp.push_back(text[j]);
      }
    }
    double X = stoi(tmp);
    getline(ifstxt, text, ',');
    string tmp;
    for (int j = 0; j < text.size(); ++j) {
      if (text[j] != ' ' && text[j] != ',') {
        tmp.push_back(text[j]);
      }
    }
    double Y = stoi(tmp);
    getline(ifstxt, text, ',');
    string tmp;
    for (int j = 0; j < text.size(); ++j) {
      if (text[j] != ' ' && text[j] != ',') {
        tmp.push_back(text[j]);
      }
    }
    double W = stoi(tmp);
    getline(ifstxt, text, ',');
    string tmp;
    for (int j = 0; j < text.size(); ++j) {
      if (text[j] != ' ' && text[j] != ',') {
        tmp.push_back(text[j]);
      }
    }
    double H = stoi(tmp);

    //шаг сектора
    double b = 2 * PI / 100;
    double x, y;
    vector<double> sum(100);
    for (int i = 0; i < IMG_H; ++i) {
      for (int j = 0; j < IMG_H; ++j) {
        int c = img[i][j];
        x = j - X;
        if (i <= Y) {
          y = Y - i;
        } else {
          y = (-1) * (i - Y);
        }

        double a = atan2(y, x);
        if (a < 0) {
          a += 2 * PI;
        }
        sum[(int)(a / b)] += c;
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

    double angle = imax * (b - 1) + b / 2;

    ofstream ofs("direction.txt");
    ofs << to_string(angle);
  }

  return 0;
}
