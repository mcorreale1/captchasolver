LIB_PATHS=-L /usr/local/Cellar/opencv/3.3.0_3/lib
LIBS=/usr/local/Cellar/opencv/3.3.0_3/lib/libopencv_highgui.3.3.0.dylib /usr/local/Cellar/opencv/3.3.0_3/lib/libopencv_core.3.3.0.dylib 
OPENCV= `pkg-config opencv --cflags --libs`
all: mcorrea4_a5

mcorrea4_a5: mcorrea4_a5.cpp
	g++ -g -O0 -std=c++11 mcorrea4_a5.cpp  ${OPENCV} -o mcorrea4_a5

.PHONY:clean

clean:
	rm mcorrea4_a5
	rm *.o
	rm -r *.dSYM
