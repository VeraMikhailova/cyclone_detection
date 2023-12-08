#include <bits/stdc++.h>

using namespace std;

template<class A,class B> istream& operator>>(istream&i,pair<A,B>&v){
	return i>>v.first>>v.second;
}
template<class T> istream& operator>>(istream&i,vector<T>&v){
	for(auto&val:v){
		i>>val;
	}
	return i;
}
template<class A, class B> bool rmax(A& a, const B& b){
	A t=b;
	if(a<t){
		a=move(t);
		return 1;
	}
	return 0;
}
template<class A, class B> bool rmin(A& a, const B& b){
	A t=b;
	if(a>t){
		a=move(t);
		return 1;
	}
	return 0;
}

vector<vector<int>> cmps;
vector<pair<pair<int,int>,pair<int,int>>> rects;
vector<pair<pair<int,int>,pair<int,int>>> rects2;
vector<string> ucls;
vector<double> COLr({1,0,0,1,1,1,0,0.4});
vector<double> COLg({0,1,0,1,1,0,1,0.2});
vector<double> COLb({0,0,1,1,0,1,1,0.7});
int COLS=8;
vector<pair<int,int>> pts;
vector<pair<int,int>> id;
void draw_around(map<pair<int,int>,int>&m,pair<int,int> p,int color){
	for(int dx=0;dx<2;++dx){
		for(int dy=0;dy<2;++dy){
			m[{p.first+dx,p.second+dy}]=color;
			m[{p.first-dx,p.second+dy}]=color;
			m[{p.first+dx,p.second-dy}]=color;
			m[{p.first-dx,p.second-dy}]=color;
		}
	}
}
void display(const vector<vector<double>>&v){
	{
		ofstream cout("out.ppm");
		cout<<"P6 "<<v.back().size()<<' '<<v.size()<<" 255"<<endl;
		map<pair<int,int>,int> cents;
		map<pair<int,int>,int> marks;
		for(int i=0;i<pts.size();++i){
			cents[pts[i]]=i;
		}
		for(auto r:rects2){
			auto p=r.first;
			draw_around(marks,p,2);
		}
		for(auto r:rects){
			auto p=r.first;
			draw_around(marks,p,0);
		}
		for(auto p:id){
			draw_around(marks,p,1);
		}
		for(int second=0;second<v.size();++second){
			for(int first=0;first<v[second].size();++first){
				char col=v[second][first]*255;
				char ma=255;
				char z=0;

				bool inrect=0;
				for(auto r:rects){
					bool nearx=abs(abs(r.first.first-first)-r.second.first/2)<2;
					bool neary=abs(abs(r.first.second-second)-r.second.second/2)<2;
					bool near=1;
					near&=(first<=r.first.first+r.second.first/2);
					near&=(second<=r.first.second+r.second.second/2);
					near&=(first>=r.first.first-r.second.first/2);
					near&=(second>=r.first.second-r.second.second/2);
					inrect|=(neary||nearx)&&near;
				}

				int i=-2;
				if(marks.count({first,second})){
					i=marks[{first,second}];
				}else{
					if(cents.count({first,second})){
						i=cents[{first,second}];
					}
				}
				if(i!=-2){
					char r=COLr[i%COLS]*255;
					char g=COLg[i%COLS]*255;
					char b=COLb[i%COLS]*255;
					cout<<r<<g<<b;
					continue;
				}
				if(inrect){
					cout<<z<<ma<<z;
				}else{
					if(!cmps[second][first]){
						char col=v[second][first]*255;
						cout<<col<<col<<col;
						continue;
					}
					char r=COLr[cmps[second][first]%COLS]*v[second][first]*255;
					char g=COLg[cmps[second][first]%COLS]*v[second][first]*255;
					char b=COLb[cmps[second][first]%COLS]*v[second][first]*255;
					cout<<r<<g<<b;
				}
			}
		}
	}
	system("lximage-qt out.ppm");
}
void apply(vector<vector<double>>& v,const vector<vector<double>>&m,int&offx,int&offy){
	vector<vector<double>> nv(v.size()-m.size()+1,vector<double>(v.back().size()-m.back().size()+1,0));
	offx+=m.back().size()-1;
	offy+=m.size()-1;
	for(int i=0;i<nv.size();++i){
		for(int j=0;j<nv[i].size();++j){
			for(int second=0;second<m.size();++second){
				for(int first=0;first<m[second].size();++first){
					nv[i][j]+=v[i+second][j+first]*m[second][first];
				}
			}
		}
	}
	v=nv;
}
void norm(vector<vector<double>>& v){
	double mn,mx;
	mn=mx=v[0][0];
	for(auto& first:v){
		for(auto& second:first){
			rmin(mn,second);
			rmax(mx,second);
		}
	}
	for(auto& first:v){
		for(auto& second:first){
			second=(second-mn)/(mx-mn);
		}
	}
}
void dfs(const vector<vector<double>>&v,pair<int,int> p,int c){
	if(p.second>=cmps.size()||p.second<0){
		return;
	}
	if(p.first>=cmps.back().size()||p.first<0){
		return;
	}
	if(cmps[p.second][p.first]+1){
		return;
	}
	if(v[p.second][p.first]<0.5){
		cmps[p.second][p.first]=0;
		return;
	}
	cmps[p.second][p.first]=c;
	for(int dx=-1;dx<=1;++dx){
		for(int dy=-1;dy<=1;++dy){
			dfs(v,{p.first+dx,p.second+dy},c);
		}
	}
};
void comps(const vector<vector<double>>&v){
	cmps.resize(v.size(),vector<int>(v[0].size(),-1));
	int c=0;
	for(int second=0;second<v.size();++second){
		for(int first=0;first<v[second].size();++first){
			if(v[second][first]<0.5){
				cmps[second][first]=0;
			}
			if(cmps[second][first]==-1){
				dfs(v,{first,second},++c);
			}
		}
	}
}
void centers(const vector<vector<double>>&v){
	vector<double> xs;
	vector<double> ys;
	vector<double> ss;
	for(int second=0;second<v.size();++second){
		for(int first=0;first<v[second].size();++first){
			int c=cmps[second][first];
			if(pts.size()<=c){
				pts.resize(c+1);
				xs.resize(c+1);
				ys.resize(c+1);
				ss.resize(c+1);
			}
			double w=v[second][first];
			ss[c]+=w;
			xs[c]+=w*first;
			ys[c]+=w*second;
		}
	}
	for(int i=0;i<pts.size();++i){
		if(ss[i]>1e-6){
			pts[i]={xs[i]/ss[i],ys[i]/ss[i]};
		}
	}
}
int get_cmp(int first,int second){
	if(second<0){
		return -1;
	}
	if(first<0){
		return -1;
	}
	if(second>=cmps.size()){
		return -1;
	}
	if(first>=cmps.back().size()){
		return -1;
	}
	return cmps[second][first];
}
pair<int,int> find_center(vector<vector<double>> v,pair<pair<int,int>,pair<int,int>> rect){
	int K=360;
	double first=rect.first.first;
	double second=rect.first.second;
	double PI=3.1415926565;
	double phi=2*PI/K;
	vector<pair<int,int>> ps;
	vector<int> cs;
	map<int,int> sts;
	for(int i=0;i<K;++i){
		double psi=phi*i;
		double dx=cos(psi);
		double dy=sin(psi);
		double xx=first;
		double yy=second;
		while(get_cmp(xx,yy)==0){
			xx+=dx;
			yy+=dy;
		}
		int c=get_cmp(xx,yy);
		if(c!=-1){
			cs.push_back(c);
			ps.push_back({(int)xx,(int)yy});
			++sts[c];
		}
	}
	int ma=0;
	int cma=-1;
	for(auto p:sts){
		if(rmax(ma,p.second)){
			cma=p.first;
		}
	}
	vector<pair<int,int>> vp;
	for(int i=0;i<ps.size();++i){
		if(cs[i]==cma){
			vp.push_back(ps[i]);
		}
	}
	if(vp.empty()){
		return rect.first;
	}
	{
		double s=0;
		double first=0;
		double second=0;
		for(auto p:vp){
			double w=v[p.second][p.first];
			s+=w;
			first+=w*p.first;
			second+=w*p.second;
		}
		first/=s;
		second/=s;
		return {first,second};
	}
}
void abs(vector<vector<double>>&v){
	for(auto&first:v){
		for(auto&second:first){
			second=abs(second);
		}
	}
}
void discr(vector<vector<double>>&v,double p=0.5){
	for(auto&first:v){
		for(auto&second:first){
			if(second>=p){
				second=1;
			}else{
				second=0;
			}
		}
	}
}
void sides(vector<vector<double>>& v,int&offx,int&offy){
	apply(v,{
			vector<double>{-1,-1,-1},
			vector<double>{-1,8,-1},
			vector<double>{-1,-1,-1},
			},offx,offy);
}
vector<vector<double>> read_img(const string&file){
	vector<vector<double>> v;
	ifstream in(file);
	int first;
	in>>first;
	vector<double> vv(first);
	while(in>>vv){
		v.push_back(vv);
	}
	norm(v);
	return v;
}
void read_ans(const string&file){
	ifstream in(file);
	pair<int,int> p;
	while(in>>p){
		id.push_back(p);
	}
}
void read_lbl(const string&file){
	string cls,first,second,w,h;
	ifstream in(file);
	while(in>>cls>>first>>second>>w>>h){
		first.pop_back();
		second.pop_back();
		id.push_back({stoi(first),stoi(second)});
	}
}
void read_rects(const string&file){
	string cls,first,second,w,h;
	ifstream in(file);
	while(in>>cls>>first>>second>>w>>h){
		first.pop_back();
		second.pop_back();
		w.pop_back();
		rects.push_back({pair<int,int>{stoi(first),stoi(second)},pair<int,int>{stoi(w),stoi(h)}});
		ucls.push_back(cls);
	}
}
bool is_dupe(string a,string b){
	ifstream fa(a);
	ifstream fb(b);
	while(1){
		if((!!fa)!=(!!fb)){
			return 0;
		}
		if(!fa){
			return 1;
		}
		char c1=fa.get();
		char c2=fb.get();
		if(c1!=c2){
			return 0;
		}
	}
}
vector<vector<double>> cut_off(const vector<vector<double>>&v,int offx,int offy){
	vector<vector<double>> _(v.size()-offy,vector<double>(v.back().size()-offx));
	for(int second=0;second<_.size();++second){
		for(int first=0;first<_[second].size();++first){
			_[second][first]=v[second+offy/2][first+offx/2];
		}
	}
	return _;
}

using namespace filesystem;

int main(){
	cout<<"Input a name of file for analytical solution (enter A and file A.jpg, A.txt and A.lbl/A.ans will be analysed): "<<flush;
	string name;
	cin>>name;
	cout<<"Select the mode (0 - binary processing, 1 - grayscale): "<<flush;
	int grayscale;
	cin>>grayscale;
	{
		string name_jpg=name+".jpg";
		string name_txt=name+".txt";
		string name_ans=name+".ans";
		string name_lbl=name+".lbl";
		cout<<"Loading image:\n"<<flush;
		if(!exists(name_jpg)){
			cout<<"No such image!\n"<<flush;
			return 0;
		}
		if(!exists(name_txt)){
			cout<<"No such label!\n"<<flush;
			return 0;
		}
		read_rects(name_txt);
		if(!exists(name_lbl)){
			cout<<"No answers in label format.\n"<<flush;
			if(!exists(name_ans)){
				cout<<"No answers.\n"<<flush;
			}else{
				read_ans(name_ans);
				cout<<"Answers read.\n"<<flush;
			}
		}else{
			read_lbl(name_lbl);
			cout<<"Answers read.\n"<<flush;
		}
		if(exists("tmp.jpg")){
			if(is_dupe("tmp.jpg",name_jpg)){
				cout<<"Shortcut used.\n"<<flush;
				goto rd;
			}
			remove("tmp.jpg");
		}
		copy(name_jpg,"tmp.jpg");
		system("python3 convert_cv.py");
		system("python3 convert.py");
		cout<<"Image converted.\n"<<flush;
	}
	rd:;
	vector<vector<double>> v;
	if(grayscale){
		v=read_img("tmp1.txt");
	}else{
		v=read_img("tmp0.txt");
	}
	vector<vector<double>> img=v;
	cout<<"Processing image:\n"<<flush;
	int offx=0;
	int offy=0;
	cout<<"Mean representation..."<<flush;
	for(int i=0;i<4;++i){
		apply(v,{
				vector<double>{1,1,1},
				vector<double>{1,1,1},
				vector<double>{1,1,1},
				},offx,offy);
		apply(v,{
				vector<double>{1,1,1,1,1},
				vector<double>{1,2,2,2,1},
				vector<double>{1,2,3,2,1},
				vector<double>{1,2,2,2,1},
				vector<double>{1,1,1,1,1},
				},offx,offy);
	}
	norm(v);
	discr(v);
	cout<<" done.\n"<<flush;
	cout<<"Building edges..."<<flush;
	sides(v,offx,offy);
	abs(v);
	norm(v);
	discr(v,0.35);
	apply(v,{
			vector<double>{0.3,0.3,0.3},
			vector<double>{0.3,1,0.3},
			vector<double>{0.3,0.3,0.3},
			},offx,offy);
	norm(v);
	discr(v,0.28);
	cout<<" done.\n"<<flush;
	cout<<"Components processing..."<<flush;
	comps(v);
	centers(v);
	cout<<" done.\n"<<flush;
	cout<<"Applying offset..."<<flush;
	for(auto&r:rects){
		r.first.first-=offx/2;
		r.first.second-=offy/2;
	}
	for(auto&p:id){
		p.first-=offx/2;
		p.second-=offy/2;
	}
	img=cut_off(img,offx,offy);
	cout<<" done.\n"<<flush;
	cout<<"Center finding..."<<flush;
	for(auto r:rects){
		pair<int,int> c=find_center(v,r);
		rects2.push_back({c,r.second});
	}
	cout<<" done.\n"<<flush;
	for(int i=0;i<v.size();++i){
		for(int j=0;j<v[i].size();++j){
			v[i][j]+=img[i][j];
			v[i][j]/=2;
		}
	}
	display(v);
	ofstream out("out.txt");
	for(int i=0;i<rects2.size();++i){
		out<<(ucls.size()?ucls[i]:"cyclone,")<<' '<<rects2[i].first.first<<", "<<rects2[i].first.second<<", "<<rects2[i].second.first<<", "<<rects2[i].second.second<<"\n";
	}
	cout<<"Output is written to out.txt and out.ppm.\n"<<flush;
	return 0;
}
