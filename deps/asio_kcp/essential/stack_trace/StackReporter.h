#pragma once
#include <string>
#include "../../../Essential/src/general/stack_trace/StackTraceDef.h"
namespace StackTrace
{
	namespace StackReporter
	{		
		/**
		* ���ı���ʽչʾStackRecords
		**/
		void ShowStackRecordsInTxt( const StackFrameInfo_t& stackInfo );
	}
}