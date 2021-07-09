#include "proof_checker.h"

using namespace std;

string ProofChecker::axiomErrorMessage(const string& axiom) {
    stringstream error_msg;
    error_msg << "ERROR: Axiom \"" << axiom << "\" is not a well formed formula";
    return error_msg.str();
}

void ProofChecker::proccess(const Formula& formula, int index) {
    theorems_.insert(formula.expr, index);
    for (int support_index: precedents_.getSupport(formula.expr)) {
        consequents_.insert(formulas_[support_index - 1].constituents[1], 0, {support_index, index});
    }
}

void ProofChecker::proccessImplication(const Formula& formula, int index) {
    if (formula.isImplication()) {
        precedents_.insert(formula.constituents[0], 0, {index});
        int theorem_index = theorems_.contains(formula.constituents[0]);
        if (theorem_index) {
            consequents_.insert(formula.constituents[1], 0, {index, theorem_index});
        } 
    }
}

bool ProofChecker::obtainAxiom(const Formula& formula, int index) {
    int axiom_index = axioms_.contains(formula.expr);  
    if (axiom_index) {
        cout << index << ". " << formula.expr << " " << "Axiom " << axiom_index << endl; 
        proccess(formula, index);
        proccessImplication(formula, index);
        return true;
    }
    return false;
}

bool ProofChecker::obtainConsequent(const Formula& formula, int index) {
    vector<int> support = consequents_.getSupport(formula.expr);  
    if (support.size() > 1) {
        cout << index << ". " << formula.expr << " " << "MP " << support[0] << ", " << support[1] << endl; 
        proccess(formula, index);
        proccessImplication(formula, index);
        return true;
    }
    return false;
}

ProofChecker::ProofChecker(const string& axioms_file) {
    fstream axioms_ss(axioms_file);
    int index = 0;
    string axiom;
    while(axioms_ss >> axiom) {
        if (isWellFormedFormula(axiom)) {
            axioms_.insert(axiom, ++index);
        } else {
            throw logic_error(axiomErrorMessage(axiom)); 
        }
    }   
}

void ProofChecker::checkProof(const string& proof_file) {
    fstream proof_ss(proof_file);
    string expr;
    while(proof_ss >> expr) {
        Formula formula = parseExpression(expr);
        if (!formula.isWellFormed()) {
            cout  << "Invalid Proof\"" << expr << "\" is not a well formed formula" << endl;
            return;
        }
        formulas_.push_back(formula);
        if (!obtainAxiom(formula, formulas_.size())) {
            if(!obtainConsequent(formula, formulas_.size())) {
                cout << "Invalid Proof \"" << expr << "\" cannot be deduced" << endl;
                return;
            } 
        }
    }
}

int main() {
    ProofChecker checker("axioms.txt");
    checker.checkProof("proof.txt");
    return 0;
}
