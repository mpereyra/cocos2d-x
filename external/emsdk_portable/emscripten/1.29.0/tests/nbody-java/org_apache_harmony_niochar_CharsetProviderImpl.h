#ifndef __ORG_APACHE_HARMONY_NIOCHAR_CHARSETPROVIDERIMPL__
#define __ORG_APACHE_HARMONY_NIOCHAR_CHARSETPROVIDERIMPL__

#include "xmlvm.h"

// Preprocessor constants for interfaces:
#define XMLVM_ITABLE_SIZE_org_apache_harmony_niochar_CharsetProviderImpl 0
// Implemented interfaces:
// Super Class:
#include "java_nio_charset_spi_CharsetProvider.h"

// Circular references:
#ifndef XMLVM_FORWARD_DECL_java_lang_Object
#define XMLVM_FORWARD_DECL_java_lang_Object
XMLVM_FORWARD_DECL(java_lang_Object)
#endif
#ifndef XMLVM_FORWARD_DECL_java_lang_String
#define XMLVM_FORWARD_DECL_java_lang_String
XMLVM_FORWARD_DECL(java_lang_String)
#endif
#ifndef XMLVM_FORWARD_DECL_java_lang_StringBuilder
#define XMLVM_FORWARD_DECL_java_lang_StringBuilder
XMLVM_FORWARD_DECL(java_lang_StringBuilder)
#endif
#ifndef XMLVM_FORWARD_DECL_java_nio_charset_Charset
#define XMLVM_FORWARD_DECL_java_nio_charset_Charset
XMLVM_FORWARD_DECL(java_nio_charset_Charset)
#endif
#ifndef XMLVM_FORWARD_DECL_java_security_AccessController
#define XMLVM_FORWARD_DECL_java_security_AccessController
XMLVM_FORWARD_DECL(java_security_AccessController)
#endif
#ifndef XMLVM_FORWARD_DECL_java_util_ArrayList
#define XMLVM_FORWARD_DECL_java_util_ArrayList
XMLVM_FORWARD_DECL(java_util_ArrayList)
#endif
#ifndef XMLVM_FORWARD_DECL_java_util_Collections
#define XMLVM_FORWARD_DECL_java_util_Collections
XMLVM_FORWARD_DECL(java_util_Collections)
#endif
#ifndef XMLVM_FORWARD_DECL_java_util_HashMap
#define XMLVM_FORWARD_DECL_java_util_HashMap
XMLVM_FORWARD_DECL(java_util_HashMap)
#endif
#ifndef XMLVM_FORWARD_DECL_java_util_Iterator
#define XMLVM_FORWARD_DECL_java_util_Iterator
XMLVM_FORWARD_DECL(java_util_Iterator)
#endif
#ifndef XMLVM_FORWARD_DECL_java_util_Map
#define XMLVM_FORWARD_DECL_java_util_Map
XMLVM_FORWARD_DECL(java_util_Map)
#endif
#ifndef XMLVM_FORWARD_DECL_org_apache_harmony_niochar_CharsetProviderImpl_1
#define XMLVM_FORWARD_DECL_org_apache_harmony_niochar_CharsetProviderImpl_1
XMLVM_FORWARD_DECL(org_apache_harmony_niochar_CharsetProviderImpl_1)
#endif
// Class declarations for org.apache.harmony.niochar.CharsetProviderImpl
XMLVM_DEFINE_CLASS(org_apache_harmony_niochar_CharsetProviderImpl, 8, XMLVM_ITABLE_SIZE_org_apache_harmony_niochar_CharsetProviderImpl)

extern JAVA_OBJECT __CLASS_org_apache_harmony_niochar_CharsetProviderImpl;
extern JAVA_OBJECT __CLASS_org_apache_harmony_niochar_CharsetProviderImpl_1ARRAY;
extern JAVA_OBJECT __CLASS_org_apache_harmony_niochar_CharsetProviderImpl_2ARRAY;
extern JAVA_OBJECT __CLASS_org_apache_harmony_niochar_CharsetProviderImpl_3ARRAY;
//XMLVM_BEGIN_DECLARATIONS
#define __ADDITIONAL_INSTANCE_FIELDS_org_apache_harmony_niochar_CharsetProviderImpl
//XMLVM_END_DECLARATIONS

#define __INSTANCE_FIELDS_org_apache_harmony_niochar_CharsetProviderImpl \
    __INSTANCE_FIELDS_java_nio_charset_spi_CharsetProvider; \
    struct { \
        JAVA_OBJECT cache_; \
        JAVA_OBJECT charsets_; \
        JAVA_OBJECT packageName_; \
        __ADDITIONAL_INSTANCE_FIELDS_org_apache_harmony_niochar_CharsetProviderImpl \
    } org_apache_harmony_niochar_CharsetProviderImpl

struct org_apache_harmony_niochar_CharsetProviderImpl {
    __TIB_DEFINITION_org_apache_harmony_niochar_CharsetProviderImpl* tib;
    struct {
        __INSTANCE_FIELDS_org_apache_harmony_niochar_CharsetProviderImpl;
    } fields;
};
#ifndef XMLVM_FORWARD_DECL_org_apache_harmony_niochar_CharsetProviderImpl
#define XMLVM_FORWARD_DECL_org_apache_harmony_niochar_CharsetProviderImpl
typedef struct org_apache_harmony_niochar_CharsetProviderImpl org_apache_harmony_niochar_CharsetProviderImpl;
#endif

#define XMLVM_VTABLE_SIZE_org_apache_harmony_niochar_CharsetProviderImpl 8
#define XMLVM_VTABLE_IDX_org_apache_harmony_niochar_CharsetProviderImpl_charsets__ 7
#define XMLVM_VTABLE_IDX_org_apache_harmony_niochar_CharsetProviderImpl_charsetForName___java_lang_String 6

void __INIT_org_apache_harmony_niochar_CharsetProviderImpl();
void __INIT_IMPL_org_apache_harmony_niochar_CharsetProviderImpl();
void __DELETE_org_apache_harmony_niochar_CharsetProviderImpl(void* me, void* client_data);
void __INIT_INSTANCE_MEMBERS_org_apache_harmony_niochar_CharsetProviderImpl(JAVA_OBJECT me, int derivedClassWillRegisterFinalizer);
JAVA_OBJECT __NEW_org_apache_harmony_niochar_CharsetProviderImpl();
JAVA_OBJECT __NEW_INSTANCE_org_apache_harmony_niochar_CharsetProviderImpl();
JAVA_BOOLEAN org_apache_harmony_niochar_CharsetProviderImpl_GET_HAS_LOADED_NATIVES();
void org_apache_harmony_niochar_CharsetProviderImpl_PUT_HAS_LOADED_NATIVES(JAVA_BOOLEAN v);
JAVA_INT org_apache_harmony_niochar_CharsetProviderImpl_GET_CHARSET_CLASS();
void org_apache_harmony_niochar_CharsetProviderImpl_PUT_CHARSET_CLASS(JAVA_INT v);
JAVA_INT org_apache_harmony_niochar_CharsetProviderImpl_GET_CHARSET_INSTANCE();
void org_apache_harmony_niochar_CharsetProviderImpl_PUT_CHARSET_INSTANCE(JAVA_INT v);
JAVA_INT org_apache_harmony_niochar_CharsetProviderImpl_GET_CHARSET_ALIASES();
void org_apache_harmony_niochar_CharsetProviderImpl_PUT_CHARSET_ALIASES(JAVA_INT v);
JAVA_BOOLEAN org_apache_harmony_niochar_CharsetProviderImpl_hasLoadedNatives__();
JAVA_OBJECT org_apache_harmony_niochar_CharsetProviderImpl_toUpperCase___java_lang_String(JAVA_OBJECT n1);
JAVA_BOOLEAN org_apache_harmony_niochar_CharsetProviderImpl_passthru___char(JAVA_CHAR n1);
void org_apache_harmony_niochar_CharsetProviderImpl___INIT___(JAVA_OBJECT me);
// Vtable index: 7
JAVA_OBJECT org_apache_harmony_niochar_CharsetProviderImpl_charsets__(JAVA_OBJECT me);
// Vtable index: 6
JAVA_OBJECT org_apache_harmony_niochar_CharsetProviderImpl_charsetForName___java_lang_String(JAVA_OBJECT me, JAVA_OBJECT n1);
void org_apache_harmony_niochar_CharsetProviderImpl_putCharsets___java_util_Map(JAVA_OBJECT me, JAVA_OBJECT n1);
JAVA_OBJECT org_apache_harmony_niochar_CharsetProviderImpl_getPackageName__(JAVA_OBJECT me);
JAVA_OBJECT org_apache_harmony_niochar_CharsetProviderImpl_getCharsetsInfo__(JAVA_OBJECT me);
void org_apache_harmony_niochar_CharsetProviderImpl___CLINIT_();

#endif
