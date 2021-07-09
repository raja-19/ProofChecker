#include "wff.h"
#include "formula_trie.h"

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>

class ProofChecker{
private:
    FormulaTrie axioms_;
    FormulaTrie theorems_;
    FormulaTrie precedents_;
    FormulaTrie consequents_;
    std::vector<Formula> formulas_;

    std::string axiomErrorMessage(const std::string& axiom);
    void process(const Formula& formula, int index);
    void processImplication(const Formula& formula, int index);
    bool obtainAxiom(const Formula& formula, int index);
    bool obtainConsequent(const Formula& formula, int index);
public:
    ProofChecker(const std::string& axioms_file);
    void checkProof(const std::string& proof_file);
};

