main.exe: csvexam.obj
	cl /Zi /Od /EHsc /Fe:build/main.exe main.cpp build/csvexam.obj

csvexam.obj:
	cl /Zi /c /Od /EHsc /Fo:build/csvexam.obj csvexam.cpp
