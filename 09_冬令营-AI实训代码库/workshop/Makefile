export PKG_CONFIG_PATH=/usr/local/AID/pkgconfig:$PKG_CONFIG_PATH

LDDFLAGS= -lrtspclient -lrockchip_rga -lrockchip_mpp -ldrm -lcam_engine_cifisp -lcam_ia -lpthread  `pkg-config --libs fastcv` `pkg-config --libs gtk+-3.0` 
INCFLAGS= `pkg-config --cflags fastcv` `pkg-config --cflags gtk+-3.0`
all: 

allall: demo1 demo2 demo3 demo4

demo1: 01_cap_disp.cpp
	g++ -c 01_cap_disp.cpp ${INCFLAGS}
	g++ 01_cap_disp.o ${LDDFLAGS} -o $@
demo2: 02_cap_disp_time.cpp
	g++ -c 02_cap_disp_time.cpp ${INCFLAGS}
	g++ 02_cap_disp_time.o ${LDDFLAGS} -o $@
demo3: 03_cap_disp_pthread.cpp
	g++ -c 03_cap_disp_pthread.cpp ${INCFLAGS}
	g++ 03_cap_disp_pthread.o ${LDDFLAGS} -o $@
demo4: 04_cap_enc.cpp
	g++ -c 04_cap_enc.cpp ${INCFLAGS}
	g++ 04_cap_enc.o ${LDDFLAGS} -o $@
demo5: 05_cap_resize_enc.cpp
	g++ -c 05_cap_resize_enc.cpp ${INCFLAGS}
	g++ 05_cap_resize_enc.o ${LDDFLAGS} -o $@
demo6: 06_cap_enc_dec_disp.cpp
	g++ -c 06_cap_enc_dec_disp.cpp ${INCFLAGS}
	g++ 06_cap_enc_dec_disp.o ${LDDFLAGS} -o $@

clean:
	rm -f *.o
	rm -f demo*