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

#include "xtlm.h"
#include "xtlm_ap_ctrl.h"

class vadd : public xsc::xtlm_ap_ctrl_none {
   public:
    SC_HAS_PROCESS(vadd);
    vadd(sc_module_name name, xsc::common_cpp::properties& _properties) : xsc::xtlm_ap_ctrl_none(name) {
        DEFINE_XTLM_AXIMM_LITE_SLAVE_IF(s_axi_control, 32);
        DEFINE_XTLM_AXIS_SLAVE_IF(in1, 32);
        DEFINE_XTLM_AXIS_SLAVE_IF(in2, 32);
        DEFINE_XTLM_AXIS_MASTER_IF(out, 32);

        ADD_STREAM_IF_ARG(in1, 0x14, 0x4);
        ADD_STREAM_IF_ARG(in2, 0x18, 0x4);
        ADD_STREAM_IF_ARG(out, 0x1C, 0x4);

        SC_THREAD(main_thread);
    }

    //! Declare aximm interfaces..
    DECLARE_XTLM_AXIMM_LITE_SLAVE_IF(s_axi_control);
    DECLARE_XTLM_AXIS_SLAVE_IF(in1);
    DECLARE_XTLM_AXIS_SLAVE_IF(in2);
    DECLARE_XTLM_AXIS_MASTER_IF(out);

    void main_thread() {
        unsigned data1, data2, data_r;
        in1->read((unsigned char *)&data1);
        in2->read((unsigned char *)&data2);
        data_r = data1 + data2;
        out->write((unsigned char *)&data_r); //! Write the result
    }
};
