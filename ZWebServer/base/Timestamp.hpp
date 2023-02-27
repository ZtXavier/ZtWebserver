#ifndef TIMESTAMP_HPP_
#define TIMESTAMP_HPP_

#include <string>
#include <algorithm>

#include "Copyable.hpp"
#include <boost/operators.hpp>

namespace zvws{

namespace detail{

class Timestamp : public Copyable,
                  public boost::equality_comparable<Timestamp>,
                  public boost::less_than_comparable<Timestamp>{
public:
    Timestamp() : microsecondsSinceEpoch_(0){}
    explicit Timestamp(uint64_t args) : microsecondsSinceEpoch_(args){}


    void swap(Timestamp& T){
        std::swap(T.microsecondsSinceEpoch_, microsecondsSinceEpoch_);
    }
    void swap(Timestamp&& T){
        std::swap(T.microsecondsSinceEpoch_, microsecondsSinceEpoch_);
    }

    std::string toString() const;
    std::string toFormattedString(bool showMicroseconds = true) const;

    uint64_t Data_microsecond() const {return microsecondsSinceEpoch_;}
    time_t Data_second() const { return static_cast<time_t>(microsecondsSinceEpoch_/KmicroSecond);}

    static Timestamp now();
    static Timestamp invalid(){
        return Timestamp();
    }
    static Timestamp fromUnixTime(time_t t){
        return fromUnixTime(t, 0);
    }

    static Timestamp fromUnixTime(time_t t, int microseconds){
        return Timestamp(static_cast<int64_t>(t) * KmicroSecond + microseconds);
    }

    static constexpr const int KmicroSecond = 1E6;
private:
    uint64_t microsecondsSinceEpoch_;
};

inline bool operator<(Timestamp lhs, Timestamp rhs){
    return lhs.Data_microsecond() < rhs.Data_microsecond();
}

inline bool operator==(Timestamp lhs, Timestamp rhs){
    return lhs.Data_microsecond() == rhs.Data_microsecond();
}

inline Timestamp addTime(Timestamp timestamp, double seconds){
    uint64_t delta = static_cast<uint64_t>(seconds * Timestamp::KmicroSecond);
    return Timestamp(timestamp.Data_microsecond() + delta);
}

}

}

#endif //TIMESTAMP_H_