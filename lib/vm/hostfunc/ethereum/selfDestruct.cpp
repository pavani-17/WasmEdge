// SPDX-License-Identifier: Apache-2.0
#include "vm/hostfunc/ethereum/selfDestruct.h"

namespace SSVM {
namespace Executor {

ErrCode EEISelfDestruct::body(VM::EnvironmentManager &EnvMgr,
                              Instance::MemoryInstance &MemInst,
                              uint64_t AddressOffset) {
  evmc_context *Cxt = Env.getEVMCContext();

  /// Get address data.
  evmc::address Addr, Self;
  MemInst.getArray(Addr.bytes, AddressOffset, 20);
  std::memcpy(Self.bytes, &Env.getAddress()[0], 20);

  /// Take additional gas if call new account.
  if (!Cxt->host->account_exists(Cxt, &Addr)) {
    if (!EnvMgr.addCost(25000ULL)) {
      return ErrCode::Revert;
    }
  }

  /// Call selfdestruct.
  Cxt->host->selfdestruct(Cxt, &Self, &Addr);
  return ErrCode::Terminated;
}

} // namespace Executor
} // namespace SSVM
