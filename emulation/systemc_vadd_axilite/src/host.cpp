/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

#include <iostream>
#include <xrt/xrt_device.h>
#include <xrt/xrt_kernel.h>
#include <experimental/xrt_ip.h>
#include <xrt/xrt_bo.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <XCLBIN File>" << std::endl;
        return EXIT_FAILURE;
    }

    std::string binaryFile = argv[1];

    auto dev = xrt::device(0);
    auto xclbin_uuid = dev.load_xclbin(binaryFile);
    auto idma_krnl = xrt::kernel(dev, xclbin_uuid, "idma:{idma_1}");
    auto odma_krnl = xrt::kernel(dev, xclbin_uuid, "odma:{odma_1}");
    auto vadd = xrt::ip(dev, xclbin_uuid, "vadd:{vadd_1}");

    std::cout << "Current bias is " << vadd.read_register(0) << std::endl;
    unsigned int bias = 1;
    vadd.write_register(0, bias);
    std::cout << "Current bias is " << vadd.read_register(0) << std::endl;

    auto buf = xrt::bo(dev, 3*4, idma_krnl.group_id(0));
    
    unsigned int ubuf[3];
    ubuf[0] = 42;
    ubuf[1] = 24;
    ubuf[2] = 0;
    buf.write(ubuf);
    buf.sync(XCL_BO_SYNC_BO_TO_DEVICE);

    //execute
    auto run1 = odma_krnl(buf);
    auto run0 = idma_krnl(buf);
    run0.wait();
    run1.wait();

    //check output
    buf.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    buf.read(ubuf);

    if(ubuf[2] != (ubuf[0] + ubuf[1] + bias)){
        std::cout << "Test failed, got " << ubuf[2] << std::endl;
    } else {
        std::cout << "Test passed" << std::endl;
    }

    std::cout << "Current bias is " << vadd.read_register(0) << std::endl;
    bias = 42;
    vadd.write_register(0, bias);
    std::cout << "Current bias is " << vadd.read_register(0) << std::endl;

    //execute again
    auto run3 = odma_krnl(buf);
    auto run2 = idma_krnl(buf);
    run3.wait();
    run2.wait();

    //check output again
    buf.sync(XCL_BO_SYNC_BO_FROM_DEVICE);
    buf.read(ubuf);
    if(ubuf[2] != (ubuf[0] + ubuf[1] + bias)){
        std::cout << "Test failed, got " << ubuf[2] << std::endl;
    } else {
        std::cout << "Test passed" << std::endl;
    }

    std::cout << "Done testing!" << std::endl;
    return 0;
}
