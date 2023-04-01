#pragma once

#include "xtlm.h"
#include "xtlm_ap_ctrl.h"

class vadd : public xsc::xtlm_ap_ctrl_none {

    enum {
        REG_MEM_SIZE = 8192
    };

    public:
        SC_HAS_PROCESS(vadd);
        vadd(sc_module_name name, xsc::common_cpp::properties& _properties);
        ~vadd();

        //! Declare aximm interfaces..
        xtlm::xtlm_aximm_target_socket* s_axi_control_rd_socket;
        xtlm::xtlm_aximm_target_socket* s_axi_control_wr_socket;

        xtlm::xtlm_axis_slave_if* in1;
        xtlm::xtlm_axis_target_socket* in1_socket;
        xtlm::xtlm_axis_slave_if* in2;
        xtlm::xtlm_axis_target_socket* in2_socket;
        xtlm::xtlm_axis_master_if* out;
        xtlm::xtlm_axis_initiator_socket* out_socket;

        void main_thread();

    protected:
        void kernel_config_write();
        void kernel_status_read();

        //axilite memory and args overlapped onto it
        unsigned char reg_mem[REG_MEM_SIZE];
        uint32_t*     kernel_args;

        xtlm::xtlm_aximm_target_wr_socket_util*  s_axi_control_wr_util;
        xtlm::xtlm_aximm_target_rd_socket_util*  s_axi_control_rd_util;
};
