
#include <ap_int.h>
#include <hls_stream.h>

void idma(ap_uint<32> *mem, hls::stream<ap_uint<32>> &op0){
#pragma HLS INTERFACE m_axi port=mem offset=slave depth=3
#pragma HLS INTERFACE axis port=op0 register_mode=both
#pragma HLS INTERFACE s_axilite port=return
    op0.write(mem[0]);
}

void odma(ap_uint<32> *mem, hls::stream<ap_uint<32>> &res){
#pragma HLS INTERFACE m_axi port=mem offset=slave depth=3
#pragma HLS INTERFACE axis port=res register_mode=both
#pragma HLS INTERFACE s_axilite port=return
    mem[2] = res.read();
}