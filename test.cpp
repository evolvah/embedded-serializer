#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "ByteArray.h"

void test_ByteArray() {
    char        buff[30];
    ByteArray ba(buff, 30);

    // - - - - - - - - - - - - - - - - - - - - - -
    uint32_t    d1 = 1;
    const char* s1 = "blah-blah!";
    uint16_t    d2 = 2;
    uint8_t     d3 = 3;
    double      d4 = 0.123467;

    printf("Serializing  : x1: %d, r1: '%s' x2: %d, x3: %d, x4: %lf\n", d1, s1, d2, d3, d4);
    ba << d1 << s1 << d2 << d3 << d4;
    printf("ok: %s, capacity: %d , position: %d \n", (ba.IsOk() ? "true" : "false"), ba.GetCapacity(), ba.GetPosition());
    assert(ba.IsOk());

    // - - - - - - - - - - - - - - - - - - - - - -
    uint32_t x1 = 0;
    char     r1[20];
    uint16_t x2 = 0;
    uint8_t  x3 = 0;
    double   x4 = 0.0;

    ba.Reset();
    ba >> x1 >> r1 >> x2 >> x3 >> x4;

    printf("Deserializing: x1: %d, r1: '%s' x2: %d, x3: %d, x4: %lf\n", x1, r1, x2, x3, x4);
    assert(x1 == d1);
    assert(x2 == d2);
    assert(x3 == d3);
    assert(strcmp(s1, r1) == 0);

    // - - - - - - - - - - - - - - - - - - - - - -
    printf("Testing overflow\n");
    ba << s1 << s1 << s1 << s1 << s1;
    printf("ok: %s\n", (ba.IsOk() ? "true" : "false"));
    assert(ba.IsOk() == false);
}

class Serializable {
public:
    virtual void    Serialize   (ByteArray& ba) = 0;
    virtual void    Deserialize (ByteArray& ba) = 0;
};

struct LogRecord : public Serializable {
    enum LOG_LEVEL {ERROR, WARNING, INFO};

    uint32_t    schedulerClocksFromStart;
    LOG_LEVEL   level;
    uint32_t    source;
    uint32_t    code;
    uint8_t     textToStore[48];

    void        Print() {
        printf("LogRecord: this: %p schedulerClocksFromStart: %u, level: %u, source: %u, code: %u, textToStore: '%s'\n",
            this, schedulerClocksFromStart, level, source, code, textToStore);
    }

    static const uint32_t MAX_SERIALIZED_LEN = 64;

    void        Serialize (ByteArray& ba) {
        uint32_t level_buffer = (uint32_t)level;
        ba << schedulerClocksFromStart << level_buffer << source << code << (const char*) textToStore;
    }

    void        Deserialize (ByteArray& ba) {
        uint32_t level_buffer = 0;
        ba >> schedulerClocksFromStart >> level_buffer >> source >> code >> (char*) textToStore;
        level = (LogRecord::LOG_LEVEL)level_buffer;
    }
};

void test_LogRecord() {
    // Allocate buffer
    char        buff[LogRecord::MAX_SERIALIZED_LEN];
    ByteArray   ba(buff, LogRecord::MAX_SERIALIZED_LEN);

    // Create and initialize a LogRecord
    LogRecord lr;
    lr.schedulerClocksFromStart = 1;
    lr.level = LogRecord::WARNING;
    lr.source = 2;
    lr.code = 3;
    strcpy((char*)lr.textToStore, "Test string!");

    printf("Serializing LogRecord...\n");
    lr.Print();

    lr.Serialize(ba);

    ba.Reset(); // reset postion in the buffer

    LogRecord new_lr;
    printf("Deserializing LogRecord...\n");
    new_lr.Deserialize(ba);
    new_lr.Print();
}

int main(int argc, char const *argv[])
{
    test_ByteArray();
    test_LogRecord();

    return 0;
}