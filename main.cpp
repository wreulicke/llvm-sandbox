
#include <iostream>
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/raw_ostream.h"

int main()
{
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);
    std::unique_ptr<llvm::Module> module =
        llvm::make_unique<llvm::Module>("top", context);

    llvm::FunctionType *funcType = llvm::FunctionType::get(builder.getVoidTy(), false);
    llvm::Function *mainFunc =
        llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module.get());
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
    builder.SetInsertPoint(entry);

    llvm::Value *hello = builder.CreateGlobalStringPtr("Hello %s!\n");
    llvm::Value *john = builder.CreateGlobalStringPtr("John");

    std::vector<llvm::Type *> putsArgs;
    putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
    llvm::ArrayRef<llvm::Type *> argsRef(putsArgs);

    llvm::FunctionType *putsType =
        llvm::FunctionType::get(builder.getInt32Ty(), argsRef, true);
    llvm::Constant *putsFunc = module->getOrInsertFunction("printf", putsType);

    std::vector<llvm::Value *> args;
    args.push_back(hello);
    args.push_back(john);
    llvm::ArrayRef<llvm::Value *> argumentsRef(args);
    builder.CreateCall(putsFunc, argumentsRef);
    builder.CreateRetVoid();

    bool isBroken = llvm::verifyModule(*module, &llvm::outs());
    if (isBroken)
    {
        return 1;
    }

    module->print(llvm::outs(), nullptr);

    return 0;
}
