#include <string>

enum Type{
    NUMBER, OPERATOR, LPAR, RPAR
};

class Token{
    public:
        Token(std::string s, Type t = Type::NUMBER, int precedence = -1){
            this->content = s;
            this->type = t;
            this->precedence = precedence;
        };
        std::string content;
        Type type;
        int precedence; 
};