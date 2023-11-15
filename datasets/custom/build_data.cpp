#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace filesystem;

constexpr double IMG_W = 2101;
constexpr double IMG_H = 2101;

int main() {
	cout << "Dataset type (train|train12): " << flush;
	string type;
	cin >> type;
	path labels = absolute(path(type) / "labels");
	path labels_hr = absolute(path(type) / "labels_hr");
	if (exists(labels)) {
		remove_all(labels);
	}
	copy(labels_hr, labels);
	vector<path> all;
	for (auto e : directory_iterator(labels)) {
		all.push_back(e.path());
	}
	for (auto p : all) {
		{
			string tmp;
			{
				string cls, x, y, w, h;
				ifstream ifs(p);
				while (ifs >> cls >> x >> y >> w >> h) {
					x.pop_back();
					y.pop_back();
					w.pop_back();
					tmp.push_back(cls[6] - 1);
					tmp.push_back(' ');
					tmp += to_string(stoi(x) / IMG_W);
					tmp.push_back(' ');
					tmp += to_string(stoi(y) / IMG_H);
					tmp.push_back(' ');
					tmp += to_string(stoi(w) / IMG_W);
					tmp.push_back(' ');
					tmp += to_string(stoi(h) / IMG_H);
					tmp.push_back('\n');
				}
			}
			ofstream ofs(p);
			ofs << tmp;
		}
		string _ = absolute(p);
		_.pop_back();
		_.pop_back();
		_.pop_back();
		_.pop_back();
		_ += "_pro.txt";
		copy(p, _);
		remove(p);
	}
}
