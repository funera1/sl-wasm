#include <wasmedge/wasmedge.h>
#include <stdio.h>

/* This function can add 2 i32 values and return the result. */
WasmEdge_Result __syscall_faccessat(void *, const WasmEdge_CallingFrameContext *,
                    const WasmEdge_Value *In, WasmEdge_Value *Out) {
  /*
  * Params: {i32, i32, i32, i32}
  * Returns: {i32}
  */

  // 無条件で1を返す.
  printf("Called __syscall_faccessat\n");
  Out[0] = WasmEdge_ValueGenI32(1);
  return WasmEdge_Result_Success;
}

WasmEdge_Result __syscall__newselect(void *, const WasmEdge_CallingFrameContext *,
                    const WasmEdge_Value *In, WasmEdge_Value *Out) {
  /*
  * Params: {i32, i32, i32, i32, i32}
  * Returns: {i32}
  */

  // 無条件で1を返す.
  printf("Called __syscall__newselect\n");
  Out[0] = WasmEdge_ValueGenI32(1);
  return WasmEdge_Result_Success;
}

/// @brief  __syscall_faccesseatのホスト関数インスタンスを作成
/// @return 
WasmEdge_FunctionInstanceContext *HostFaccessAt() {
  enum WasmEdge_ValType ParamList[4] = {WasmEdge_ValType_I32,
                                  WasmEdge_ValType_I32,
                                  WasmEdge_ValType_I32,
                                  WasmEdge_ValType_I32};
  enum WasmEdge_ValType ReturnList[1] = {WasmEdge_ValType_I32};


  // Decralation function type
  WasmEdge_FunctionTypeContext *HostFType = 
      WasmEdge_FunctionTypeCreate(ParamList, 4, ReturnList, 1);


  // Add function
  WasmEdge_FunctionInstanceContext *HostFunc =
      WasmEdge_FunctionInstanceCreate(HostFType, __syscall_faccessat, NULL, 0);


  WasmEdge_FunctionTypeDelete(HostFType);
  return HostFunc;
}


/// @brief  __syscall__newselectのホスト関数インスタンスを作成
/// @return 
WasmEdge_FunctionInstanceContext *HostNewSelect() {
  enum WasmEdge_ValType ParamList[5] = {WasmEdge_ValType_I32,
                                  WasmEdge_ValType_I32,
                                  WasmEdge_ValType_I32,
                                  WasmEdge_ValType_I32,
                                  WasmEdge_ValType_I32};
  enum WasmEdge_ValType ReturnList[1] = {WasmEdge_ValType_I32};


  // Decralation function type
  WasmEdge_FunctionTypeContext *HostFType = 
      WasmEdge_FunctionTypeCreate(ParamList, 5, ReturnList, 1);


  // Add function
  WasmEdge_FunctionInstanceContext *HostFunc =
      WasmEdge_FunctionInstanceCreate(HostFType, __syscall__newselect, NULL, 0);


  WasmEdge_FunctionTypeDelete(HostFType);
  return HostFunc;
}


/// @brief ホストモジュール作成
/// @return 
WasmEdge_ModuleInstanceContext *CreateEnvModule() {
  // Create the module instnace
  WasmEdge_String HostName = WasmEdge_StringCreateByCString("env");
  WasmEdge_ModuleInstanceContext *HostMod = WasmEdge_ModuleInstanceCreate(HostName);
  WasmEdge_StringDelete(HostName);

  // Add the host function
  HostName = WasmEdge_StringCreateByCString("__syscall_faccessat");
  WasmEdge_ModuleInstanceAddFunction(HostMod, HostName, HostFaccessAt());
  WasmEdge_StringDelete(HostName);

  HostName = WasmEdge_StringCreateByCString("__syscall__newselect");
  WasmEdge_ModuleInstanceAddFunction(HostMod, HostName, HostNewSelect());
  WasmEdge_StringDelete(HostName);

  return HostMod;
}

WasmEdge_StoreContext *ImportHostModule() {
  WasmEdge_StoreContext *StoreCxt = WasmEdge_StoreCreate();
  WasmEdge_ExecutorContext *ExecCxt = WasmEdge_ExecutorCreate(NULL, NULL);


  // Envモジュールの作成＆登録
  WasmEdge_ModuleInstanceContext *HostModCxt = CreateEnvModule();
  WasmEdge_Result Res = 
    WasmEdge_ExecutorRegisterImport(ExecCxt, StoreCxt, HostModCxt);
  if (!WasmEdge_ResultOK(Res)) {
    printf("Host module registration failed: %s\n", WasmEdge_ResultGetMessage(Res));
    return NULL;
  }

  // Wasiモジュールの作成＆登録
  WasmEdge_ModuleInstanceContext *WasiModCxt = WasmEdge_ModuleInstanceCreateWASI(
    NULL, 0, NULL, 0, NULL, 0
  );
  Res = WasmEdge_ExecutorRegisterImport(ExecCxt, StoreCxt, WasiModCxt);
  if (!WasmEdge_ResultOK(Res)) {
    printf("Host module registration failed: %s\n", WasmEdge_ResultGetMessage(Res));
    return NULL;
  }

  return StoreCxt;
}


/// @brief restore modeの設定をする
/// @param ConfCxt 
/// @return 
bool setupConfigure(struct WasmEdge_ConfigureContext *ConfCxt) {
  // ConfCxt->Conf.getStatisticsConfigure().setRestoreFlag(true);
  // WasmEdge_ConfigureStatisticsSetRestoreFlag(ConfCxt, true);
  // WasmEdge_ConfigureAddHostRegistration(ConfCxt, WasmEdge_HostRegistration_Wasi);
}

/// @brief ファイルから実行
/// @param VMCxt 
/// @return 
int RunFromFile(WasmEdge_VMContext *VMCxt) {
  WasmEdge_Result Res;

  /* Step 1: Load WASM file. */
  Res = WasmEdge_VMLoadWasmFromFile(VMCxt, "../sl.wasm");
  if (!WasmEdge_ResultOK(Res)) {
    printf("Loading phase failed: %s\n", WasmEdge_ResultGetMessage(Res));
    return 1;
  }

  /* Step 2: Validate the WASM module. */
  Res = WasmEdge_VMValidate(VMCxt);
  if (!WasmEdge_ResultOK(Res)) {
    printf("Validation phase failed: %s\n", WasmEdge_ResultGetMessage(Res));
    return 1;
  }

  /* Step 3: Instantiate the WASM module. */
  Res = WasmEdge_VMInstantiate(VMCxt);
  /*
   * Developers can load, validate, and instantiate another WASM module to
   * replace the instantiated one. In this case, the old module will be
   * cleared, but the registered modules are still kept.
   */
  if (!WasmEdge_ResultOK(Res)) {
    printf("Instantiation phase failed: %s\n",
           WasmEdge_ResultGetMessage(Res));
    return 1;
  }


  /*
   * Step 4: Execute WASM functions. You can execute functions repeatedly
   * after instantiation.
   */
  WasmEdge_LogSetDebugLevel();
  WasmEdge_Value Params[1];
  WasmEdge_Value Returns[1];
  WasmEdge_String FuncName = WasmEdge_StringCreateByCString("_start");
  Res = WasmEdge_VMExecute(VMCxt, FuncName, Params, 0, Returns, 0);
  if (WasmEdge_ResultOK(Res)) {
    // printf("Get the result: %d\n", WasmEdge_ValueGetI32(Returns[0]));
    printf("Succeed\n");
  } else {
    printf("Execution phase failed: %s\n", WasmEdge_ResultGetMessage(Res));
  }

  WasmEdge_StringDelete(FuncName);
}


int main(int Argc, const char* Argv[]) {
  /* Create the configure context and add the WASI support. */
  /* This step is not necessary unless you need WASI support. */
  
  // ConfCxtの中にDumpFlagやRestoreFlagがある
  WasmEdge_ConfigureContext *ConfCxt = WasmEdge_ConfigureCreate();
  setupConfigure(ConfCxt);

  // Storeを作成し、ホストモジュールを登録
  WasmEdge_StoreContext *StoreCxt = ImportHostModule();
  if (!StoreCxt) {
    return -1;
  }

  /* The configure and store context to the VM creation can be NULL. */
  WasmEdge_VMContext *VMCxt = WasmEdge_VMCreate(ConfCxt, StoreCxt);
  RunFromFile(VMCxt);

  /* Resources deallocations. */
  WasmEdge_VMDelete(VMCxt);
  WasmEdge_ConfigureDelete(ConfCxt);
  return 0;
}
