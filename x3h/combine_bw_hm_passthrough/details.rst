Combine_Bandwidth_Hostmemory(C)
===============================

This example contains verify test, bandwidth test and host memory test kernels to validate FPGA.

In the verify test we have extremely simple HLS C Kernel to verify that the platform has basic functionality. It will make a call to the kernel with an empty global buffer. The kernel will then write the string of characters "Hello World" into the buffer and return. The host will copy this buffer locally and then print out the resulting buffer contents.

In the bandwidth test we try to get the maximum possible bandwidth between Kernel and Global Memory.
Extracting Memory Information and generate cfg file:
   
      - Platforminfo -j (path to xpfm) > platform_info.json
      - From the platform_info.json file we pick the memory info
      - Generate the meta data related to Memory Banks(DDR/HBM/HOST) to platform_bandwidth.cfg file


Using the ``sp`` option  in the platform_bandwidth.cfg file AXI-Master Port is connected to the IP. 

::

   sp=bandwidth_1.input:DDR[0]
   sp=bandwidth_1.output:DDR[0]

hostmemory test is to validate direct host memory access from kernel using slave bridge.

The host allocates a buffer into specific host-only buffer using ``XCL_MEM_EXT_HOST_ONLY``. The ``cl_mem_ext_ptr`` object needs to be used in cases where memory assignment is done by user explicitly:

.. code:: cpp

   cl_mem_ext_ptr_t input_buffer_ext;
   input_buffer_ext.flags = XCL_MEM_EXT_HOST_ONLY;
   input_buffer_ext.obj = nullptr;
   input_buffer_ext.param = 0;
   
   OCL_CHECK(err, input_buffer[i] = new cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_EXT_PTR_XILINX, vector_size_bytes,
                                             &input_buffer_ext, &err));

Using the ``sp`` option  in the platform_hostmemory.cfg file, AXI-Master Port is connected to the Slave-Bridge IP:

::

   sp=hostmemory.input:HOST[0]
   sp=hostmemory.output:HOST[0]

BUILD INSTRUCTIONS
------------------

Instruction below can be used to generate XCLBIN for Hardware (Board) flow.

::

    make all TARGET=hw  DEVICE=<path to xilinx_x3522p_gen4x8_hn10g_M7_1_202110_1.xpfm file> 

Instruction below can be used to generate XCLBIN for Hardware Emulation Flow.

:: 

    make run TARGET=hw_emu DEVICE=<path to xilinx_x3522p_gen4x8_hn10g_1_202220_1.xpfm emulation platform>

Note: In case of emulation flow platform file is present under /proj/xbuilds/Vitis_HW_EMU/2022.2/xilinx_x3522p_gen4x8_hn10g_1_202220_1/2022.2_component_verified.

