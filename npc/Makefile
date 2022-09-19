BUILD_DIR = ./build

test:
	./mill -i __.test

verilog:
	$(call git_commit, "generate verilog")
	mkdir -p $(BUILD_DIR)
	./mill -i __.test.runMain topgen -td $(BUILD_DIR)

help:
	./mill -i __.test.runMain Elaborate --help

compile:
	./mill -i __.compile

bsp:
	./mill -i mill.bsp.BSP/install

reformat:
	./mill -i __.reformat

checkformat:
	./mill -i __.checkFormat

clean:
	-rm -rf $(BUILD_DIR)

.PHONY: test verilog help compile bsp reformat checkformat clean

sim:	verilog
	# verilator -Wall --cc --exe --build csrc/sim_main.cpp vsrc/top.v --trace
	verilator -Wall -Wno-DECLFILENAME -Wno-EOFNEWLINE -Wno-UNUSED --top-module riscv --cc --exe --build  csrc/riscvtest.cpp csrc/pmem.cpp $(BUILD_DIR)/riscv.v  $(BUILD_DIR)/ebProbe.v 
	obj_dir/Vriscv
	# gtkwave wave.vcd
	$(call git_commit, "sim RTL") # DO NOT REMOVE THIS LINE!!!
	@echo "Write this Makefile by yourself."

test:
	verilator -Wno-WIDTH --top-module riscv --cc --exe --build  csrc/riscvtest.cpp csrc/pmem.cpp  csrc/difftest.cpp vsrc/riscv.v  playground/src/main/vsrc/*  -LDFLAGS -ldl

include ../Makefile
