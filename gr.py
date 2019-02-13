import numpy as np

def rgb2y(r,g,b):
	return 0.2125*r + 0.7154*g + 0.0721*b
lis = []
for i in range(256):
	r = ((i&0xe0)>>5)/7.0
	g = ((i&0x1c)>>2)/7.0
	b = (i&0x3)/3.0
	lis.append(255-int(rgb2y(r,g,b)*255))

at =0
string = "const uint8_t grayscale["+str(len(lis))+"] = {"
for i in range(len(lis)):
	string += str(lis[i])
	if i != len(lis)-1 and at != 30:
		string += ", "
	elif at == 30:
		string += ",\n\t\t\t\t\t\t"
		at = 0
	at += 1
string += "};"
print(string)