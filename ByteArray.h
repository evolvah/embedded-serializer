#ifndef ByteArray_h_
#define ByteArray_h_

#include <stdint.h>
#include <string.h>

// Thin wrapper for static byte array buffer serialization
class ByteArray {
    uint32_t    position;
    uint32_t    capacity;
    uint8_t*    data;
    bool        ok;

    // Hide restricted constructors
    ByteArray();
    ByteArray(const ByteArray& ba);
    ByteArray& operator = (const ByteArray& ba);

public:
    ByteArray(void* dataIn, const uint32_t len):
        position(0), capacity(len), data((uint8_t*)dataIn), ok(true)
        {}
    ~ByteArray() {}

    // Get current read/write position in the buffer
    uint32_t        GetPosition() const { return position; }
    // Get buffer maximum capacity in bytes
    uint32_t        GetCapacity() const { return capacity; }
    // Get pointer to the beginning of the data
    const uint8_t*  GetData() const     { return data; }
    // Is the buffer healthy/valid?
    bool            IsOk() const        { return ok; }
    // Reset position and the healthy/valid flag
    void            Reset()             { position = 0; ok = true; }

    // Serialize a primitive type
    template <typename T>
    ByteArray&  operator << (const T& value) {
        if (!ok) return (*this);

        ok = ok && (position + sizeof(value) <= capacity);
        if (ok) {
            memcpy(data + position, &value, sizeof(value));
            position += sizeof(value);
        }
        return (*this);
    }

    // Serialize a null-terminated string including the '\0' at the end
    ByteArray&  operator << (const char* value) {
        if (!ok) return (*this);

        uint32_t len = strlen(value) + 1;
        ok = ok && (position + len <= capacity);
        if (ok) {
            memcpy(data + position, value, len);
            position += len;
        }
        return (*this);
    }

    // Deserialize a primitive type
    template <typename T>
    ByteArray&  operator >> (T& value) {
        if (!ok) return (*this);

        ok = ok && (position + sizeof(value) <= capacity);
        if (ok) {
            memcpy(&value, data + position, sizeof(value));
            position += sizeof(value);
        }
        return (*this);
    }

    // Deserialize a null-terminated string including the '\0' at the end
    ByteArray&  operator >> (char* value) {
        if (!ok) return (*this);

        uint32_t len = strlen((char*)data + position) + 1;
        ok = ok && (position + len <= capacity);
        if (ok) {
            memcpy(value, data + position, len);
            position += len;
        }
        return (*this);
    }
};

#endif