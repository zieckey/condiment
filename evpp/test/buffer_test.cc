
#include "test_common.h"

#include "evpp/buffer.h"

using evpp::Buffer;
using std::string;

TEST_UNIT(testBufferAppendRetrieve)
{
    Buffer buf;
    H_TEST_EQUAL(buf.readableBytes(), 0);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

    const string str(200, 'x');
    buf.append(str);
    H_TEST_EQUAL(buf.readableBytes(), str.size());
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - str.size());
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

    const string str2 = buf.retrieveAsString(50);
    H_TEST_EQUAL(str2.size(), 50);
    H_TEST_EQUAL(buf.readableBytes(), str.size() - str2.size());
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - str.size());
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + str2.size());

    buf.append(str);
    H_TEST_EQUAL(buf.readableBytes(), 2 * str.size() - str2.size());
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 2 * str.size());
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + str2.size());

    const string str3 = buf.retrieveAllAsString();
    H_TEST_EQUAL(str3.size(), 350);
    H_TEST_EQUAL(buf.readableBytes(), 0);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
}

TEST_UNIT(testBufferGrow)
{
    Buffer buf;
    buf.append(string(400, 'y'));
    H_TEST_EQUAL(buf.readableBytes(), 400);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 400);

    buf.retrieve(50);
    H_TEST_EQUAL(buf.readableBytes(), 350);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 400);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + 50);

    buf.append(string(1000, 'z'));
    H_TEST_EQUAL(buf.readableBytes(), 1350);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
    H_TEST_ASSERT(buf.writableBytes() >= 0);

    buf.retrieveAll();
    H_TEST_EQUAL(buf.readableBytes(), 0);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
    H_TEST_ASSERT(buf.writableBytes() >= Buffer::kInitialSize*2);
}

TEST_UNIT(testBufferInsideGrow)
{
    Buffer buf;
    buf.append(string(800, 'y'));
    H_TEST_EQUAL(buf.readableBytes(), 800);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 800);

    buf.retrieve(500);
    H_TEST_EQUAL(buf.readableBytes(), 300);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 800);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + 500);

    buf.append(string(300, 'z'));
    H_TEST_EQUAL(buf.readableBytes(), 600);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 600);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
}

TEST_UNIT(testBufferShrink)
{
    Buffer buf;
    buf.append(string(2000, 'y'));
    H_TEST_EQUAL(buf.readableBytes(), 2000);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
    H_TEST_ASSERT(buf.writableBytes() >= 0);

    buf.retrieve(1500);
    H_TEST_EQUAL(buf.readableBytes(), 500);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend + 1500);
    H_TEST_ASSERT(buf.writableBytes() >= 0);

    buf.shrink(0);
    H_TEST_EQUAL(buf.readableBytes(), 500);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 500);
    H_TEST_EQUAL(buf.retrieveAllAsString(), string(500, 'y'));
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);
}

TEST_UNIT(testBufferPrepend)
{
    Buffer buf;
    buf.append(string(200, 'y'));
    H_TEST_EQUAL(buf.readableBytes(), 200);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 200);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend);

    int x = 0;
    buf.prepend(&x, sizeof x);
    H_TEST_EQUAL(buf.readableBytes(), 204);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize - 200);
    H_TEST_EQUAL(buf.prependableBytes(), Buffer::kCheapPrepend - 4);
}

TEST_UNIT(testBufferReadInt)
{
    Buffer buf;
    buf.append("HTTP");

    H_TEST_EQUAL(buf.readableBytes(), 4);
    H_TEST_EQUAL(buf.peekInt8(), 'H');
    int top16 = buf.peekInt16();
    H_TEST_EQUAL(top16, 'H' * 256 + 'T');
    H_TEST_EQUAL(buf.peekInt32(), top16 * 65536 + 'T' * 256 + 'P');

    H_TEST_EQUAL(buf.readInt8(), 'H');
    H_TEST_EQUAL(buf.readInt16(), 'T' * 256 + 'T');
    H_TEST_EQUAL(buf.readInt8(), 'P');
    H_TEST_EQUAL(buf.readableBytes(), 0);
    H_TEST_EQUAL(buf.writableBytes(), Buffer::kInitialSize);

    buf.appendInt8(-1);
    buf.appendInt16(-1);
    buf.appendInt32(-1);
    H_TEST_EQUAL(buf.readableBytes(), 7);
    H_TEST_EQUAL(buf.readInt8(), -1);
    H_TEST_EQUAL(buf.readInt32(), -1);
    H_TEST_EQUAL(buf.readInt16(), -1);
}

TEST_UNIT(testBufferFindEOL)
{
    Buffer buf;
    buf.append(string(100000, 'x'));
    const char* null = NULL;
    H_TEST_EQUAL(buf.findEOL(), null);
    H_TEST_EQUAL(buf.findEOL(buf.peek() + 90000), null);
}