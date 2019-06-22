
#include <iostream>
#include <Windows.h>


extern "C" {
	int32_t add_int_asm(int32_t, int32_t);
	uint32_t strlen_asm(const char* str);
	int64_t test1_asm(int64_t v);
	uint64_t test2_asm(uint64_t v);
	int64_t rdtsc_asm();
	uint32_t test_crc_asm(const char* data);
};

void test0()
{
	std::cout << add_int_asm(5, 7) << std::endl;
	std::cout << strlen_asm("toto") << std::endl;
}

void test_rdtsc()
{
	auto start = rdtsc_asm();
	Sleep(1000);
	auto end = rdtsc_asm();
	std::cout << (end - start) / (double)(1e6) << " Mhz" << std::endl;
}

/*

#define STILL_ACTIVE                        STATUS_PENDING
#define EXCEPTION_ACCESS_VIOLATION          STATUS_ACCESS_VIOLATION
#define EXCEPTION_DATATYPE_MISALIGNMENT     STATUS_DATATYPE_MISALIGNMENT
#define EXCEPTION_BREAKPOINT                STATUS_BREAKPOINT
#define EXCEPTION_SINGLE_STEP               STATUS_SINGLE_STEP
#define EXCEPTION_ARRAY_BOUNDS_EXCEEDED     STATUS_ARRAY_BOUNDS_EXCEEDED
#define EXCEPTION_FLT_DENORMAL_OPERAND      STATUS_FLOAT_DENORMAL_OPERAND
#define EXCEPTION_FLT_DIVIDE_BY_ZERO        STATUS_FLOAT_DIVIDE_BY_ZERO
#define EXCEPTION_FLT_INEXACT_RESULT        STATUS_FLOAT_INEXACT_RESULT
#define EXCEPTION_FLT_INVALID_OPERATION     STATUS_FLOAT_INVALID_OPERATION
#define EXCEPTION_FLT_OVERFLOW              STATUS_FLOAT_OVERFLOW
#define EXCEPTION_FLT_STACK_CHECK           STATUS_FLOAT_STACK_CHECK
#define EXCEPTION_FLT_UNDERFLOW             STATUS_FLOAT_UNDERFLOW
#define EXCEPTION_INT_DIVIDE_BY_ZERO        STATUS_INTEGER_DIVIDE_BY_ZERO
#define EXCEPTION_INT_OVERFLOW              STATUS_INTEGER_OVERFLOW
#define EXCEPTION_PRIV_INSTRUCTION          STATUS_PRIVILEGED_INSTRUCTION
#define EXCEPTION_IN_PAGE_ERROR             STATUS_IN_PAGE_ERROR
#define EXCEPTION_ILLEGAL_INSTRUCTION       STATUS_ILLEGAL_INSTRUCTION
#define EXCEPTION_NONCONTINUABLE_EXCEPTION  STATUS_NONCONTINUABLE_EXCEPTION
#define EXCEPTION_STACK_OVERFLOW            STATUS_STACK_OVERFLOW
#define EXCEPTION_INVALID_DISPOSITION       STATUS_INVALID_DISPOSITION
#define EXCEPTION_GUARD_PAGE                STATUS_GUARD_PAGE_VIOLATION
#define EXCEPTION_INVALID_HANDLE            STATUS_INVALID_HANDLE
#define EXCEPTION_POSSIBLE_DEADLOCK         STATUS_POSSIBLE_DEADLOCK
#define CONTROL_C_EXIT                      STATUS_CONTROL_C_EXIT
*/

void my_seh_translator(unsigned int error_code, _EXCEPTION_POINTERS * p)
{

	std::cout << "Called" << std::endl;
	std::cout << std::hex << "Address: " << p->ExceptionRecord->ExceptionAddress << std::endl;
	switch (error_code)
	{
	case EXCEPTION_ILLEGAL_INSTRUCTION:
		std::cout << "Illegal instruction1" << std::endl;
		throw std::runtime_error("Illegal instruction2");
		break;
	}
}

int main()
{
	auto ret = _set_se_translator(my_seh_translator);	
	try {
		const int64_t test_value = 0x123456789A;
		std::cout << std::hex << test1_asm(test_value) << std::endl;
		auto ret = test2_asm(0x1u << 31);
		std::cout << std::hex << ret << std::endl;

		std::cout << test_crc_asm("123456789") << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "Catched: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Some unhandled exception" << std::endl;
	}
}

