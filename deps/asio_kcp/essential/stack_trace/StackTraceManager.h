#pragma once
#include <vector>
#include <string>
#include "../../src\general\stack_trace\StackTraceDef.h"

namespace StackTrace
{
	class StackFrame;
	class FunctionParam;
	class FuncNameMappingTable;
	class Stack;
	
	/**
	* ���ڲ�֧�ֶ��߳�ʹ�ã������ܼ���ʹ���ڶ��߳��У�����ʽ�ǣ�ֻ��ע���߳���ʹ�ã�
	* �����߳���ʹ�ò�����
	**/
	class StackTraceManager
	{
	public:
		friend class InterfaceToolsTester;


		StackTraceManager();
		virtual ~StackTraceManager();
		/**
		* ���ù�ע���̣߳�ע������ֻ֧�ֵ��߳�
		**/
		void SetThreadId( unsigned long threadId );
		const unsigned long& GetThreadId() const;

		/**
		* StackTrace�Ƿ���Ч
		**/
		static bool IsStackTraceCanUse();
		
		/**
		* ��ȡstackFrame info
		**/
		StackFrameInfo_t GetStackRecords( void );

	public:
		size_t FrameSize(void) const;
		const StackFrame& operator[](const size_t& i) const;

		/**
		* ѹ���frameû�в���
		**/
		void PushFrame(const FunctionIndex_t& index);

		/**
		* ����ǰջ֡λ��ѹ�����
		**/
		void PushParamToCurFrame(const FunctionParam& param);


		/**
		* ѹ���frame�в���
		**/
		void PushFrame(const FunctionIndex_t& index, const FunctionParam& param);

		void PopFrame(const FunctionIndex_t& index);

	public:
		// ע�� " ��������" �� "FunctionIndex_t"
		void Register(const FunctionIndex_t& index, const FunctionName_t& name);

		// ����index��ѯfunction����
		//   �Ҳ���ʱ���ؿ��ַ���
		FunctionName_t GetFunctionName(const FunctionIndex_t& index) const;

	private:
		FuncNameMappingTable* m_pFuncNameMappingTable;
		Stack* m_pStack;

		unsigned long m_threadId;
	};
};
