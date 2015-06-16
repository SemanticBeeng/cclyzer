#ifndef INSTRUCTION_VISITOR_H__
#define INSTRUCTION_VISITOR_H__

#include <string>

#include <boost/unordered_map.hpp>

#include "llvm/InstVisitor.h"
#include "llvm/IR/Attributes.h"

#include "PredicateNames.hpp"
#include "CsvGenerator.hpp"

class InstructionVisitor : public llvm::InstVisitor<InstructionVisitor>
{
  public:
    InstructionVisitor(CsvGenerator *generator, const llvm::Module *M)
        : csvGen(generator), Mod(M) {}

    /*******************************
     * Instruction Visitor methods *
     *******************************/

    // Binary Operations

    void visitAdd(llvm::BinaryOperator &);
    void visitFAdd(llvm::BinaryOperator &);
    void visitSub(llvm::BinaryOperator &);
    void visitFSub(llvm::BinaryOperator &);
    void visitMul(llvm::BinaryOperator &);
    void visitFMul(llvm::BinaryOperator &);
    void visitSDiv(llvm::BinaryOperator &);
    void visitFDiv(llvm::BinaryOperator &);
    void visitUDiv(llvm::BinaryOperator &);
    void visitSRem(llvm::BinaryOperator &);
    void visitFRem(llvm::BinaryOperator &);
    void visitURem(llvm::BinaryOperator &);

    // Bitwise Binary Operations

    void visitShl(llvm::BinaryOperator &);
    void visitLShr(llvm::BinaryOperator &);
    void visitAShr(llvm::BinaryOperator &);
    void visitAnd(llvm::BinaryOperator &);
    void visitOr(llvm::BinaryOperator &);
    void visitXor(llvm::BinaryOperator &);

    // Conversion Operations

    template<typename T>
    void writeCastInst(llvm::CastInst &instr)
    {
        typedef T pred;
        using namespace auxiliary_methods;

        csvGen->writeEntity(pred::instr, instrNum);
        writeOperand(pred::from_operand, instr.getOperand(0));

        csvGen->writeSimpleFact(pred::to_type, instrNum, printType(instr.getType()));
    }

    template<typename T>
    void writeBinaryInst(llvm::BinaryOperator &instr)
    {
        typedef T pred;
        using namespace auxiliary_methods;

        writeOptimizationInfoToFile(&instr, instrNum);
        csvGen->writeEntity(pred::instr, instrNum);

        // Record left operand of binary operator
        writeOperand(pred::first_operand, instr.getOperand(0));

        // Record right operand of binary operator
        writeOperand(pred::second_operand, instr.getOperand(1));
    }


    void visitTruncInst(llvm::TruncInst &);
    void visitZExtInst(llvm::ZExtInst &);
    void visitSExtInst(llvm::SExtInst &);
    void visitFPTruncInst(llvm::FPTruncInst &);
    void visitFPExtInst(llvm::FPExtInst &);
    void visitFPToUIInst(llvm::FPToUIInst &);
    void visitFPToSIInst(llvm::FPToSIInst &);
    void visitUIToFPInst(llvm::UIToFPInst &);
    void visitSIToFPInst(llvm::SIToFPInst &);
    void visitPtrToIntInst(llvm::PtrToIntInst &);
    void visitIntToPtrInst(llvm::IntToPtrInst &);
    void visitBitCastInst(llvm::BitCastInst &);

    // Terminator Instructions

    void visitReturnInst(llvm::ReturnInst &);
    void visitBranchInst(llvm::BranchInst &);
    void visitSwitchInst(const llvm::SwitchInst &);
    void visitIndirectBrInst(llvm::IndirectBrInst &);
    void visitInvokeInst(llvm::InvokeInst &);
    void visitResumeInst(llvm::ResumeInst &);
    void visitUnreachableInst(llvm::UnreachableInst &);

    // Aggregate Operations

    void visitInsertValueInst(llvm::InsertValueInst &);
    void visitExtractValueInst(llvm::ExtractValueInst &);

    // Memory Operations

    void visitAllocaInst(llvm::AllocaInst &);
    void visitLoadInst(llvm::LoadInst &);
    void visitStoreInst(llvm::StoreInst &);
    void visitAtomicCmpXchgInst(llvm::AtomicCmpXchgInst &);
    void visitAtomicRMWInst(llvm::AtomicRMWInst &);
    void visitFenceInst(llvm::FenceInst &);
    void visitGetElementPtrInst(llvm::GetElementPtrInst &);

    // Other

    void visitICmpInst(llvm::ICmpInst &);
    void visitFCmpInst(llvm::FCmpInst &);
    void visitPHINode(llvm::PHINode &);
    void visitSelectInst(llvm::SelectInst &);
    void visitLandingPadInst(llvm::LandingPadInst &);
    void visitCallInst(llvm::CallInst &);
    void visitVAArgInst(llvm::VAArgInst &);

    // Vector Operations

    void visitExtractElementInst(llvm::ExtractElementInst &);
    void visitInsertElementInst(llvm::InsertElementInst &);
    void visitShuffleVectorInst(llvm::ShuffleVectorInst &);

    // 'default' case
    void visitInstruction(llvm::Instruction &I);

    void setInstrNum(std::string instructionNum) {
        instrNum = instructionNum;
        immediateOffset = 0;
    }
    void setInstrId(std::string instructionId) {

        instrId = instructionId;
    }

private:
    typedef predicate_names::pred_t pred_t;

    // Auxiliary methods
    void writeValue(pred_t predicate, const llvm::Value *Value, int index = -1);
    void writeOperand(pred_t predicate, const llvm::Value *Operand, int index = -1);

    const char* writePredicate(unsigned predicate);
    void writeOptimizationInfoToFile(const llvm::User *u, std::string instrId);
    const char *writeAtomicInfo(std::string instrId, llvm::AtomicOrdering order, llvm::SynchronizationScope synchScope);
    void writeAtomicRMWOp(std::string instrId, llvm::AtomicRMWInst::BinOp op);

    void writeVolatileFlag(std::string instrId, bool volatileFlag) {
        if (volatileFlag)
            csvGen->writeSimpleFact(predicate_names::instruction::flag, instrId, "volatile");
    }

    std::string instrNum;
    std::string instrId;
    int immediateOffset;
    CsvGenerator *csvGen;
    const llvm::Module *Mod;
};

#endif
