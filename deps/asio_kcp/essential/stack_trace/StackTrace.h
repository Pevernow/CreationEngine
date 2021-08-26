#pragma once

#include "../../src/general/stack_trace/FuncNameRegisterStub.h"
#include "../../src/general/stack_trace/StackTraceStub.h"
#include "../../src/general/stack_trace/FunctionParam.h"
#include "StackTraceIns.h"


#include "../config.h"
#include "../compile_config.h"


#ifdef ENABLE_STACK_TRACE
/**
* �����ڴ�����ʹ����STACK_TRACE_BIND_INS����������������������
**/
#define STACK_TRACE																	\
		static StackTrace::FuncNameRegisterStub registerStub(__FUNCTION__);				\
		StackTrace::StackTraceStub traceStub(registerStub.GetFuncIndex());	 		\
		traceStub.SetArgument

/**
* ��һ��Stack���󵽵������Լ���һ��FuncNameMappingTable���󵽵�����
**/
#define STACK_TRACE_BIND_INS( __StackTraceManagerInstance__  )			\
	StackTrace::StackTraceManagerIns::Set( __StackTraceManagerInstance__ );


#else // ENABLE_STACK_TRACE
	#define STACK_TRACE __noop
	#define STACK_TRACE_BIND_INS( __StackTraceManagerInstance__ )
#endif // ENABLE_STACK_TRACE





//1. �ϲ�BIND_STACK_INSTANCE �� BIND_FNMT_INSTANCE  ok
//2. �꿪��. Ҫ���release�£�����ENABLE_STACK_TRACEʱ��������.
//3. ��ֲ��adapter(visitor, world, stub) func(ai)
//4. ȫ�߿��STACK_TRACE


//4.5 ������ENABLE_STACK_TRACEʱ��������
//5. assert_check����δ��ֲStackTraceϵͳ������Ĵ�����Ҫ��������.
//6. ������ֲ˵��.
//7. Զ��Ŀ�꣺ ������STACK_TRACE2 �� STACK_TRACE_INIT2   and 3 and 4 ...
