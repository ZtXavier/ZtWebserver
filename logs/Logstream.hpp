#ifndef LOGSTREAM_HPP_
#define LOGSTREAM_HPP_

#include <memory>
#include <string>
#include <functional>
#include <algorithm>
#include <cstring>

#include "../base/Nocopyable.hpp"


namespace zvws{
namespace detail{

constexpr int SmallBuffer = 4000;
constexpr int LargerBuffer = 4000*1000;

template<int BufferSize>
class FixedBuffer : public Nocopyable{
public: 
        FixedBuffer() : Spot(0), Buffer_(std::make_unique<char[]>(BufferSize)){
            setcookie(CookieStart); 
        }
        
        void setcookie(std::function<void()> cookie) {cookie_ = std::move(cookie);}
        constexpr uint32_t Length() const noexcept {return Spot;}
        uint32_t avail() const {return BufferSize - Spot;}
        void add(uint32_t len) {Spot += len;}
        void setSpotBegin() {Spot = 0;}
        char* current() const {return Buffer_.get() + Spot;}
        const char* data() const {return Buffer_.get();}
        void setZero() {memset(Buffer_.get(), 0, BufferSize);}
        void reset() {Spot = 0;}
        std::string toString() const {return std::string(Buffer_.get(), Spot);}

        void append(const char* buf, size_t len){
            if(avail() > len){
                //使用共享内存来拷贝
                memcpy(Buffer_.get() + Spot, buf, len);
                Spot += len;
            }
        }
    private:
        std::unique_ptr<char[]> Buffer_;
        std::function<void()> cookie_;  // 用于在 core dump 文件中查找丢失的日志，其值为某个函数的地址；
        uint32_t Spot;

        const char* end() const {return std::advance(Buffer_, Length());}
        static void CookieStart(){};
        static void CookieEnd(){};
};

    class Logstream{
        using self = Logstream;
    public:
        using Buffer = FixedBuffer<SmallBuffer>;

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
        // self& operator<<(long double);

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

        self& operator<<(const Buffer& v){
            *this << v.toString();
            return *this;
        }

        void append(const char* data, int len) {buffer_.append(data, len); }
        const Buffer& buffer() const { return buffer_; }
        void resetBuffer() { buffer_.reset(); }


    private:
        Buffer buffer_; 
        constexpr const static int kMaxNumericSize = 32;

        template<typename T>
        void formatInteger(T);
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