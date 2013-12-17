#ifndef _TASK4_6_SOLUTION_H_
#define _TASK4_6_SOLUTION_H_

#include <map>
#include <vector>
#include <iostream>
#include <stack>
#include <string>
#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/noncopyable.hpp>

using boost::uint32_t;
using boost::uint8_t;

namespace task4_6
{
    class Operand;
    class Operator;
    class Token;
    class Variable;
    class Calc;

    typedef boost::shared_ptr<Variable> VariablePtr;
    typedef boost::shared_ptr<Operand> OperandPtr;
    typedef boost::shared_ptr<Operator> OperatorPtr;
    typedef boost::shared_ptr<Token> TokPtr;
    typedef std::vector< std::string > lines;

    using boost::uint32_t;
    using boost::uint8_t;

    class Token: virtual protected boost::noncopyable
    {
        public:
            Token(Calc& calc) : calc_ ( calc ) {}
            virtual void eval() = 0;
            virtual void add(TokPtr) = 0;
            virtual std::string what() const = 0;
            static boost::shared_ptr<Token> createToken(Calc&, std::string const&);
            friend std::ostream& operator<<(std::ostream& os, Token const& o);
        protected:
            Calc& calc_;
    };

    class LeftParens: public Token
    {
        public:
            LeftParens(Calc& calc) : Token(calc){}
            void add(TokPtr);
            void eval(){}
            std::string what() const{return "(";}
    };

    class RightParens: public Token
    {
        public:
            RightParens(Calc& calc) : Token(calc){}
            void add(TokPtr);
            void eval(){}
            std::string what() const{return ")";}
    };

    class Operand: public Token
    { 
        public:
            explicit Operand(Calc& calc_, float value) : Token( calc_ ), value_ ( value ) { }
            void add(TokPtr);
            void eval();
            friend std::ostream& operator<<(std::ostream& os, Operand const& o);
            OperandPtr operator+(Operand const& o)
            {
                return OperandPtr(new Operand(calc_, this->value() + o.value()));
            }
            OperandPtr operator-(Operand const& o)
            {
                return OperandPtr(new Operand(calc_, this->value() - o.value()));
            }
            OperandPtr operator*(Operand const& o)
            {			
                return OperandPtr(new Operand(calc_, this->value() * o.value() ));
            }
            OperandPtr operator/(Operand const& o)
            {
                if(o.value() == 0)
                    throw std::logic_error("Division by zero");
                return OperandPtr(new Operand(calc_, this->value() / o.value()));
            }
            std::string what() const
            {
                std::stringstream ss;
                ss << value_;
                return ss.str();
            } 
            virtual float value() const { return value_; }
        protected: 
            void value(float const value) { value_ = value; }
            float value_;
    };

    class Variable: public Operand
    {
        public:
            explicit Variable(Calc& calc, std::string const& name): Operand( calc, 0u ), name_(name) {}
            void add(TokPtr);
            float value() const;	
            std::string what() const{return name_;} 
            std::string name() const { return name_; }		
        private:
            std::string name_;
    };

    class Operator: public Token
    {
        public:
            Operator(Calc& calc) : Token(calc){}
            void add(TokPtr);
            void eval();
            virtual TokPtr apply(OperandPtr op1, OperandPtr op2) = 0; 
            virtual bool operator>(Operator const& op) const;
            virtual uint8_t priority() const = 0;
            virtual uint8_t lvl() const { return lvl_; }
            virtual void lvl(uint8_t l) { lvl_ = l; }
        private:
            uint8_t lvl_;
    };

    class OpPlus: public Operator
    {
        public:
            OpPlus(Calc& calc) : Operator(calc){}
            TokPtr apply(OperandPtr op1, OperandPtr op2);		
            virtual uint8_t priority() const{return 1u;
            }
            friend std::ostream& operator<<(std::ostream&, OpPlus const&); 
            std::string what() const{return "+";} 
        private:
    };

    class OpMinus: public Operator
    {
        public: 
            OpMinus(Calc& calc) : Operator(calc){}
            TokPtr apply(OperandPtr op1, OperandPtr op2);		
            uint8_t priority() const{return 1u;}
            friend std::ostream& operator<<(std::ostream&, OpPlus const&);
            std::string what() const{return "-";} 
        private:
    };

    class OpMultiply: public Operator
    {
        public:
            OpMultiply(Calc& calc) : Operator(calc){}
            TokPtr apply(OperandPtr op1, OperandPtr op2);		
            uint8_t priority() const{return 2u;}
            friend std::ostream& operator<<(std::ostream&, OpPlus const&); 
            std::string what() const{return "*";} 
        private:
    };

    class OpDivide: public Operator
    {
        public: 
            OpDivide(Calc& calc) : Operator(calc){}
            TokPtr apply(OperandPtr op1, OperandPtr op2);		
            uint8_t priority() const{return 2u;}
            friend std::ostream& operator<<(std::ostream&, OpPlus const&); 
            std::string what() const{return "/";} 
        private:
    }; 
    class OpAssign: public Operator
    {
        public: 
            OpAssign(Calc& calc) : Operator(calc){}	
            void eval();
            TokPtr apply(OperandPtr op1, OperandPtr op2){return TokPtr(reinterpret_cast<Token*>(0));}
            uint8_t priority() const
            {
                return 0;
            }
            friend std::ostream& operator<<(std::ostream&, OpPlus const&); 
            std::string what() const
            {
                return "=";
            } 
        private:
    };	

    class Calc
    { 
        public:
            Calc(std::map<std::string, float>& vars) : level ( 0u ), variables_(vars) {}	
            std::stack<TokPtr> stack_;						
            std::vector<TokPtr> v;
            TokPtr current;
            std::stack<OperatorPtr> oper;
            uint8_t level;
            std::map<std::string, float>& variables_;

            void eval();
            friend std::ostream& operator<<(std::ostream& os, Calc const& calc);
    };

    class solution
    {
        std::map<std::string, float> variables_;
        public:
        explicit solution( const lines& calulator_lines );
        int result_for( const std::string& key ) const;
    };
}

#endif // _TASK4_5_SOLUTION_H_ 

