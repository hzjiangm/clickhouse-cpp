#pragma once

#include "input.h"

#include <string>

namespace clickhouse {
namespace io {

/**
 * Class which reads and decodes binary data which is composed of varint-
 * encoded integers and fixed-width pieces.
 */
class CodedInputStream {
public:
    /// Create a CodedInputStream that reads from the given InputStream.
    explicit CodedInputStream(ZeroCopyInput* input);

    // Create a CodedInputStream that reads from the given flat array.
    explicit CodedInputStream(const uint8_t* buffer, size_t size);

    // Read an unsigned integer with Varint encoding, truncating to 32 bits.
    // Reading a 32-bit value is equivalent to reading a 64-bit one and casting
    // it to uint32, but may be more efficient.
    bool ReadVarint32(uint32_t* value);

    // Read an unsigned integer with Varint encoding.
    bool ReadVarint64(uint64_t* value);

    // Read raw bytes, copying them into the given buffer.
    bool ReadRaw(void* buffer, size_t size);

    // Like ReadRaw, but reads into a string.
    //
    // Implementation Note:  ReadString() grows the string gradually as it
    // reads in the data, rather than allocating the entire requested size
    // upfront.  This prevents denial-of-service attacks in which a client
    // could claim that a string is going to be MAX_INT bytes long in order to
    // crash the server because it can't allocate this much space at once.
    bool ReadString(std::string* buffer, int size);

    // Skips a number of bytes.  Returns false if an underlying read error
    // occurs.
    bool Skip(size_t count);

private:
    ZeroCopyInput* input_;
    const uint8_t* buffer_;
    const uint8_t* buffer_end_;
};
}
}



