g++ `pkg-config --libs --cflags gtkmm-2.4` sample.cpp  -shared -o libsample.so
g++ `pkg-config --libs --cflags gtkmm-2.4 ` main.cpp
