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

#include "vadd.h"

vadd::vadd(sc_module_name name, xsc::common_cpp::properties& _properties) : xtlm_ap_ctrl_none(name) {
    in1 = new xtlm::xtlm_axis_slave_if("in1", in1_socket, 32);
    in2 = new xtlm::xtlm_axis_slave_if("in2", in2_socket, 32);
    out = new xtlm::xtlm_axis_master_if("out", out_socket, 32);

    s_axi_control_rd_socket = new xtlm::xtlm_aximm_target_socket("rd_socket", 32);
    s_axi_control_rd_util   = new xtlm::xtlm_aximm_target_rd_socket_util("rd_util", xtlm::aximm::TRANSACTION, 32);

    s_axi_control_wr_socket = new xtlm::xtlm_aximm_target_socket("wr_socket", 32);
    s_axi_control_wr_util   = new xtlm::xtlm_aximm_target_wr_socket_util("wr_util", xtlm::aximm::TRANSACTION, 32);

    s_axi_control_rd_socket->bind(s_axi_control_rd_util->rd_socket);
    s_axi_control_wr_socket->bind(s_axi_control_wr_util->wr_socket);

    memset(reg_mem, 0, REG_MEM_SIZE);
    kernel_args = (uint32_t*)(&reg_mem);
    m_log = new xsc::common_cpp::report_handler("xtlm_ap_ctrl");

    SC_METHOD(kernel_config_write);
    sensitive << s_axi_control_wr_util->transaction_available;
    dont_initialize();

    SC_METHOD(kernel_status_read);
    sensitive << s_axi_control_rd_util->transaction_available;
    dont_initialize();

    SC_THREAD(main_thread);
}

vadd::~vadd(){
    delete s_axi_control_rd_util;
    delete s_axi_control_wr_util;
    delete s_axi_control_rd_socket;
    delete s_axi_control_wr_socket;
}

void vadd::main_thread() {
    unsigned data1, data2, data_r;
    while(true){
        in1->read((unsigned char *)&data1);
        in2->read((unsigned char *)&data2);
        data_r = data1 + data2 + kernel_args[0];
        out->write((unsigned char *)&data_r); //! Write the result
    }
}

void vadd::kernel_config_write(){
    xtlm::aximm_payload* trans = s_axi_control_wr_util->get_transaction();
    unsigned long long addr = trans->get_address() & 0xffff;
    unsigned int data = *(unsigned int*)trans->get_data_ptr();

    if(addr >= REG_MEM_SIZE) {
        m_ss.str("");
        m_ss << "Out of bound address received: 0x" << std::hex << addr;
        XSC_REPORT_ERROR((*m_log), m_name, m_ss.str().c_str());
        return; //! Throw error
    }

    m_ss.str("");
    m_ss << "Writing via axi-lite to   addr 0x" << std::hex << std::setfill('0') << std::setw(8) << addr 
            << " data 0x" << std::setw(8) << data;
    XSC_REPORT_INFO((*m_log), m_name, m_ss.str().c_str());

    memcpy(&reg_mem[addr], trans->get_data_ptr(), trans->get_data_length());

    trans->set_response_status(xtlm::XTLM_OK_RESPONSE);
    sc_core::sc_time delay = SC_ZERO_TIME;
    s_axi_control_wr_util->send_resp(*trans, delay);
}

void vadd::kernel_status_read(){
    xtlm::aximm_payload* trans = s_axi_control_rd_util->get_transaction();
    unsigned long long addr = trans->get_address() & 0xffff;

    if(addr >= REG_MEM_SIZE) {
        m_ss.str("");
        m_ss << "Out of bound address received: 0x" << std::hex << addr;
        XSC_REPORT_ERROR((*m_log), m_name, m_ss.str().c_str());
        return; //! Throw    error
    }

    m_ss.str("");
    m_ss << "Reading via axi-lite from addr 0x" << std::hex << std::setfill('0') << std::setw(8) << addr 
            << " data 0x" << std::setw(8) << *(unsigned int*)(&reg_mem[addr]);
    XSC_REPORT_INFO_VERB((*m_log), m_name, m_ss.str().c_str(), DEBUG);

    memcpy(trans->get_data_ptr(), &reg_mem[addr], trans->get_data_length());
    trans->set_response_status(xtlm::XTLM_OK_RESPONSE);
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
    s_axi_control_rd_util->send_data(*trans, delay);
}
