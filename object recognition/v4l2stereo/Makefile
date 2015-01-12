
all:
	g++ -std=c++11 -O3 -o v4l2stereo *.cpp calibration/*.cpp elas/*.cpp -I/usr/include/opencv -L/usr/lib `pkg-config opencv --cflags --libs` -msse3 -Wall -pedantic -fopenmp

gstreamer:
	g++ -std=c++11 -O3 -o v4l2stereo *.cpp calibration/*.cpp elas/*.cpp -I/usr/include/opencv -L/usr/lib `pkg-config --cflags --libs gstreamer-0.10` `pkg-config opencv --cflags --libs` `pkg-config --cflags --libs glib-2.0` `pkg-config --cflags --libs gstreamer-plugins-base-0.10` -msse3 -lgstapp-0.10 -Wall -pedantic -fopenmp

debug:
	g++ -std=c++11 -g -o v4l2stereo *.cpp calibration/*.cpp elas/*.cpp -I/usr/include/opencv -L/usr/lib `pkg-config opencv --cflags --libs` -msse3 -Wall -pedantic -fopenmp

clean:
	rm -f v4l2stereo
