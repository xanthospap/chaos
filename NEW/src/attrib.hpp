#ifndef __ATTRIBUTES_TYPES__
#define __ATTRIBUTES_TYPES__

namespace ngpt {

class Attribute {
    public:
        Attribute():m_attribute('X'){};
        explicit Attribute(char c):m_attribute(c){};

        inline char as_char () const {return m_attribute;}

    private:
        char m_attribute;
};

} // end namespace ngpt

#endif
