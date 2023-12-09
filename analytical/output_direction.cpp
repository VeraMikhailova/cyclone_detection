#include <bits/stdc++.h>

using namespace std;
using namespace filesystem;

int main(){
    cout<<"Input a name of file (.jpg) and label (.txt). Results will be in file direction.txt.\n"<<flush;
    string s_img,s_txt;
    cin>>s_img>>s_txt;
    if (!exists(s_img)||!exists(s_txt)){
    	cout<<"These files do not exist!\n"<<flush;
    }	
    ifstream ifsimg (s_img);
    int IMG_H;
    ifsimg>>IMG_H;
    
    
    //яркость пикселя
    double c;
    system ("python3 convert_cv.py");
    vector <vector <double>> img (IMG_H);
    for (int i=0; i<IMG_H;++i){
        for (int j=0;j<IMG_H;++i){
            ifsimg>>c;
            img[i][j]=c;
        }
    }
    ifstream ifstxt (s_txt);
    string text;
    ifstxt>>text;
    char xx,yy;
    xx=text[9];
    yy=text[12];
    int X=xx-'0';
    int Y=yy-'0';
    //шаг сектора
    double b=2*3.1415926/100;
    double x,y;
    vector<double> sum(100);
    for(int i=0;i<IMG_H;++i){
        for (int j=0;j<IMG_H;++j){
            c=img[i][j];
            x=j+1-X;
            if (i+1>Y){
                y=-1*(2*Y-i-1);
            }
            if (i+1<=Y){
                y=Y-i-1;
            }
   
   
        double a = atan2 (y,x);
        if (y<0){
            a+=2*3.1415926;
        }
        sum[(int)(a/b)] += c;
        }  
    }
    double max=0;
    int imax=0;
    for (int i=0;i<100;++i){
        if (sum[i]>max){
            max=sum[i];
            imax=i;
        }
       
    }
   
    double angle=imax*(b-1)+b/2;
    
    ofstream ofs ("direction.txt");
    ofs<<angle;


    return 0;
} 
