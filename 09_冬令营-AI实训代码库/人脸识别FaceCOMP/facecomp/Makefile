export PKG_CONFIG_PATH=/usr/local/AID/pkgconfig:$PKG_CONFIG_PATH

LDDFLAGS= `pkg-config --libs fastcv` `pkg-config --libs gtk+-3.0` `pkg-config --libs tengine`
INCFLAGS= `pkg-config --cflags fastcv` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags tengine` 

all:
	g++ -c lightcnn.cpp mtcnn.cpp mtcnn_utils.cpp face_comp.cpp -lrockchip_vendor ${INCFLAGS}
	g++ lightcnn.o mtcnn.o mtcnn_utils.o face_comp.o ${LDDFLAGS} -o face_comp -lrockchip_vendor
