/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: accessories.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "accessories.pb-c.h"
void   response__init
                     (Response         *message)
{
  static const Response init_value = RESPONSE__INIT;
  *message = init_value;
}
size_t response__get_packed_size
                     (const Response *message)
{
  assert(message->base.descriptor == &response__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t response__pack
                     (const Response *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &response__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t response__pack_to_buffer
                     (const Response *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &response__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Response *
       response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Response *)
     protobuf_c_message_unpack (&response__descriptor,
                                allocator, len, data);
}
void   response__free_unpacked
                     (Response *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &response__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   control_envelope__init
                     (ControlEnvelope         *message)
{
  static const ControlEnvelope init_value = CONTROL_ENVELOPE__INIT;
  *message = init_value;
}
size_t control_envelope__get_packed_size
                     (const ControlEnvelope *message)
{
  assert(message->base.descriptor == &control_envelope__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t control_envelope__pack
                     (const ControlEnvelope *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &control_envelope__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t control_envelope__pack_to_buffer
                     (const ControlEnvelope *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &control_envelope__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
ControlEnvelope *
       control_envelope__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (ControlEnvelope *)
     protobuf_c_message_unpack (&control_envelope__descriptor,
                                allocator, len, data);
}
void   control_envelope__free_unpacked
                     (ControlEnvelope *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &control_envelope__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor response__field_descriptors[10] =
{
  {
    "error_code",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(Response, error_code),
    &error_code__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "device_information",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Response, payload_case),
    offsetof(Response, device_information),
    &device_information__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "state",
    7,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Response, payload_case),
    offsetof(Response, state),
    &state__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "connection_details",
    8,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Response, payload_case),
    offsetof(Response, connection_details),
    &connection_details__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "device_configuration",
    10,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Response, payload_case),
    offsetof(Response, device_configuration),
    &device_configuration__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "central_information",
    13,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Response, payload_case),
    offsetof(Response, central_information),
    &central_information__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "dialog",
    14,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Response, payload_case),
    offsetof(Response, dialog),
    &dialog__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "speech_provider",
    15,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Response, payload_case),
    offsetof(Response, speech_provider),
    &speech_provider__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "locales",
    21,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Response, payload_case),
    offsetof(Response, locales),
    &locales__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "device_features",
    28,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Response, payload_case),
    offsetof(Response, device_features),
    &device_features__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned response__field_indices_by_name[] = {
  5,   /* field[5] = central_information */
  3,   /* field[3] = connection_details */
  4,   /* field[4] = device_configuration */
  9,   /* field[9] = device_features */
  1,   /* field[1] = device_information */
  6,   /* field[6] = dialog */
  0,   /* field[0] = error_code */
  8,   /* field[8] = locales */
  7,   /* field[7] = speech_provider */
  2,   /* field[2] = state */
};
static const ProtobufCIntRange response__number_ranges[7 + 1] =
{
  { 1, 0 },
  { 3, 1 },
  { 7, 2 },
  { 10, 4 },
  { 13, 5 },
  { 21, 8 },
  { 28, 9 },
  { 0, 10 }
};
const ProtobufCMessageDescriptor response__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "Response",
  "Response",
  "Response",
  "",
  sizeof(Response),
  10,
  response__field_descriptors,
  response__field_indices_by_name,
  7,  response__number_ranges,
  (ProtobufCMessageInit) response__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor control_envelope__field_descriptors[32] =
{
  {
    "command",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_ENUM,
    0,   /* quantifier_offset */
    offsetof(ControlEnvelope, command),
    &command__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "response",
    9,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, response),
    &response__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "provide_speech",
    10,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, provide_speech),
    &provide_speech__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "start_speech",
    11,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, start_speech),
    &start_speech__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "stop_speech",
    12,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, stop_speech),
    &stop_speech__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "endpoint_speech",
    13,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, endpoint_speech),
    &endpoint_speech__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "notify_speech_state",
    14,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, notify_speech_state),
    &notify_speech_state__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "get_device_information",
    20,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, get_device_information),
    &get_device_information__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "get_device_configuration",
    21,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, get_device_configuration),
    &get_device_configuration__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "override_assistant",
    22,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, override_assistant),
    &override_assistant__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "start_setup",
    23,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, start_setup),
    &start_setup__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "complete_setup",
    24,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, complete_setup),
    &complete_setup__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "notify_device_configuration",
    25,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, notify_device_configuration),
    &notify_device_configuration__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "update_device_information",
    26,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, update_device_information),
    &update_device_information__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "notify_device_information",
    27,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, notify_device_information),
    &notify_device_information__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "get_device_features",
    28,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, get_device_features),
    &get_device_features__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "upgrade_transport",
    30,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, upgrade_transport),
    &upgrade_transport__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "switch_transport",
    31,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, switch_transport),
    &switch_transport__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "forward_at_command",
    40,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, forward_at_command),
    &forward_atcommand__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "incoming_call",
    41,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, incoming_call),
    &incoming_call__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "synchronize_settings",
    50,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, synchronize_settings),
    &synchronize_settings__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "reset_connection",
    51,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, reset_connection),
    &reset_connection__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "keep_alive",
    55,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, keep_alive),
    &keep_alive__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "remove_device",
    56,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, remove_device),
    &remove_device__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "get_locales",
    57,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, get_locales),
    &get_locales__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "set_locale",
    58,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, set_locale),
    &set_locale__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "launch_app",
    59,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, launch_app),
    &launch_app__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "issue_media_control",
    60,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, issue_media_control),
    &issue_media_control__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "get_state",
    100,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, get_state),
    &get_state__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "set_state",
    101,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, set_state),
    &set_state__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "synchronize_state",
    102,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, synchronize_state),
    &synchronize_state__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "get_central_information",
    103,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(ControlEnvelope, payload_case),
    offsetof(ControlEnvelope, get_central_information),
    &get_central_information__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned control_envelope__field_indices_by_name[] = {
  0,   /* field[0] = command */
  11,   /* field[11] = complete_setup */
  5,   /* field[5] = endpoint_speech */
  18,   /* field[18] = forward_at_command */
  31,   /* field[31] = get_central_information */
  8,   /* field[8] = get_device_configuration */
  15,   /* field[15] = get_device_features */
  7,   /* field[7] = get_device_information */
  24,   /* field[24] = get_locales */
  28,   /* field[28] = get_state */
  19,   /* field[19] = incoming_call */
  27,   /* field[27] = issue_media_control */
  22,   /* field[22] = keep_alive */
  26,   /* field[26] = launch_app */
  12,   /* field[12] = notify_device_configuration */
  14,   /* field[14] = notify_device_information */
  6,   /* field[6] = notify_speech_state */
  9,   /* field[9] = override_assistant */
  2,   /* field[2] = provide_speech */
  23,   /* field[23] = remove_device */
  21,   /* field[21] = reset_connection */
  1,   /* field[1] = response */
  25,   /* field[25] = set_locale */
  29,   /* field[29] = set_state */
  10,   /* field[10] = start_setup */
  3,   /* field[3] = start_speech */
  4,   /* field[4] = stop_speech */
  17,   /* field[17] = switch_transport */
  20,   /* field[20] = synchronize_settings */
  30,   /* field[30] = synchronize_state */
  13,   /* field[13] = update_device_information */
  16,   /* field[16] = upgrade_transport */
};
static const ProtobufCIntRange control_envelope__number_ranges[8 + 1] =
{
  { 1, 0 },
  { 9, 1 },
  { 20, 7 },
  { 30, 16 },
  { 40, 18 },
  { 50, 20 },
  { 55, 22 },
  { 100, 28 },
  { 0, 32 }
};
const ProtobufCMessageDescriptor control_envelope__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "ControlEnvelope",
  "ControlEnvelope",
  "ControlEnvelope",
  "",
  sizeof(ControlEnvelope),
  32,
  control_envelope__field_descriptors,
  control_envelope__field_indices_by_name,
  8,  control_envelope__number_ranges,
  (ProtobufCMessageInit) control_envelope__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue command__enum_values_by_number[31] =
{
  { "NONE", "COMMAND__NONE", 0 },
  { "PROVIDE_SPEECH", "COMMAND__PROVIDE_SPEECH", 10 },
  { "START_SPEECH", "COMMAND__START_SPEECH", 11 },
  { "STOP_SPEECH", "COMMAND__STOP_SPEECH", 12 },
  { "ENDPOINT_SPEECH", "COMMAND__ENDPOINT_SPEECH", 13 },
  { "NOTIFY_SPEECH_STATE", "COMMAND__NOTIFY_SPEECH_STATE", 14 },
  { "GET_DEVICE_INFORMATION", "COMMAND__GET_DEVICE_INFORMATION", 20 },
  { "GET_DEVICE_CONFIGURATION", "COMMAND__GET_DEVICE_CONFIGURATION", 21 },
  { "OVERRIDE_ASSISTANT", "COMMAND__OVERRIDE_ASSISTANT", 22 },
  { "START_SETUP", "COMMAND__START_SETUP", 23 },
  { "COMPLETE_SETUP", "COMMAND__COMPLETE_SETUP", 24 },
  { "NOTIFY_DEVICE_CONFIGURATION", "COMMAND__NOTIFY_DEVICE_CONFIGURATION", 25 },
  { "UPDATE_DEVICE_INFORMATION", "COMMAND__UPDATE_DEVICE_INFORMATION", 26 },
  { "NOTIFY_DEVICE_INFORMATION", "COMMAND__NOTIFY_DEVICE_INFORMATION", 27 },
  { "GET_DEVICE_FEATURES", "COMMAND__GET_DEVICE_FEATURES", 28 },
  { "UPGRADE_TRANSPORT", "COMMAND__UPGRADE_TRANSPORT", 30 },
  { "SWITCH_TRANSPORT", "COMMAND__SWITCH_TRANSPORT", 31 },
  { "FORWARD_AT_COMMAND", "COMMAND__FORWARD_AT_COMMAND", 40 },
  { "INCOMING_CALL", "COMMAND__INCOMING_CALL", 41 },
  { "SYNCHRONIZE_SETTINGS", "COMMAND__SYNCHRONIZE_SETTINGS", 50 },
  { "RESET_CONNECTION", "COMMAND__RESET_CONNECTION", 51 },
  { "KEEP_ALIVE", "COMMAND__KEEP_ALIVE", 55 },
  { "REMOVE_DEVICE", "COMMAND__REMOVE_DEVICE", 56 },
  { "GET_LOCALES", "COMMAND__GET_LOCALES", 57 },
  { "SET_LOCALE", "COMMAND__SET_LOCALE", 58 },
  { "LAUNCH_APP", "COMMAND__LAUNCH_APP", 59 },
  { "ISSUE_MEDIA_CONTROL", "COMMAND__ISSUE_MEDIA_CONTROL", 60 },
  { "GET_STATE", "COMMAND__GET_STATE", 100 },
  { "SET_STATE", "COMMAND__SET_STATE", 101 },
  { "SYNCHRONIZE_STATE", "COMMAND__SYNCHRONIZE_STATE", 102 },
  { "GET_CENTRAL_INFORMATION", "COMMAND__GET_CENTRAL_INFORMATION", 103 },
};
static const ProtobufCIntRange command__value_ranges[] = {
{0, 0},{10, 1},{20, 6},{30, 15},{40, 17},{50, 19},{55, 21},{100, 27},{0, 31}
};
static const ProtobufCEnumValueIndex command__enum_values_by_name[31] =
{
  { "COMPLETE_SETUP", 10 },
  { "ENDPOINT_SPEECH", 4 },
  { "FORWARD_AT_COMMAND", 17 },
  { "GET_CENTRAL_INFORMATION", 30 },
  { "GET_DEVICE_CONFIGURATION", 7 },
  { "GET_DEVICE_FEATURES", 14 },
  { "GET_DEVICE_INFORMATION", 6 },
  { "GET_LOCALES", 23 },
  { "GET_STATE", 27 },
  { "INCOMING_CALL", 18 },
  { "ISSUE_MEDIA_CONTROL", 26 },
  { "KEEP_ALIVE", 21 },
  { "LAUNCH_APP", 25 },
  { "NONE", 0 },
  { "NOTIFY_DEVICE_CONFIGURATION", 11 },
  { "NOTIFY_DEVICE_INFORMATION", 13 },
  { "NOTIFY_SPEECH_STATE", 5 },
  { "OVERRIDE_ASSISTANT", 8 },
  { "PROVIDE_SPEECH", 1 },
  { "REMOVE_DEVICE", 22 },
  { "RESET_CONNECTION", 20 },
  { "SET_LOCALE", 24 },
  { "SET_STATE", 28 },
  { "START_SETUP", 9 },
  { "START_SPEECH", 2 },
  { "STOP_SPEECH", 3 },
  { "SWITCH_TRANSPORT", 16 },
  { "SYNCHRONIZE_SETTINGS", 19 },
  { "SYNCHRONIZE_STATE", 29 },
  { "UPDATE_DEVICE_INFORMATION", 12 },
  { "UPGRADE_TRANSPORT", 15 },
};
const ProtobufCEnumDescriptor command__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "Command",
  "Command",
  "Command",
  "",
  31,
  command__enum_values_by_number,
  31,
  command__enum_values_by_name,
  8,
  command__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
