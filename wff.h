#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <map>

enum class Type {
    ILL_FORMED, SENTENCE_SYMBOL, NEGATION, IMPLICATION, CONJUNCTION, DISJUNCTION, EQUIVALENCE
};

struct Formula {
    Type type;
    std::string expr;
    std::vector<std::string> constituents;
    bool isWellFormed() const;
    bool isImplication() const;
};

bool inRange(int index, const std::string& expr);
bool isBinaryConnective(char c);
bool isSentenceSymbol(const std::string& expr, int begin, int end);
int  findEnd(const std::string& expr, int begin);
bool isWellFormedNegation(const std::string& expr, int begin, int end);
bool isWellFormedNonNegation(const std::string& expr, int begin, int end);
bool isWellFormedSubFormula(const std::string& expr, int begin, int end);
bool isWellFormedFormula(const std::string& expr);
Formula parseExpression(const std::string& expr);
