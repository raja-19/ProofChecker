#include "formula_trie.h"

using namespace std;

Node::Node(char symbol): symbol_(symbol) {}

char Node::getSymbol() const {
    return symbol_;
}

int Node::getIndex() const {
    return index_;
}

vector<int> Node::getSupport() const {
    return support_;
}

void Node::setIndex(int index) {
    index_ = index;
}

void Node::support(vector<int> support) {
    support_.insert(support_.end(), support.begin(), support.end());
}

shared_ptr<Node> Node::addChild(shared_ptr<Node> child) {
    char symbol = child->getSymbol();
    if (children_.find(symbol) == children_.end()) {
        children_[symbol] = child;
    }
    return children_[symbol];
}

shared_ptr<Node> Node::findChild(char symbol) {
    if (children_.find(symbol) == children_.end()) {
        return nullptr;
    }
    return children_[symbol];
}

FormulaTrie::FormulaTrie() {}

void FormulaTrie::insert(const string& expr, int index, vector<int> support) {
    shared_ptr<Node> current = root;
    for (char symbol: expr) {
        current = current->addChild(make_shared<Node>(symbol));
    }
    current->setIndex(index);
    current->support(support);
}

shared_ptr<Node> FormulaTrie::find(const string& expr) const {
    shared_ptr<Node> current = root;
    int i = 0;
    while(current && i < expr.size()) {
        current = current->findChild(expr[i++]);
    }
    return current;
}

int FormulaTrie::contains(const string& expr) const {
    shared_ptr<Node> node = find(expr);
    if (node) {
        return node->getIndex();
    }    
    return 0;
}

vector<int> FormulaTrie::getSupport(const string& expr) const {
    shared_ptr<Node> node = find(expr);
    if (node) {
        return node->getSupport();
    }    
    return {};
}
