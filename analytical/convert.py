from PIL import Image
from numpy import*

temp=Image.open("tmp.jpg")
temp=temp.convert("1")
A = array(temp)
f=open("tmp0.txt","w")
f.write(str(A.shape[0]));
f.write('\n')
for i in range(A.shape[0]):
	for j in range(A.shape[1]):
		if A[i][j]==True:
			f.write("1\n");
		else:
			f.write("0\n");
