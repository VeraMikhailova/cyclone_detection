import cv2
from numpy import*

A=cv2.imread("tmp.jpg",cv2.IMREAD_GRAYSCALE)
f=open("tmp1.txt","w")
f.write(str(A.shape[0]));
f.write('\n')
for i in range(A.shape[0]):
	for j in range(A.shape[1]):
		f.write(str((A[i][j])));
		#f.write(str((A[i][j][0]+A[i][j][1]+A[i][j][2])/3));
		f.write("\n");
