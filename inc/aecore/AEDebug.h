#ifndef AE_DEBUG_H_
#define AE_DEBUG_H_

/*!
\brief	Prints the formatted string (like printf) to the stdout
and to the Visual Studio Output window.
*/
void AEDbgPrintFunction(const char * format, ...);

/*!
\brief	Same as above but also writes to a file called Log.txt
*/
void AEDbgLogPrintFunction(const char * format, ...);

/*!
\brief	This function displays an error message box if the value of 'expression' is false.
*/
void AEDbgAssertFunction(bool expression, const char * filename, int line, const char * format, ...);

/*!
\fn		MyErrorIfFunction
\brief	This function displays an error message box if the value of 'expression' is true.
*/
void AEDbgErrorIfFunction(bool expression, const char * filename, int line, const char * format, ...);

// ---------------------------------------------------------------------------
/// assert defines

#ifdef _DEBUG
#ifdef _LOG
// __VA_ARGS__ is a special macro that replaces all the variadic arguments passed to the macro function
// https://msdn.microsoft.com/en-us/library/ms177415.aspx
#define AE_PRINT(...) MyLogPrintFunction(__VA_ARGS__)
#else
#define AE_PRINT(...) AEDbgPrintFunction(__VA_ARGS__)
#endif

// ---------------------------------------------------------------------------

#define AE_ASSERT(x) AEDbgAssertFunction((x), __FILE__, __LINE__, #x)

// ---------------------------------------------------------------------------

#define AE_ASSERT_MESG(x, ...) AEDbgAssertFunction((x), __FILE__, __LINE__, __VA_ARGS__)

// ---------------------------------------------------------------------------

#else // RELEASE

// ---------------------------------------------------------------------------
#define AE_PRINT(...)
#define AE_ASSERT(x)
#define AE_ASSERT_MESG(x, ...)

// ---------------------------------------------------------------------------

#endif // RELEASE

#endif
