#include <string>
#include <sstream>
#include <stack>
#include <algorithm>
#include <iostream>
#include <cctype>

#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/special_functions/round.hpp>

#include "solution.h" 

namespace task4_6
{
    using boost::dynamic_pointer_cast;	

    void Operand::add(TokPtr next_token)
    {
        calc_.v.push_back(next_token);
    }

    void Variable::add(TokPtr next_token)
    {
        calc_.v.push_back(next_token);
    }

    void Operator::add(TokPtr next_token)
    {
        OperatorPtr next_op = dynamic_pointer_cast<Operator>(next_token);
        next_op->lvl(calc_.level);

        if(!calc_.oper.empty())
        {
            OperatorPtr op_stack = dynamic_pointer_cast<Operator>(calc_.oper.top());
            while( !calc_.oper.empty() && *op_stack > *next_op)
            {
                calc_.v.push_back(op_stack);
                calc_.oper.pop();
                op_stack = dynamic_pointer_cast<Operator>(calc_.oper.top());
            }
        }  
        calc_.oper.push(next_op);
    }

    void LeftParens::add(TokPtr next_token)
    {
        ++calc_.level;
    }

    void RightParens::add(TokPtr next_token)
    {
        if(calc_.level == 0)
            throw std::logic_error("Extra close parens.");

        --calc_.level;
    }

    float Variable::value() const
    {
        std::map<std::string, float>::iterator it = calc_.variables_.find(this->name_);
        if(it == calc_.variables_.end())
        {		
            throw std::logic_error("Variable " + this->what() + " is not found");
        }		
        return (*it).second;
    }	

    void Operator::eval()
    {
        OperandPtr op1 = dynamic_pointer_cast<Operand>(calc_.stack_.top());
        calc_.stack_.pop();
        OperandPtr op2 = dynamic_pointer_cast<Operand>(calc_.stack_.top());
        calc_.stack_.pop();		
        TokPtr res = apply(op2, op1);
#ifndef NDEBUG
        //std::cout << op2->what() << " " << this->what() << " " << op1->what() << " = " << res->what() << std::endl;
#endif
        calc_.stack_.push( res );
    }

    void Operand::eval()
    {
        calc_.stack_.push(calc_.current);
    }

    TokPtr OpPlus::apply(OperandPtr op1, OperandPtr op2)
    {
        return *op1 + *op2;
    }

    TokPtr OpMinus::apply(OperandPtr op1, OperandPtr op2)
    {
        return *op1 - *op2;
    }

    TokPtr OpMultiply::apply(OperandPtr op1, OperandPtr op2)
    {
        return *op1 * *op2;
    }

    TokPtr OpDivide::apply(OperandPtr op1, OperandPtr op2)
    {
        if( op2 == 0 )
            throw std::logic_error("Division by zero");
        return *op1 / *op2;
    }

    void OpAssign::eval()
    {
        OperandPtr op1 = dynamic_pointer_cast<Operand>(calc_.stack_.top());
        calc_.stack_.pop();
        VariablePtr op2 = dynamic_pointer_cast<Variable>(calc_.stack_.top());
        calc_.stack_.pop();
        if(calc_.variables_.count(op2->name()) > 0 )
            throw std::logic_error("Variable " + op2->name() + " redefinition");

        //std::cout << "Assignment:" << op2->name() << " = " << op1->value() << std::endl;
        calc_.variables_[op2->name()] = op1->value();
    }

    TokPtr Token::createToken(Calc& calc, std::string const& str)
    { 
        try
        {
            float val = boost::lexical_cast<float>(str);
            return TokPtr(new Operand( calc, val ) );
        } catch(boost::bad_lexical_cast&)
        { 
            if(str.size() != 1)
                throw std::logic_error("Unknown token " + str);
            char const c = str.at(0);
            switch(c)
            {
                case '+':
                    return TokPtr(new OpPlus(calc));
                case '-':
                    return TokPtr(new OpMinus(calc));
                case '*':
                    return TokPtr(new OpMultiply(calc));
                case '/':
                    return TokPtr(new OpDivide(calc));
                case '=':
                    return TokPtr(new OpAssign(calc));
                case '(':
                    return TokPtr(new LeftParens(calc));
                case ')':
                    return TokPtr(new RightParens(calc));
                default:
                    return VariablePtr(new Variable(calc, str));
            }
        } 
    } 

    bool Operator::operator>(Operator const& op) const
    {
        if(this->lvl() > op.lvl())
            return true;
        if(this->lvl() < op.lvl())
            return false;
        return this->priority() > op.priority();
    } 

    std::ostream& operator<<(std::ostream& os, Token const& o)
    {
        os << " " << o.what() << " ";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, Operand const& o)
    {
        os << " " << o.value_ << " ";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, OpPlus const& plus)
    {
        os << plus.what();
        return os;
    }

    std::ostream& operator<<(std::ostream& os, OpMinus const& minus) 
    {
        os << minus.what();
        return os;
    }
    std::ostream& operator<<(std::ostream& os, OpMultiply const& multiply)
    {
        os << multiply.what();
        return os;
    }
    std::ostream& operator<<(std::ostream& os, OpDivide const& divide)
    {
        os << divide.what();
        return os;
    }

    void Calc::eval()
    { 
        while(!oper.empty())
        {
            v.push_back(oper.top());
            oper.pop();
        }		
        //for(std::vector<TokPtr>::iterator it = v.begin(); it != v.end(); ++it)
        //{
        //	std::cout << (*it)->what() << " ";
        //}
        //std::cout << std::endl;
        for(std::vector<TokPtr>::iterator it = v.begin(); it != v.end(); ++it)
        {
            TokPtr ptr = *it;
            current = ptr;
            ptr->eval();
        }

    }

    std::ostream& operator<<(std::ostream& os, Calc const& calc)
    {
        for(std::vector<TokPtr>::const_iterator it = calc.v.begin(); it != calc.v.end(); ++it)
        {
            os << **it;
        }
        return os;
    }

    solution::solution( const lines& calculator_lines )
    { 
        for(lines::const_iterator it = calculator_lines.begin(); it != calculator_lines.end(); ++it)
        {
            Calc calc(variables_);
            std::string line = *it;
            std::stringstream ss(line);
            while(ss.good())
            { 
                char c = ss.get();
                if(std::isalnum(c) || std::ispunct(c))
                {
                    std::string token;
                    if(std::isdigit(c))
                    {
                        ss.unget();
                        float i;
                        ss >> i;
                        token = boost::lexical_cast<std::string>(i);
                    } else 
                    {
                        token = std::string(1, c);
                    }
                    TokPtr t = boost::shared_ptr<Token>(Token::createToken(calc, token)); 
                    t->add( t );				
                }
            }
            calc.eval();
        }
    }

    int task4_6::solution::result_for( const std::string& key ) const
    {
        return static_cast<int>(boost::math::round(variables_.at(key)));
    }

}
