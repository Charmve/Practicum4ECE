export PKG_CONFIG_PATH=/usr/local/AID/pkgconfig:$PKG_CONFIG_PATH

LDDFLAGS= -lrtspclient -lrockchip_rga -lrockchip_mpp -ldrm -lcam_engine_cifisp -lcam_ia -lpthread  `pkg-config --libs fastcv` `pkg-config --libs gtk+-3.0` `pkg-config --libs tengine` -lrockchip_vendor
INCFLAGS= `pkg-config --cflags fastcv` `pkg-config --cflags gtk+-3.0` `pkg-config --cflags tengine`
all:
	g++ -c mipi_cam.cpp mtcnn.cpp mtcnn_utils.cpp mipi-face.cpp ${INCFLAGS}
	g++ mipi_cam.o mtcnn.o mtcnn_utils.o mipi-face.o ${LDDFLAGS} -o mipi-face
