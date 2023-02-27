#ifndef NOCOPYABLE_HPP_
#define NOCOPYABLE_HPP_



    class Nocopyable{
        private:
            Nocopyable(const Nocopyable&);
            Nocopyable &operator=(const Nocopyable&);
        protected:
            constexpr Nocopyable() = default;
            virtual ~Nocopyable() = default;
    };


#endif 