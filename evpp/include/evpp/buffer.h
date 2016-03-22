#ifndef LIBEVENTPP_BUFFER_H_
#define LIBEVENTPP_BUFFER_H_

#include "evpp/inner_pre.h"
#include "evpp/slice.h"
#include "evpp/sockets.h"

#include <algorithm>

namespace evpp {
    class _EXPORT_LIBEVENTPP Buffer {
    public:
        static const size_t kCheapPrepend = 8;
        static const size_t kInitialSize = 1024;

        explicit Buffer(size_t initialSize = kInitialSize)
            : capacity_(kCheapPrepend + initialSize),
            readerIndex_(kCheapPrepend),
            writerIndex_(kCheapPrepend) {
            buffer_ = new char[capacity_];
            assert(length() == 0);
            assert(writableBytes() == initialSize);
            assert(prependableBytes() == kCheapPrepend);
        }

        ~Buffer() {
            delete[] buffer_;
            buffer_ = NULL;
            capacity_ = 0;
        }

        // implicit copy-ctor, move-ctor, dtor and assignment are fine
        // NOTE: implicit move-ctor is added in g++ 4.6

        void swap(Buffer& rhs) {
            std::swap(buffer_, rhs.buffer_);
            std::swap(capacity_, rhs.capacity_);
            std::swap(readerIndex_, rhs.readerIndex_);
            std::swap(writerIndex_, rhs.writerIndex_);
        }

        size_t writableBytes() const {
            assert(capacity_ >= writerIndex_);
            return capacity_ - writerIndex_;
        }

        size_t prependableBytes() const {
            return readerIndex_;
        }

        const char* peek() const {
            return begin() + readerIndex_;
        }

        const char* findCRLF() const {
            const char* crlf = std::search(peek(), beginWrite(), kCRLF, kCRLF + 2);
            return crlf == beginWrite() ? NULL : crlf;
        }

        const char* findCRLF(const char* start) const {
            assert(peek() <= start);
            assert(start <= beginWrite());
            const char* crlf = std::search(start, beginWrite(), kCRLF, kCRLF + 2);
            return crlf == beginWrite() ? NULL : crlf;
        }

        const char* findEOL() const {
            const void* eol = memchr(peek(), '\n', length());
            return static_cast<const char*>(eol);
        }

        const char* findEOL(const char* start) const {
            assert(peek() <= start);
            assert(start <= beginWrite());
            const void* eol = memchr(start, '\n', beginWrite() - start);
            return static_cast<const char*>(eol);
        }

        // retrieve returns void, to prevent
        // string str(retrieve(readableBytes()), readableBytes());
        // the evaluation of two functions are unspecified
        void retrieve(size_t len) {
            Next(len);
        }

        void retrieveInt64() {
            retrieve(sizeof(int64_t));
        }

        void retrieveInt32() {
            retrieve(sizeof(int32_t));
        }

        void retrieveInt16() {
            retrieve(sizeof(int16_t));
        }

        void retrieveInt8() {
            retrieve(sizeof(int8_t));
        }

        // Truncate discards all but the first n unread bytes from the buffer
        // but continues to use the same allocated storage.
        // It does nothing if n is greater than the length of the buffer.
        void Truncate(size_t n) {
            if (n == 0) {
                readerIndex_ = kCheapPrepend;
                writerIndex_ = kCheapPrepend;
            } else if (writerIndex_ > readerIndex_ + n) {
                writerIndex_ = readerIndex_ + n;
            }
        }

        // Reset resets the buffer to be empty,
        // but it retains the underlying storage for use by future writes.
        // Reset is the same as Truncate(0).
        void Reset() {
            Truncate(0);
        }
        
        void append(const Slice& str) {
            append(str.data(), str.size());
        }

        void append(const char* /*restrict*/ data, size_t len) {
            ensureWritableBytes(len);
            memcpy(beginWrite(), data, len);
            hasWritten(len);
        }

        void append(const void* /*restrict*/ data, size_t len) {
            append(static_cast<const char*>(data), len);
        }

        void ensureWritableBytes(size_t len) {
            if (writableBytes() < len) {
                grow(len);
            }
            assert(writableBytes() >= len);
        }

        char* beginWrite() {
            return begin() + writerIndex_;
        }

        const char* beginWrite() const {
            return begin() + writerIndex_;
        }

        void hasWritten(size_t len) {
            assert(len <= writableBytes());
            writerIndex_ += len;
        }

        void unwrite(size_t len) {
            assert(len <= length());
            writerIndex_ -= len;
        }

        ///
        /// Append int32_t using network endian
        ///
        void appendInt32(int32_t x) {
            int32_t be32 = ::htonl(x);
            append(&be32, sizeof be32);
        }

        void appendInt16(int16_t x) {
            int16_t be16 = ::htons(x);
            append(&be16, sizeof be16);
        }

        void appendInt8(int8_t x) {
            append(&x, sizeof x);
        }

        ///
        /// Read int32_t from network endian
        ///
        /// Require: buf->readableBytes() >= sizeof(int32_t)
        int32_t readInt32() {
            int32_t result = peekInt32();
            retrieveInt32();
            return result;
        }

        int16_t readInt16() {
            int16_t result = peekInt16();
            retrieveInt16();
            return result;
        }

        int8_t readInt8() {
            int8_t result = peekInt8();
            retrieveInt8();
            return result;
        }

        ///
        /// Peek int32_t from network endian
        ///
        /// Require: buf->readableBytes() >= sizeof(int32_t)
        int32_t peekInt32() const {
            assert(length() >= sizeof(int32_t));
            int32_t be32 = 0;
            ::memcpy(&be32, peek(), sizeof be32);
            return ::ntohl(be32);
        }

        int16_t peekInt16() const {
            assert(length() >= sizeof(int16_t));
            int16_t be16 = 0;
            ::memcpy(&be16, peek(), sizeof be16);
            return ::ntohs(be16);
        }

        int8_t peekInt8() const {
            assert(length() >= sizeof(int8_t));
            int8_t x = *peek();
            return x;
        }

        ///
        /// Prepend int32_t using network endian
        ///
        void prependInt32(int32_t x) {
            int32_t be32 = ::htonl(x);
            prepend(&be32, sizeof be32);
        }

        void prependInt16(int16_t x) {
            int16_t be16 = ::htons(x);
            prepend(&be16, sizeof be16);
        }

        void prependInt8(int8_t x) {
            prepend(&x, sizeof x);
        }

        void prepend(const void* /*restrict*/ data, size_t len) {
            assert(len <= prependableBytes());
            readerIndex_ -= len;
            const char* d = static_cast<const char*>(data);
            memcpy(begin() + readerIndex_, d, len);
        }

        void Shrink(size_t reserve) {
            Buffer other(length() + reserve);
            other.append(ToSlice());
            swap(other);
        }

        /// Read data directly into buffer.
        ///
        /// It may implement with readv(2)
        /// @return result of read(2), @c errno is saved
        ssize_t ReadFromFD(SOCKET fd, int* savedErrno);

        // Next returns a slice containing the next n bytes from the buffer,
        // advancing the buffer as if the bytes had been returned by Read.
        // If there are fewer than n bytes in the buffer, Next returns the entire buffer.
        // The slice is only valid until the next call to a read or write method.
        Slice Next(size_t len) {
            if (len < length()) {
                Slice result(peek(), len);
                readerIndex_ += len;
                return result;
            }
            return NextAll();
        }

        // NextAll returns a slice containing all the unread portion of the buffer,
        // advancing the buffer as if the bytes had been returned by Read.
        Slice NextAll() {
            Slice result(peek(), length());
            Reset();
            return result;
        }

        std::string NextString(size_t len) {
            Slice s = Next(len);
            return std::string(s.data(), s.size());
        }

        std::string NextAllString() {
            Slice s = NextAll();
            return std::string(s.data(), s.size());
        }

        Slice ToSlice() const {
            return Slice(peek(), static_cast<int>(length()));
        }

        // ReadByte reads and returns the next byte from the buffer.
        // If no byte is available, it returns '\0'.
        char ReadByte() {
            assert(length() >= 1);
            if (length() == 0) {
                return '\0';
            }
            return buffer_[readerIndex_++];
        }

        // data returns a pointer of length Buffer.length() holding the unread portion of the buffer.
        // The data is valid for use only until the next buffer modification (that is,
        // only until the next call to a method like Read, Write, Reset, or Truncate).
        // The data aliases the buffer content at least until the next buffer modification,
        // so immediate changes to the slice will affect the result of future reads.
        const char* data() const {
            return buffer_ + readerIndex_;
        }

        // length returns the number of bytes of the unread portion of the buffer
        size_t length() const {
            assert(writerIndex_ >= readerIndex_);
            return writerIndex_ - readerIndex_;
        }

        // size returns the number of bytes of the unread portion of the buffer.
        // It is the same as length().
        size_t size() const {
            return length();
        }

        // capacity returns the capacity of the buffer's underlying byte slice, that is, the
        // total space allocated for the buffer's data.
        size_t capacity() const {
            return capacity_;
        }

    private:
        char* begin() {
            return buffer_;
        }

        const char* begin() const {
            return buffer_;
        }

        void grow(size_t len) {
            if (writableBytes() + prependableBytes() < len + kCheapPrepend) {
                //grow the capacity
                size_t n = (capacity_ << 1) + len;
                size_t m = length();
                char* d = new char[n];
                memcpy(d + kCheapPrepend, begin() + readerIndex_, m);
                writerIndex_ = m + kCheapPrepend;
                readerIndex_ = kCheapPrepend;
                capacity_ = n;
                delete[] buffer_;
                buffer_ = d;
            } else {
                // move readable data to the front, make space inside buffer
                assert(kCheapPrepend < readerIndex_);
                size_t readable = length();
                memcpy(begin() + kCheapPrepend, begin() + readerIndex_, length());
                readerIndex_ = kCheapPrepend;
                writerIndex_ = readerIndex_ + readable;
                assert(readable == length());
                assert(writableBytes() >= len);
            }
        }

    private:
        char* buffer_;
        size_t capacity_;
        size_t readerIndex_;
        size_t writerIndex_;

        static const char kCRLF[];
    };

}

#endif

