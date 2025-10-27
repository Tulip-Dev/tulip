/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

#ifndef PYTHONINCLUDES_H
#define PYTHONINCLUDES_H

// Need to include cmath before Python.h when compiling with MinGW and C++11 standard
// to avoid a compilation error (see http://stackoverflow.com/questions/28683358/)
#if defined(__MINGW32__)
#include <cmath>
#endif

#if PY_MINOR_VERSION < 13
#define PY_SSIZE_T_CLEAN
#endif
#include <Python.h>
#if PY_MINOR_VERSION < 11
#include <frameobject.h>
#endif
#if PY_MINOR_VERSION < 12
#include <structmember.h>
#endif
#include <sip.h>

#include <tulip/tulipconf.h>

#define SIP_MODULE_STR STRINGIFY(SIP_MODULE)

static const sipAPIDef *sipAPIPtr = nullptr;

inline const sipAPIDef *sipAPI() {
  if (!sipAPIPtr) {
    sipAPIPtr = static_cast<const sipAPIDef *>(PyCapsule_Import(SIP_MODULE_STR "._C_API", 0));
  }

  return sipAPIPtr;
}

// #define sipMalloc sipAPI()->api_malloc
// #define sipFree sipAPI()->api_free
// #define sipBuildResult sipAPI()->api_build_result
// #define sipCallMethod sipAPI()->api_call_method
// #define sipCallProcedureMethod sipAPI()->api_call_procedure_method
// #define sipCallErrorHandler sipAPI()->api_call_error_handler
// #define sipParseResultEx sipAPI()->api_parse_result_ex
// #define sipParseResult sipAPI()->api_parse_result
// #define sipParseArgs sipAPI()->api_parse_args
// #define sipParseKwdArgs sipAPI()->api_parse_kwd_args
// #define sipParsePair sipAPI()->api_parse_pair
// #define sipInstanceDestroyed sipAPI()->api_instance_destroyed
// #define sipConvertFromSequenceIndex sipAPI()->api_convert_from_sequence_index
// #define sipConvertFromVoidPtr sipAPI()->api_convert_from_void_ptr
// #define sipConvertToVoidPtr sipAPI()->api_convert_to_void_ptr
// #define sipAddException sipAPI()->api_add_exception
// #define sipNoFunction sipAPI()->api_no_function
// #define sipNoMethod sipAPI()->api_no_method
// #define sipAbstractMethod sipAPI()->api_abstract_method
// #define sipBadClass sipAPI()->api_bad_class
// #define sipBadCatcherResult sipAPI()->api_bad_catcher_result
// #define sipBadCallableArg sipAPI()->api_bad_callable_arg
// #define sipBadOperatorArg sipAPI()->api_bad_operator_arg
// #define sipTrace sipAPI()->api_trace
// #define sipTransferBack sipAPI()->api_transfer_back
#define sipTransferTo sipAPI()->api_transfer_to
// #define sipTransferBreak sipAPI()->api_transfer_break
// #define sipSimpleWrapper_Type sipAPI()->api_simplewrapper_type
// #define sipWrapper_Type sipAPI()->api_wrapper_type
// #define sipWrapperType_Type sipAPI()->api_wrappertype_type
// #define sipVoidPtr_Type sipAPI()->api_voidptr_type
// #define sipGetPyObject sipAPI()->api_get_pyobject
// #define sipGetAddress sipAPI()->api_get_address
// #define sipGetMixinAddress sipAPI()->api_get_mixin_address
// #define sipGetCppPtr sipAPI()->api_get_cpp_ptr
// #define sipGetComplexCppPtr sipAPI()->api_get_complex_cpp_ptr
// #define sipIsPyMethod sipAPI()->api_is_py_method
// #define sipCallHook sipAPI()->api_call_hook
// #define sipEndThread sipAPI()->api_end_thread
// #define sipConnectRx sipAPI()->api_connect_rx
// #define sipDisconnectRx sipAPI()->api_disconnect_rx
// #define sipRaiseUnknownException sipAPI()->api_raise_unknown_exception
// #define sipRaiseTypeException sipAPI()->api_raise_type_exception
// #define sipBadLengthForSlice sipAPI()->api_bad_length_for_slice
// #define sipAddTypeInstance sipAPI()->api_add_type_instance
// #define sipFreeSipslot sipAPI()->api_free_sipslot
// #define sipSameSlot sipAPI()->api_same_slot
// #define sipPySlotExtend sipAPI()->api_pyslot_extend
// #define sipConvertRx sipAPI()->api_convert_rx
// #define sipAddDelayedDtor sipAPI()->api_add_delayed_dtor
#define sipCanConvertToType sipAPI()->api_can_convert_to_type
#define sipConvertToType sipAPI()->api_convert_to_type
// #define sipForceConvertToType sipAPI()->api_force_convert_to_type
// #define sipReleaseType sipAPI()->api_release_type
#define sipConvertFromType sipAPI()->api_convert_from_type
#define sipConvertFromNewType sipAPI()->api_convert_from_new_type
// #define sipConvertFromNewPyType sipAPI()->api_convert_from_new_pytype
// #define sipConvertFromEnum sipAPI()->api_convert_from_enum
// #define sipGetState sipAPI()->api_get_state
// #define sipLong_AsUnsignedLong sipAPI()->api_long_as_unsigned_long
// #define sipExportSymbol sipAPI()->api_export_symbol
// #define sipImportSymbol sipAPI()->api_import_symbol
#define sipFindType sipAPI()->api_find_type
// #define sipBytes_AsChar sipAPI()->api_bytes_as_char
// #define sipBytes_AsString sipAPI()->api_bytes_as_string
// #define sipString_AsASCIIChar sipAPI()->api_string_as_ascii_char
// #define sipString_AsASCIIString sipAPI()->api_string_as_ascii_string
// #define sipString_AsLatin1Char sipAPI()->api_string_as_latin1_char
// #define sipString_AsLatin1String sipAPI()->api_string_as_latin1_string
// #define sipString_AsUTF8Char sipAPI()->api_string_as_utf8_char
// #define sipString_AsUTF8String sipAPI()->api_string_as_utf8_string
// #define sipUnicode_AsWChar sipAPI()->api_unicode_as_wchar
// #define sipUnicode_AsWString sipAPI()->api_unicode_as_wstring
// #define sipConvertFromConstVoidPtr sipAPI()->api_convert_from_const_void_ptr
// #define sipConvertFromVoidPtrAndSize sipAPI()->api_convert_from_void_ptr_and_size
// #define sipConvertFromConstVoidPtrAndSize sipAPI()->api_convert_from_const_void_ptr_and_size
// #define sipInvokeSlot sipAPI()->api_invoke_slot
// #define sipInvokeSlotEx sipAPI()->api_invoke_slot_ex
// #define sipSaveSlot sipAPI()->api_save_slot
// #define sipClearAnySlotReference sipAPI()->api_clear_any_slot_reference
// #define sipVisitSlot sipAPI()->api_visit_slot
// #define sipWrappedTypeName(wt) ((wt)->wt_td->td_cname)
// #define sipDeprecated sipAPI()->api_deprecated
// #define sipGetReference sipAPI()->api_get_reference
// #define sipKeepReference sipAPI()->api_keep_reference
// #define sipRegisterProxyResolver sipAPI()->api_register_proxy_resolver
// #define sipRegisterPyType sipAPI()->api_register_py_type
// #define sipTypeFromPyTypeObject sipAPI()->api_type_from_py_type_object
// #define sipTypeScope sipAPI()->api_type_scope
#define sipResolveTypedef sipAPI()->api_resolve_typedef
// #define sipRegisterAttributeGetter sipAPI()->api_register_attribute_getter
// #define sipSetDestroyOnExit sipAPI()->api_set_destroy_on_exit
// #define sipEnableAutoconversion sipAPI()->api_enable_autoconversion
// #define sipInitMixin sipAPI()->api_init_mixin
// #define sipExportModule sipAPI()->api_export_module
// #define sipInitModule sipAPI()->api_init_module
// #define sipGetInterpreter sipAPI()->api_get_interpreter
// #define sipSetNewUserTypeHandler sipAPI()->api_set_new_user_type_handler
// #define sipSetTypeUserData sipAPI()->api_set_type_user_data
// #define sipGetTypeUserData sipAPI()->api_get_type_user_data
// #define sipPyTypeName sipAPI()->api_py_type_name
// #define sipGetCFunction sipAPI()->api_get_c_function
// #define sipGetMethod sipAPI()->api_get_method
// #define sipFromMethod sipAPI()->api_from_method
// #define sipGetDate sipAPI()->api_get_date
// #define sipFromDate sipAPI()->api_from_date
// #define sipGetDateTime sipAPI()->api_get_datetime
// #define sipFromDateTime sipAPI()->api_from_datetime
// #define sipGetTime sipAPI()->api_get_time
// #define sipFromTime sipAPI()->api_from_time
// #define sipIsUserType sipAPI()->api_is_user_type
// #define sipGetFrame sipAPI()->api_get_frame
// #define sipCheckPluginForType sipAPI()->api_check_plugin_for_type
// #define sipUnicodeNew sipAPI()->api_unicode_new
// #define sipUnicodeWrite sipAPI()->api_unicode_write
// #define sipUnicodeData sipAPI()->api_unicode_data
// #define sipGetBufferInfo sipAPI()->api_get_buffer_info
// #define sipReleaseBufferInfo sipAPI()->api_release_buffer_info
// #define sipIsOwnedByPython sipAPI()->api_is_owned_by_python
// #define sipIsDerivedClass sipAPI()->api_is_derived_class
// #define sipGetUserObject sipAPI()->api_get_user_object
// #define sipSetUserObject sipAPI()->api_set_user_object
// #define sipRegisterEventHandler sipAPI()->api_register_event_handler

#endif // PYTHONINCLUDES_H
