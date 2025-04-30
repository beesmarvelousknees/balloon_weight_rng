// utils.h
#ifndef UTILS_H
#define UTILS_H

#include <iostream>

#include <iomanip>
#include <string>
#include <sstream>

class Utils
{
    public:

        // Returns a hexadecimal string from a byte buffer.
        static std::string bytes_to_hex(unsigned char* buf, std::size_t size)
        {
            std::stringstream ss;
            for (int i=0; i<size; i++)
            {
                ss << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(buf[i]); 
            }
            return ss.str();
        }

        static void hex_to_bytes(const std::string& hex, unsigned char* bytes) 
        {
            for (unsigned int i = 0; i < hex.length(); i += 2) 
            {
                std::string byteString = hex.substr(i, 2);
                unsigned char byte = static_cast<unsigned char>(strtol(byteString.c_str(), NULL, 16));
                bytes[i/2] = byte;
            }
        }


};

#endif
