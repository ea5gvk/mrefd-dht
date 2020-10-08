//
//  Copyright © 2020 Thomas A. Early, N7TAE
//
// ----------------------------------------------------------------------------
//
//    m17ref is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    m17ref is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    with this software.  If not, see <http://www.gnu.org/licenses/>.
// ----------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <string.h>
#include <memory>

#include "callsign.h"

////////////////////////////////////////////////////////////////////////////////////////
// aliases

// M17 Packets
//all structures must be big endian on the wire, so you'll want htonl (man byteorder 3) and such.
using AM17Lich = struct __attribute__((__packed__)) _LICH {
	uint8_t  addr_dst[6];
	uint8_t  addr_src[6];
	uint16_t frametype; //frametype flag field per the M17 spec
	uint8_t  nonce[14]; //bytes for the nonce
}; // 6 + 6 + 2 + 14 = 28 bytes = 224 bits

//without SYNC or other parts
using AM17Frame = struct __attribute__((__packed__)) _ip_frame {
	uint8_t  magic[4];
	uint16_t streamid;
	AM17Lich lich;
	uint16_t framenumber;
	uint8_t  payload[16];
	uint16_t crc; 	//16 bit CRC
}; // 4 + 2 + 28 + 2 + 16 + 2 = 54 bytes = 432 bits

class CPacket
{
public:
	CPacket() {}
	CPacket(const uint8_t *buf);
	const CCallsign &GetDestCallsign() const;
	char GetDestModule() const;
	const CCallsign &GetSourceCallsign() const;
	uint16_t GetStreamId() const;
	uint16_t GetCRC() const;
	void SetCRC(uint16_t crc);
	std::unique_ptr<CPacket> Duplicate(void) const;
	bool IsLastPacket() const;
	AM17Frame &GetFrame();

private:
	CCallsign destination, source;
	AM17Frame m17;
};
