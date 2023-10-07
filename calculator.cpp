#include "calculator.h"

#include <iostream>
#include <vector>
#include <deque>

float parse(std::deque<Token>& data){
    Token t = data[data.size()-1];
    data.pop_back();
    if(t.type == Type::NUMBER){
        return std::stof(t.content);
    } else if(t.type == Type::OPERATOR){
        float operand1 = parse(data); 
        float operand2 = parse(data);
        char c = t.content[0];
        switch (c)
        {
            case '+': return operand2 + operand1; 
            case '-': return operand2 - operand1;
            case '*': return operand2 * operand1;
            case '/': return operand2 / operand1;
        }
    }
    return 0;
}

int main(int argc, char **argv){
    try{
        std::string input = argv[1];

        std::vector<Token> tokens;
        for(int i = 0; i < input.size(); i++){
            if(isdigit(input[i])){
                // handle if i is digit
                std::string temp = "";
                while(isdigit(input[i]) || input[i] == '.'){
                    temp += input[i];
                    i++;
                }
                i -= 1;
                tokens.push_back(Token(temp));

            } else {
                // handle if not digit
                std::string temp = "";
                temp += input[i];
                switch (input[i])
                {
                case '+': case '-':
                    tokens.push_back(Token(temp, Type::OPERATOR, 1));
                    break;
                case '*': case '/':
                    tokens.push_back(Token(temp, Type::OPERATOR, 2));
                    break;
                case '(':
                    tokens.push_back(Token(temp, Type::LPAR));
                    break;
                case ')':
                    tokens.push_back(Token(temp, Type::RPAR));
                    break;
                default:
                    break;
                }
            }
        }

        std::deque<Token> queue;
        std::vector<Token> stack;
        for(int i = 0; i < tokens.size(); i++){
            if(tokens[i].type == Type::NUMBER){
                queue.push_back(tokens[i]);
            } else if (tokens[i].type == Type::OPERATOR){
                while(stack.size() > 0 && stack[stack.size()-1].type != Type::LPAR && stack[stack.size()-1].precedence >= tokens[i].precedence){
                    queue.push_back(stack[stack.size()-1]);
                    stack.pop_back();
                }
                stack.push_back(tokens[i]);
            } else if (tokens[i].type == Type::LPAR){
                stack.push_back(tokens[i]);
            } else if (tokens[i].type == Type::RPAR){
                Token o2 = stack[stack.size()-1];
                while(o2.type != Type::LPAR){
                    if(stack.size() == 0) throw std::runtime_error("Mismatched Parenthesis");
                    queue.push_back(o2);
                    stack.pop_back();
                    o2 = stack[stack.size()-1];
                }
                if(stack[stack.size()-1].type != Type::LPAR) throw std::runtime_error("Mismatched Parenthesis");
                stack.pop_back();
            }
        }

        while(stack.size() > 0){
            if(stack[stack.size()-1].type == Type::LPAR) throw std::runtime_error("Mismatched Parenthesis");
            queue.push_back(stack[stack.size()-1]);
            stack.pop_back();
        }

        for(int i = 0; i < queue.size(); i++){
            std::cout << queue[i].content << std::endl;
        }

        float result = parse(queue);
        std::cout << "result " << result << std::endl;
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }
}