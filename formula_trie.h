#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>
#include <vector>

class Node {
private:
    char symbol_;
    int index_;
    std::vector<int> support_;
    std::unordered_map<char, std::shared_ptr<Node>> children_;
public:
    Node(char symbol);
    char getSymbol() const;
    int getIndex() const;
    std::vector<int> getSupport() const;
    void setIndex(int index);
    void support(std::vector<int> support);
    std::shared_ptr<Node> addChild(std::shared_ptr<Node> child);
    std::shared_ptr<Node> findChild(char symbol);
};

class FormulaTrie {
private:
    std::shared_ptr<Node> root = std::make_shared<Node>('#');
    std::shared_ptr<Node> find(const std::string& expr) const;
public:
    FormulaTrie();
    void insert(const std::string& expr, int index = 0, std::vector<int> support = {});
    int contains(const std::string& expr) const;
    std::vector<int> getSupport(const std::string& expr) const;
};
