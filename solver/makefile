LIB_PATHS=-L /usr/local/Cellar/opencv/3.3.0_3/lib
LIBS=/usr/local/Cellar/opencv/3.3.0_3/lib/libopencv_highgui.3.3.0.dylib /usr/local/Cellar/opencv/3.3.0_3/lib/libopencv_core.3.3.0.dylib 
OPENCV= `pkg-config opencv --cflags --libs`
all: trainer

trainer: trainer.cpp
	g++ -g -O0 -std=c++11 trainer.cpp  ${OPENCV} -o trainer -larmadillo

.PHONY:clean

clean:
	rm trainer
	rm *.o
	rm -r *.dSYM
