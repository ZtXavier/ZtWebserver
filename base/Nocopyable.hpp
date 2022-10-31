#ifndef NOCOPYABLE_H_
#define NOCOPYABLE_H_



    class Nocopyable{
        private:
            Nocopyable(const Nocopyable&);
            Nocopyable &operator=(const Nocopyable&);
        protected:
            constexpr Nocopyable() = default;
            virtual ~Nocopyable() = default;
    };


#endif 