#ifndef LOGSTREAM_HPP_
#define LOGSTREAM_HPP_

#include <memory>
#include <string>
#include <functional>
#include <algorithm>
#include <cstring>

#include "Nocopyable.hpp"


namespace zvws{
namespace detail{

constexpr int kSmallBuffer = 4000;
constexpr int kLargerBuffer = 4000*1000;

template<int BufferSize>
class FixedBuffer : public Nocopyable{
public: 
    FixedBuffer() : cur_(data_) {}

    ~FixedBuffer() {}

  void append(const char* buf, size_t len) {
    if (avail() > static_cast<int>(len)) {
      memcpy(cur_, buf, len);
      cur_ += len;
    }
  }

  const char* data() const { return data_; }
  int Length() const { return static_cast<int>(cur_ - data_); }

  char* current() { return cur_; }
  int avail() const { return static_cast<int>(end() - cur_); }
  void add(size_t len) { cur_ += len; }

  void reset() { cur_ = data_; }
  void setZero() { memset(data_, 0, sizeof data_); }

 private:
  const char* end() const { return data_ + sizeof data_; }

  char data_[BufferSize];
  char* cur_;
};

    class Logstream{
        using self = Logstream;
    public:
        using Buffer = FixedBuffer<kSmallBuffer>;

        self& operator<<(bool flag) {
            buffer_.append(flag ? "1" : "0", 1);
            return *this;
        }

        self& operator<<(short);
        self& operator<<(unsigned short);
        self& operator<<(int);
        self& operator<<(unsigned int);
        self& operator<<(long);
        self& operator<<(unsigned long);
        self& operator<<(long long);
        self& operator<<(unsigned long long);

        self& operator<<(const void*);

        self& operator<<(float v) {
            *this << static_cast<double>(v);
            return *this;
        }
        self& operator<<(double);
        self& operator<<(long double);

        self& operator<<(char v) {
            buffer_.append(&v, 1);
            return *this;
        }

        // self& operator<<(signed char);
        // self& operator<<(unsigned char);

        self& operator<<(const char* str){
            if(str){
                buffer_.append(str, strlen(str));
            }else{
                buffer_.append("(null)", 6);
            }
            return *this;
        }

        self& operator<<(const unsigned char* str){
            return operator<<(reinterpret_cast<const char*>(str));
        }

        self& operator<<(const std::string& v){
            buffer_.append(v.c_str(), v.size());
            return *this;
        }


        void append(const char* data, int len) {buffer_.append(data, len); }
        const Buffer& buffer() const { return buffer_; }
        void resetBuffer() { buffer_.reset(); }


    private:
        Buffer buffer_; 
        template<typename T>
        void formatInteger(T);
        constexpr const static int kMaxNumericSize = 32;
    };

    class Fmt{// : noncopyable
        public:
        template<typename T>
        Fmt(const char* fmt, T val);

        const char* data() const { return buf_; }
        int length() const { return length_; }

        private:
        char buf_[32];
        int length_;
    };

}

}

#endif //LOGSTREAM_H_