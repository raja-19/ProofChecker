#include "wff.h"

using namespace std;

const char LEFT_PARENTHESIS = '(';
const char RIGHT_PARENTHESIS = ')';
const char NEGATION_SYMBOL = '!';
const char DISJUNCTION_SYMBOL = 'v';
const char CONJUNCTION_SYMBOL = '&';
const char IMPLICATION_SYMBOL = '<';
const char EQUIVALENCE_SYMBOL = '~';

const vector<char> BINARY_CONNECTIVES = {DISJUNCTION_SYMBOL, CONJUNCTION_SYMBOL, IMPLICATION_SYMBOL, EQUIVALENCE_SYMBOL};

const map<char, Type> CONNECTIVE_TO_TYPE = {
    {NEGATION_SYMBOL, Type::NEGATION},
    {DISJUNCTION_SYMBOL, Type::DISJUNCTION},
    {CONJUNCTION_SYMBOL, Type::CONJUNCTION},
    {IMPLICATION_SYMBOL, Type::IMPLICATION},
    {EQUIVALENCE_SYMBOL, Type::EQUIVALENCE},
};

bool Formula::isImplication() const {
   return type == Type::IMPLICATION; 
}

bool Formula::isWellFormed() const {
    return type != Type::ILL_FORMED;
}

bool inRange(int index, const string& expr) {
    return (0 <= index) && index < expr.size();
} 

bool isBinaryConnective(char c) {
    for (char connective: BINARY_CONNECTIVES) {
        if (c == connective) {
            return true;
        }
    }
    return false;
}

bool isSentenceSymbol(const string& expr, int begin, int end) {
    if (end == begin + 1) {
        if ('A' <= expr[begin] && expr[begin] <= 'Z') {
            return true;
        }
    }
    return false;
}

int findEnd(const string& expr, int begin) {
    int weight = 0;
    for (int i = begin; i < expr.size() && 0 <= weight; i++) {
        if (expr[i] == LEFT_PARENTHESIS) {
            weight++;
        } else if (expr[i] == RIGHT_PARENTHESIS) {
            weight--;
        }
        if (weight == 0) {
            return i + 1;
        }
    }
    return begin;
}

bool isWellFormedNegation(const string& expr, int begin, int end) {
    if (inRange(begin, expr) && expr[begin] == LEFT_PARENTHESIS) {
        if (inRange(begin + 1, expr) && expr[begin + 1] == NEGATION_SYMBOL) {
            if (inRange(end - 1, expr) && expr[end - 1] == RIGHT_PARENTHESIS) {
                int begin_alpha = begin + 2;
                int end_alpha = end - 1;
                return isWellFormedSubFormula(expr, begin_alpha, end_alpha);
            }
        }
    }
    return false;
}

bool isWellFormedNonNegation(const string& expr, int begin, int end) {
    if (inRange(begin, expr) && expr[begin] == LEFT_PARENTHESIS) {
        if (inRange(end - 1, expr) && expr[end - 1] == RIGHT_PARENTHESIS) {
            int begin_alpha = begin + 1;
            int end_alpha = findEnd(expr, begin_alpha);
            if (inRange(end_alpha, expr) && isBinaryConnective(expr[end_alpha])) {
                int begin_beta = end_alpha + 1;
                int end_beta = end - 1;
                return isWellFormedSubFormula(expr, begin_alpha, end_alpha) && isWellFormedSubFormula(expr, begin_beta, end_beta);
            }
        }
    }
    return false;
}

bool isWellFormedSubFormula(const string& expr, int begin, int end) {
    if (isSentenceSymbol(expr, begin, end)) {
        return true;
    }
    if (isWellFormedNegation(expr, begin, end)) {
        return true;
    }
    return isWellFormedNonNegation(expr, begin, end);
} 

bool isWellFormedFormula(const string& expr) {
    return isWellFormedSubFormula(expr, 0, expr.size());
}

Formula parseExpression(const string& expr) {
    if (isSentenceSymbol(expr, 0, expr.size())) {
        return {Type::SENTENCE_SYMBOL, expr}; 
    }
    if (isWellFormedNegation(expr, 0, expr.size())) {
        return {Type::NEGATION, expr, {expr.substr(2, expr.size() - 3)}};
    }
    if (isWellFormedNonNegation(expr, 0, expr.size())) {
        int connective_pos = findEnd(expr, 1);
        string alpha = expr.substr(1, connective_pos - 1);
        string beta = expr.substr(connective_pos + 1, expr.size() - connective_pos - 2);
        return {CONNECTIVE_TO_TYPE.at(expr[connective_pos]), expr, {alpha, beta}};
    }
    return {Type::ILL_FORMED, expr};
}
