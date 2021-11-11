/// @author    Johannes de Fine Licht (definelicht@inf.ethz.ch)
/// @copyright This software is copyrighted under the BSD 3-Clause License. 

#pragma once

#include <ap_int.h>
#include <cstddef>

namespace hlslib {

namespace axi {

/// Implements the AXI Stream interface, which is inferred by the name of the
/// variables (data, keep and last).
template <typename T, unsigned int D=1>
struct Stream {

  T data;
  ap_uint<sizeof(T)> keep{~(keep & 0)}; // Set all bits to 1
  ap_uint<1> last;
  ap_uint<D> dest{0}; // Default TDEST is zero

  Stream() : data(), keep(0), last(1), dest(0) {}
  Stream(decltype(data) const &_data, decltype(last) const &_last, decltype(dest) const &_dest)
      : data(_data), last(_last), dest(_dest) {}
  Stream(decltype(data) const &_data) : data(_data), last(1), dest(0) {}
  Stream(decltype(data) const &_data, decltype(last) const &_last) : data(_data), last(_last), dest(0) {}
};

/// Implements the command bus interface for the DataMover IP
/// https://www.xilinx.com/support/documentation/ip_documentation/axi_datamover/v5_1/pg022_axi_datamover.pdf
template <size_t addressWidth, size_t bttWidth>
struct Command {

  // Typically addressWith is in {32, 64} and bttWidth is 23, so the whole bus
  // is 72 or 104 bits.
  ap_uint<bttWidth> length; // BTT (bytes to transfer)
  ap_uint<1> type{1};
  ap_uint<6> dsa{0};
  ap_uint<1> eof{1};
  ap_uint<1> drr{1};
  ap_uint<addressWidth> address;
  ap_uint<4> tag{0};
  ap_uint<4> reserved{0};

  Command(decltype(address) const &_address, decltype(length) const &_length)
      : length(_length), address(_address) {}
  Command() : length(0), address(0) {}
};

/// Implements the status bus interface for the DataMover IP
/// https://www.xilinx.com/support/documentation/ip_documentation/axi_datamover/v5_1/pg022_axi_datamover.pdf
struct Status { // 32 bits

  ap_uint<4> tag{0};
  ap_uint<1> internalError{0};
  ap_uint<1> decodeError{0};
  ap_uint<1> slaveError{0};
  ap_uint<1> okay;
  ap_uint<23> bytesReceived{0};
  ap_uint<1> endOfPacket{0};

  Status(bool _okay, bool _eop) : okay(_okay), endOfPacket(_eop) {}
  Status(bool _okay) : okay(_okay) {}
  Status() : okay(true) {}
};

} // End namespace axi 

} // End namespace hlslib
