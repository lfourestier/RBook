MODULEDIR := $(ROOTDIR)/external/libjson
SOURCES :=   _internal/Source/internalJSONNode.cpp \
             _internal/Source/JSONPreparse.cpp \
             _internal/Source/JSONChildren.cpp \
             _internal/Source/JSONDebug.cpp \
             _internal/Source/JSONIterators.cpp \
             _internal/Source/JSONMemory.cpp \
             _internal/Source/JSONNode_Mutex.cpp \
             _internal/Source/JSONNode.cpp \
             _internal/Source/JSONWorker.cpp \
             _internal/Source/JSONWriter.cpp \
             _internal/Source/libjson.cpp \
             _internal/Source/JSONValidator.cpp \
             _internal/Source/JSONStream.cpp \
             _internal/Source/JSONAllocator.cpp 
             
#   _internal/TestSuite/main.cpp \
#   _internal/TestSuite/TestAssign.cpp \
#   _internal/TestSuite/TestChildren.cpp \
#   _internal/TestSuite/TestComments.cpp \
#	_internal/TestSuite/TestConverters.cpp \
#   _internal/TestSuite/TestCtors.cpp \
#	_internal/TestSuite/TestEquality.cpp \
#	_internal/TestSuite/TestFunctions.cpp \
#	_internal/TestSuite/TestInequality.cpp \
#	_internal/TestSuite/TestInspectors.cpp \
# 	_internal/TestSuite/TestIterators.cpp  \
#   _internal/TestSuite/TestMutex.cpp \
#	_internal/TestSuite/TestNamespace.cpp \
# 	_internal/TestSuite/TestRefCounting.cpp \
# 	_internal/TestSuite/TestSuite.cpp \
#	_internal/TestSuite/TestWriter.cpp \
#	_internal/TestSuite/TestString.cpp \
#	_internal/TestSuite/UnitTest.cpp \
#	_internal/TestSuite/TestValidator.cpp \
# 	_internal/TestSuite/TestStreams.cpp \
# 	_internal/TestSuite/TestBinary.cpp \
#	_internal/TestSuite/RunTestSuite2.cpp \
# 	_internal/TestSuite/TestSharedString.cpp \
#	_internal/TestSuite2/JSON_Base64/json_decode64.cpp \
#	_internal/TestSuite2/JSON_Base64/json_encode64.cpp \
#	_internal/TestSuite2/JSONDebug/JSON_ASSERT_SAFE.cpp \
#	_internal/TestSuite2/JSONDebug/JSON_ASSERT.cpp \
#	_internal/TestSuite2/JSONDebug/JSON_FAIL_SAFE.cpp \
#	_internal/TestSuite2/JSONDebug/JSON_FAIL.cpp \
#	_internal/TestSuite2/JSONGlobals/jsonSingleton.cpp \
#	_internal/TestSuite2/JSONValidator/isValidArray.cpp \
#	_internal/TestSuite2/JSONValidator/isValidMember.cpp \
#	_internal/TestSuite2/JSONValidator/isValidNamedObject.cpp \
#	_internal/TestSuite2/JSONValidator/isValidNumber.cpp \
#	_internal/TestSuite2/JSONValidator/isValidObject.cpp \
#	_internal/TestSuite2/JSONValidator/isValidPartialRoot.cpp \
#	_internal/TestSuite2/JSONValidator/isValidRoot.cpp \
#	_internal/TestSuite2/JSONValidator/isValidString.cpp \
#	_internal/TestSuite2/JSONValidator/securityTest.cpp \
#	_internal/TestSuite2/NumberToString/_areFloatsEqual.cpp \
#	_internal/TestSuite2/NumberToString/_atof.cpp \
#	_internal/TestSuite2/NumberToString/_ftoa.cpp \
#	_internal/TestSuite2/NumberToString/_itoa.cpp \
#	_internal/TestSuite2/NumberToString/_uitoa.cpp \
#	_internal/TestSuite2/NumberToString/getLenSize.cpp \
#	_internal/TestSuite2/NumberToString/isNumeric.cpp

SRCDIR := $(MODULEDIR)/
INCDIR := $(LOCAL_PATH)/$(MODULEDIR)/
LOCAL_SRC_FILES += $(SOURCES:%=$(SRCDIR)/%)
LOCAL_C_INCLUDES += $(INCDIR)