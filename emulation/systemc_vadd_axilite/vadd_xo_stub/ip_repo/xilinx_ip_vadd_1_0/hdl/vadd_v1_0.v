
`timescale 1 ns / 1 ps

	module vadd_v1_0 #
	(
		// Users to add parameters here

		// User parameters ends
		// Do not modify the parameters beyond this line


		// Parameters of Axi Slave Bus Interface s_axi_control
		parameter integer C_s_axi_control_DATA_WIDTH	= 32,
		parameter integer C_s_axi_control_ADDR_WIDTH	= 4,

		// Parameters of Axi Master Bus Interface out
		parameter integer C_out_TDATA_WIDTH	= 32,
		parameter integer C_out_START_COUNT	= 32,

		// Parameters of Axi Slave Bus Interface in1
		parameter integer C_in1_TDATA_WIDTH	= 32,

		// Parameters of Axi Slave Bus Interface in2
		parameter integer C_in2_TDATA_WIDTH	= 32
	)
	(
		// Users to add ports here

		// User ports ends
		// Do not modify the ports beyond this line


		// Ports of Axi Slave Bus Interface s_axi_control
		input wire  s_axi_control_aclk,
		input wire  s_axi_control_aresetn,
		input wire [C_s_axi_control_ADDR_WIDTH-1 : 0] s_axi_control_awaddr,
		input wire [2 : 0] s_axi_control_awprot,
		input wire  s_axi_control_awvalid,
		output wire  s_axi_control_awready,
		input wire [C_s_axi_control_DATA_WIDTH-1 : 0] s_axi_control_wdata,
		input wire [(C_s_axi_control_DATA_WIDTH/8)-1 : 0] s_axi_control_wstrb,
		input wire  s_axi_control_wvalid,
		output wire  s_axi_control_wready,
		output wire [1 : 0] s_axi_control_bresp,
		output wire  s_axi_control_bvalid,
		input wire  s_axi_control_bready,
		input wire [C_s_axi_control_ADDR_WIDTH-1 : 0] s_axi_control_araddr,
		input wire [2 : 0] s_axi_control_arprot,
		input wire  s_axi_control_arvalid,
		output wire  s_axi_control_arready,
		output wire [C_s_axi_control_DATA_WIDTH-1 : 0] s_axi_control_rdata,
		output wire [1 : 0] s_axi_control_rresp,
		output wire  s_axi_control_rvalid,
		input wire  s_axi_control_rready,

		// Ports of Axi Master Bus Interface out
		input wire  out_aclk,
		input wire  out_aresetn,
		output wire  out_tvalid,
		output wire [C_out_TDATA_WIDTH-1 : 0] out_tdata,
		output wire [(C_out_TDATA_WIDTH/8)-1 : 0] out_tstrb,
		output wire  out_tlast,
		input wire  out_tready,

		// Ports of Axi Slave Bus Interface in1
		input wire  in1_aclk,
		input wire  in1_aresetn,
		output wire  in1_tready,
		input wire [C_in1_TDATA_WIDTH-1 : 0] in1_tdata,
		input wire [(C_in1_TDATA_WIDTH/8)-1 : 0] in1_tstrb,
		input wire  in1_tlast,
		input wire  in1_tvalid,

		// Ports of Axi Slave Bus Interface in2
		input wire  in2_aclk,
		input wire  in2_aresetn,
		output wire  in2_tready,
		input wire [C_in2_TDATA_WIDTH-1 : 0] in2_tdata,
		input wire [(C_in2_TDATA_WIDTH/8)-1 : 0] in2_tstrb,
		input wire  in2_tlast,
		input wire  in2_tvalid
	);
// Instantiation of Axi Bus Interface s_axi_control
	vadd_v1_0_s_axi_control # ( 
		.C_S_AXI_DATA_WIDTH(C_s_axi_control_DATA_WIDTH),
		.C_S_AXI_ADDR_WIDTH(C_s_axi_control_ADDR_WIDTH)
	) vadd_v1_0_s_axi_control_inst (
		.S_AXI_ACLK(s_axi_control_aclk),
		.S_AXI_ARESETN(s_axi_control_aresetn),
		.S_AXI_AWADDR(s_axi_control_awaddr),
		.S_AXI_AWPROT(s_axi_control_awprot),
		.S_AXI_AWVALID(s_axi_control_awvalid),
		.S_AXI_AWREADY(s_axi_control_awready),
		.S_AXI_WDATA(s_axi_control_wdata),
		.S_AXI_WSTRB(s_axi_control_wstrb),
		.S_AXI_WVALID(s_axi_control_wvalid),
		.S_AXI_WREADY(s_axi_control_wready),
		.S_AXI_BRESP(s_axi_control_bresp),
		.S_AXI_BVALID(s_axi_control_bvalid),
		.S_AXI_BREADY(s_axi_control_bready),
		.S_AXI_ARADDR(s_axi_control_araddr),
		.S_AXI_ARPROT(s_axi_control_arprot),
		.S_AXI_ARVALID(s_axi_control_arvalid),
		.S_AXI_ARREADY(s_axi_control_arready),
		.S_AXI_RDATA(s_axi_control_rdata),
		.S_AXI_RRESP(s_axi_control_rresp),
		.S_AXI_RVALID(s_axi_control_rvalid),
		.S_AXI_RREADY(s_axi_control_rready)
	);

// Instantiation of Axi Bus Interface out
	vadd_v1_0_out # ( 
		.C_M_AXIS_TDATA_WIDTH(C_out_TDATA_WIDTH),
		.C_M_START_COUNT(C_out_START_COUNT)
	) vadd_v1_0_out_inst (
		.M_AXIS_ACLK(out_aclk),
		.M_AXIS_ARESETN(out_aresetn),
		.M_AXIS_TVALID(out_tvalid),
		.M_AXIS_TDATA(out_tdata),
		.M_AXIS_TSTRB(out_tstrb),
		.M_AXIS_TLAST(out_tlast),
		.M_AXIS_TREADY(out_tready)
	);

// Instantiation of Axi Bus Interface in1
	vadd_v1_0_in1 # ( 
		.C_S_AXIS_TDATA_WIDTH(C_in1_TDATA_WIDTH)
	) vadd_v1_0_in1_inst (
		.S_AXIS_ACLK(in1_aclk),
		.S_AXIS_ARESETN(in1_aresetn),
		.S_AXIS_TREADY(in1_tready),
		.S_AXIS_TDATA(in1_tdata),
		.S_AXIS_TSTRB(in1_tstrb),
		.S_AXIS_TLAST(in1_tlast),
		.S_AXIS_TVALID(in1_tvalid)
	);

// Instantiation of Axi Bus Interface in2
	vadd_v1_0_in2 # ( 
		.C_S_AXIS_TDATA_WIDTH(C_in2_TDATA_WIDTH)
	) vadd_v1_0_in2_inst (
		.S_AXIS_ACLK(in2_aclk),
		.S_AXIS_ARESETN(in2_aresetn),
		.S_AXIS_TREADY(in2_tready),
		.S_AXIS_TDATA(in2_tdata),
		.S_AXIS_TSTRB(in2_tstrb),
		.S_AXIS_TLAST(in2_tlast),
		.S_AXIS_TVALID(in2_tvalid)
	);

	// Add user logic here

	// User logic ends

	endmodule
