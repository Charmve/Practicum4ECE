export PKG_CONFIG_PATH=/usr/local/AID/pkgconfig:$PKG_CONFIG_PATH

LDDFLAGS= -lrtspclient -lrockchip_rga -lrockchip_mpp -ldrm -lcam_engine_cifisp -lcam_ia -lpthread  `pkg-config --libs fastcv` `pkg-config --libs gtk+-3.0` 
INCFLAGS= `pkg-config --cflags fastcv` `pkg-config --cflags gtk+-3.0` -I/usr/include/rockchip/ -I./
all:
	g++ -Wall -c rockchip_mpp.cpp rockchip_rga.cpp v4l2Cam.cpp enc-demo.cpp ${INCFLAGS}
	g++ rockchip_mpp.o rockchip_rga.o v4l2Cam.o enc-demo.o ${LDDFLAGS} -o enc-demo
