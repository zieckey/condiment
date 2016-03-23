// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "evpp/base/inner_pre.h"
#include "evpp/base/ip_address.h"

namespace evpp {
    namespace base {

        IPAddress::IPAddress() {}

        IPAddress::IPAddress(const IPAddress& other) = default;

        IPAddress::IPAddress(const uint8_t* address, size_t address_len)
            : ip_address_(address, address + address_len) {}

        IPAddress::IPAddress(uint8_t b0, uint8_t b1, uint8_t b2, uint8_t b3) {
            ip_address_.reserve(4);
            ip_address_.push_back(b0);
            ip_address_.push_back(b1);
            ip_address_.push_back(b2);
            ip_address_.push_back(b3);
        }

        IPAddress::IPAddress(uint8_t b0,
            uint8_t b1,
            uint8_t b2,
            uint8_t b3,
            uint8_t b4,
            uint8_t b5,
            uint8_t b6,
            uint8_t b7,
            uint8_t b8,
            uint8_t b9,
            uint8_t b10,
            uint8_t b11,
            uint8_t b12,
            uint8_t b13,
            uint8_t b14,
            uint8_t b15) {
            const uint8_t address[] = { b0, b1, b2, b3, b4, b5, b6, b7,
                b8, b9, b10, b11, b12, b13, b14, b15 };
            //ip_address_ = std::vector<uint8_t>(std::begin(address), std::end(address));
            for (int i = 0; i < 16; ++i) {
                ip_address_.push_back(address[i]);
            }
        }

        IPAddress::~IPAddress() {}

        bool IPAddress::IsIPv4() const {
            return ip_address_.size() == kIPv4AddressSize;
        }

        bool IPAddress::IsIPv6() const {
            return ip_address_.size() == kIPv6AddressSize;
        }

        bool IPAddress::IsValid() const {
            return IsIPv4() || IsIPv6();
        }

        bool IPAddress::IsReserved() const {
            //return IsIPAddressReserved(ip_address_);
            //TODO
            return false;
        }

        bool IPAddress::IsZero() const {
            for (size_t i = 0; i < ip_address_.size(); ++i) {
                if (ip_address_[i] != 0) {
                    return false;
                }
            }
            return !ip_address_.empty();
            //for (auto x : ip_address_) {
            //    if (x != 0)
            //        return false;
            //}

            //return !ip_address_.empty();
        }

        bool IPAddress::IsIPv4MappedIPv6() const {
            //return net::IsIPv4Mapped(ip_address_);
            //TODO
            return false;
        }

        std::string IPAddress::ToString() const {
            //return IPAddressToString(ip_address_);
            //TODO
            return "";
        }

        bool IPAddress::AssignFromIPLiteral(const Slice& ip_literal) {
//             std::vector<uint8_t> number;
//             if (!ParseIPLiteralToNumber(ip_literal, &number))
//                 return false;
// 
//             std::swap(number, ip_address_);
            //TODO
            return true;
        }

        //TODO
        // static
        //IPAddress IPAddress::IPv4Localhost() {
        //    static const uint8_t kLocalhostIPv4[] = { 127, 0, 0, 1 };
        //    return IPAddress(kLocalhostIPv4);
        //}

        //TODO
        // static
        //IPAddress IPAddress::IPv6Localhost() {
        //    static const uint8_t kLocalhostIPv6[] = { 0, 0, 0, 0, 0, 0, 0, 0,
        //        0, 0, 0, 0, 0, 0, 0, 1 };
        //    return IPAddress(kLocalhostIPv6);
        //}

        // static
        IPAddress IPAddress::AllZeros(size_t num_zero_bytes) {
            IPAddress ip;
            ip.ip_address_.resize(num_zero_bytes);
            //TODO bzero
            return ip;
        }

        // static
        IPAddress IPAddress::IPv4AllZeros() {
            return AllZeros(kIPv4AddressSize);
        }

        // static
        IPAddress IPAddress::IPv6AllZeros() {
            return AllZeros(kIPv6AddressSize);
        }

        bool IPAddress::operator==(const IPAddress& that) const {
            return ip_address_ == that.ip_address_;
        }

        bool IPAddress::operator!=(const IPAddress& that) const {
            return ip_address_ != that.ip_address_;
        }

        bool IPAddress::operator<(const IPAddress& that) const {
            // Sort IPv4 before IPv6.
            if (ip_address_.size() != that.ip_address_.size()) {
                return ip_address_.size() < that.ip_address_.size();
            }

            return ip_address_ < that.ip_address_;
        }

        std::string IPAddressToStringWithPort(const IPAddress& address, uint16_t port) {
            //return IPAddressToStringWithPort(address.bytes(), port);
            //TODO
            return std::string();
        }

        std::string IPAddressToPackedString(const IPAddress& address) {
            //return IPAddressToPackedString(address.bytes());
            //TODO
            return std::string();
        }

        IPAddress ConvertIPv4ToIPv4MappedIPv6(const IPAddress& address) {
            //return IPAddress(ConvertIPv4NumberToIPv6Number(address.bytes()));
            //TODO
            return IPAddress();
        }

        IPAddress ConvertIPv4MappedIPv6ToIPv4(const IPAddress& address) {
            //return IPAddress(ConvertIPv4MappedToIPv4(address.bytes()));
            //TODO
            return IPAddress();
        }

        bool IPAddressMatchesPrefix(const IPAddress& ip_address,
                         const IPAddress& ip_prefix,
                         size_t prefix_length_in_bits) {
            //             return IPNumberMatchesPrefix(ip_address.bytes(), ip_prefix.bytes(),
            //                 prefix_length_in_bits);
            //TODO 
            return true;
        }

        bool ParseCIDRBlock(const std::string& cidr_literal,
            IPAddress* ip_address,
            size_t* prefix_length_in_bits) {
            //TODO
            // We expect CIDR notation to match one of these two templates:
            //   <IPv4-literal> "/" <number of bits>
            //   <IPv6-literal> "/" <number of bits>

//             std::vector<Slice> parts = Slice(
//                 cidr_literal, "/", base::TRIM_WHITESPACE, base::SPLIT_WANT_ALL);
//             if (parts.size() != 2)
//                 return false;
// 
//             // Parse the IP address.
//             if (!ip_address->AssignFromIPLiteral(parts[0]))
//                 return false;
// 
//             // TODO(martijnc): Find a more general solution for the overly permissive
//             // base::StringToInt() parsing. https://crbug.com/596523.
//             const base::StringPiece& prefix_length = parts[1];
//             if (prefix_length.starts_with("+"))
//                 return false;
// 
//             // Parse the prefix length.
//             int number_of_bits = -1;
//             if (!base::StringToInt(prefix_length, &number_of_bits))
//                 return false;
// 
//             // Make sure the prefix length is in a valid range.
//             if (number_of_bits < 0 ||
//                 number_of_bits > static_cast<int>(ip_address->size() * 8))
//                 return false;
// 
//             *prefix_length_in_bits = static_cast<size_t>(number_of_bits);
            return true;
        }


        //TODO
//         unsigned CommonPrefixLength(const IPAddress& a1, const IPAddress& a2) {
//             return CommonPrefixLength(a1.bytes(), a2.bytes());
//         }
// 
//         unsigned MaskPrefixLength(const IPAddress& mask) {
//             return MaskPrefixLength(mask.bytes());
//         }
    } // namespace base
}  // namespace evpp
