
PLATFORM:=xilinx_u50_gen3x16_xdma_5_202210_1
XCLBIN=build/vadd.xclbin

all: $(XCLBIN) host

vadd.xo: src/vadd.cpp include/vadd.h
	cp src/vadd.cpp vadd_xo_stub/ip_repo/xilinx_ip_vadd_1_0/sysc/
	cp include/vadd.h vadd_xo_stub/ip_repo/xilinx_ip_vadd_1_0/sysc/
	cd vadd_xo_stub; zip -r ../vadd.xo *

idma.xo: src/io.cpp
	v++ -c --platform $(PLATFORM) $< -k idma -o $@

odma.xo: src/io.cpp
	v++ -c --platform $(PLATFORM) $< -k odma -o $@

$(XCLBIN): vadd.xo idma.xo odma.xo
	v++ -l -g --save-temps  -t hw_emu --platform $(PLATFORM) --temp_dir ./build -o $@ --connectivity.sc idma_1.op0:vadd_1.in1 --connectivity.sc idma_1.op1:vadd_1.in2 --connectivity.sc vadd_1.out:odma_1.res vadd.xo odma.xo idma.xo

host: src/host.cpp
	g++ -I/opt/xilinx/xrt//include -L/opt/xilinx/xrt//lib $< -lxrt_core -lxrt_coreutil -o $@

.PHONY=run

run: all
	XCL_EMULATION_MODE=hw_emu ./host $(XCLBIN)
