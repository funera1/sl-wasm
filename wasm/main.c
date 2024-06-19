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
  const char *const Envs[] = {
  "TERMINFO=b64:HgIlAB0ADwBxAVQGeHRlcm0tMjU2Y29sb3J8eHRlcm0gd2l0aCAyNTYgY29sb3JzAAABAAABAAAAAQAAAAABAQAAAAAAAAABAAABAAEBUAAAAAgAAAAYAAAA_____________________________________________________wABAAAAAAEAAAAEAAYACAAZAB4AJgAqAC4A__85AEoATABQAFcA__9ZAGYA__9qAG4AeAB8AP____-AAIQAiQCOAP__oAClAKoA__-vALQAuQC-AMcAywDSAP__5ADpAO8A9QD_______8HAf_______xkB__8dAf_______x8B__8kAf__________KAEsATIBNgE6AT4BRAFKAVABVgFcAWAB__9lAf__aQFuAXMBdwF-Af__hQGJAZEB_____________________________5kBogGrAf__rgG3AcAByQHSAdsB5AHtAfYB_wH_______8IAgwCEQIWAioCMwL_____RQJIAlMCVgJYAlsCuAL__7sC__________-9AsECxQLJAs0C_____9EC__8GA_____8KAxAD_____________xYD_____________xoDHgP__________________________________________________________________yID_____ykD__________8wAzcDPgP_____RQP__0wD________UwP_____________WgNgA2YDbQN0A3sDggOKA5IDmgOiA6oDsgO6A8IDyQPQA9cD3gPmA-4D9gP-AwYEDgQWBB4EJQQsBDMEOgRCBEoEUgRaBGIEagRyBHoEgQSIBI8ElgSeBKYErgS2BL4ExgTOBNYE3QTkBOsE8AT___________________________________________________________cEAgUHBRoFHgUnBS4F_____________________________4wF________________________kQX_____________________________________________________lwWoBf____________________________-6Bf_______74F_QX__________________z0GG1taAAcADQAbWyVpJXAxJWQ7JXAyJWRyABtbM2cAG1tIG1sySgAbW0sAG1tKABtbJWklcDElZEcAG1slaSVwMSVkOyVwMiVkSAAKABtbSAAbWz8yNWwACAAbWz8xMmwbWz8yNWgAG1tDABtbQQAbWz8xMjsyNWgAG1tQABtbTQAbKDAAG1s1bQAbWzFtABtbPzEwNDloG1syMjswOzB0ABtbMm0AG1s0aAAbWzhtABtbN20AG1s3bQAbWzRtABtbJXAxJWRYABsoQgAbKEIbW20AG1s_MTA0OWwbWzIzOzA7MHQAG1s0bAAbWzI3bQAbWzI0bQAbWz81aCQ8MTAwLz4bWz81bAAbWyFwG1s_Mzs0bBtbNGwbPgAbW0wAfwAbWzN-ABtPQgAbT1AAG1syMX4AG09RABtPUgAbT1MAG1sxNX4AG1sxN34AG1sxOH4AG1sxOX4AG1syMH4AG09IABtbMn4AG09EABtbNn4AG1s1fgAbT0MAG1sxOzJCABtbMTsyQQAbT0EAG1s_MWwbPgAbWz8xaBs9ABtbPzEwMzRsABtbPzEwMzRoABtFABtbJXAxJWRQABtbJXAxJWRNABtbJXAxJWRCABtbJXAxJWRAABtbJXAxJWRTABtbJXAxJWRMABtbJXAxJWREABtbJXAxJWRDABtbJXAxJWRUABtbJXAxJWRBABtbaQAbWzRpABtbNWkAJXAxJWMbWyVwMiV7MX0lLSVkYgAbYxtdMTA0BwAbWyFwG1s_Mzs0bBtbNGwbPgAbOAAbWyVpJXAxJWRkABs3AAoAG00AJT8lcDkldBsoMCVlGyhCJTsbWzAlPyVwNiV0OzElOyU_JXA1JXQ7MiU7JT8lcDIldDs0JTslPyVwMSVwMyV8JXQ7NyU7JT8lcDQldDs1JTslPyVwNyV0OzglO20AG0gACQAbT3cAG095ABtPdQAbT3EAG09zAGBgYWFmZmdnaWlqamtrbGxtbW5ub29wcHFxcnJzc3R0dXV2dnd3eHh5eXp6e3t8fH19fn4AG1taABtbPzdoABtbPzdsABtPRQAbT0YAG09NABtbMzsyfgAbWzE7MkYAG1sxOzJIABtbMjsyfgAbWzE7MkQAG1s2OzJ-ABtbNTsyfgAbWzE7MkMAG1syM34AG1syNH4AG1sxOzJQABtbMTsyUQAbWzE7MlIAG1sxOzJTABtbMTU7Mn4AG1sxNzsyfgAbWzE4OzJ-ABtbMTk7Mn4AG1syMDsyfgAbWzIxOzJ-ABtbMjM7Mn4AG1syNDsyfgAbWzE7NVAAG1sxOzVRABtbMTs1UgAbWzE7NVMAG1sxNTs1fgAbWzE3OzV-ABtbMTg7NX4AG1sxOTs1fgAbWzIwOzV-ABtbMjE7NX4AG1syMzs1fgAbWzI0OzV-ABtbMTs2UAAbWzE7NlEAG1sxOzZSABtbMTs2UwAbWzE1OzZ-ABtbMTc7Nn4AG1sxODs2fgAbWzE5OzZ-ABtbMjA7Nn4AG1syMTs2fgAbWzIzOzZ-ABtbMjQ7Nn4AG1sxOzNQABtbMTszUQAbWzE7M1IAG1sxOzNTABtbMTU7M34AG1sxNzszfgAbWzE4OzN-ABtbMTk7M34AG1syMDszfgAbWzIxOzN-ABtbMjM7M34AG1syNDszfgAbWzE7NFAAG1sxOzRRABtbMTs0UgAbWzFLABtbPzY5bAAbWyVpJWQ7JWRSABtbNm4AG1s_JVs7MDEyMzQ1Njc4OV1jABtbYwAbWzM5OzQ5bQAbXTEwNAcAG100OyVwMSVkO3JnYjolcDIlezI1NX0lKiV7MTAwMH0lLyUyLjJYLyVwMyV7MjU1fSUqJXsxMDAwfSUvJTIuMlgvJXA0JXsyNTV9JSolezEwMDB9JS8lMi4yWBtcABtbM20AG1syM20AG1s_NjloG1slaSVwMSVkcwAbWz82OWgbWyVpOyVwMSVkcwAbWzwAG1slPyVwMSV7OH0lPCV0MyVwMSVkJWUlcDElezE2fSU8JXQ5JXAxJXs4fSUtJWQlZTM4OzU7JXAxJWQlO20AG1slPyVwMSV7OH0lPCV0NCVwMSVkJWUlcDElezE2fSU8JXQxMCVwMSV7OH0lLSVkJWU0ODs1OyVwMSVkJTttABtbPzY5aBtbJWklcDElZDslcDIlZHMA"
  };
  WasmEdge_ModuleInstanceContext *WasiModCxt = WasmEdge_ModuleInstanceCreateWASI(
    NULL, 0, 
    Envs, 1, 
    NULL, 0
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
