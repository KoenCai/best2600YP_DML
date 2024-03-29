/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: out/system.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "system.pb-c.h"
void   reset_connection__init
                     (ResetConnection         *message)
{
  static const ResetConnection init_value = RESET_CONNECTION__INIT;
  *message = init_value;
}
size_t reset_connection__get_packed_size
                     (const ResetConnection *message)
{
  assert(message->base.descriptor == &reset_connection__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t reset_connection__pack
                     (const ResetConnection *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &reset_connection__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t reset_connection__pack_to_buffer
                     (const ResetConnection *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &reset_connection__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
ResetConnection *
       reset_connection__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (ResetConnection *)
     protobuf_c_message_unpack (&reset_connection__descriptor,
                                allocator, len, data);
}
void   reset_connection__free_unpacked
                     (ResetConnection *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &reset_connection__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   synchronize_settings__init
                     (SynchronizeSettings         *message)
{
  static const SynchronizeSettings init_value = SYNCHRONIZE_SETTINGS__INIT;
  *message = init_value;
}
size_t synchronize_settings__get_packed_size
                     (const SynchronizeSettings *message)
{
  assert(message->base.descriptor == &synchronize_settings__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t synchronize_settings__pack
                     (const SynchronizeSettings *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &synchronize_settings__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t synchronize_settings__pack_to_buffer
                     (const SynchronizeSettings *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &synchronize_settings__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
SynchronizeSettings *
       synchronize_settings__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (SynchronizeSettings *)
     protobuf_c_message_unpack (&synchronize_settings__descriptor,
                                allocator, len, data);
}
void   synchronize_settings__free_unpacked
                     (SynchronizeSettings *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &synchronize_settings__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   keep_alive__init
                     (KeepAlive         *message)
{
  static const KeepAlive init_value = KEEP_ALIVE__INIT;
  *message = init_value;
}
size_t keep_alive__get_packed_size
                     (const KeepAlive *message)
{
  assert(message->base.descriptor == &keep_alive__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t keep_alive__pack
                     (const KeepAlive *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &keep_alive__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t keep_alive__pack_to_buffer
                     (const KeepAlive *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &keep_alive__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
KeepAlive *
       keep_alive__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (KeepAlive *)
     protobuf_c_message_unpack (&keep_alive__descriptor,
                                allocator, len, data);
}
void   keep_alive__free_unpacked
                     (KeepAlive *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &keep_alive__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   remove_device__init
                     (RemoveDevice         *message)
{
  static const RemoveDevice init_value = REMOVE_DEVICE__INIT;
  *message = init_value;
}
size_t remove_device__get_packed_size
                     (const RemoveDevice *message)
{
  assert(message->base.descriptor == &remove_device__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t remove_device__pack
                     (const RemoveDevice *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &remove_device__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t remove_device__pack_to_buffer
                     (const RemoveDevice *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &remove_device__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
RemoveDevice *
       remove_device__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (RemoveDevice *)
     protobuf_c_message_unpack (&remove_device__descriptor,
                                allocator, len, data);
}
void   remove_device__free_unpacked
                     (RemoveDevice *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &remove_device__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   locale__init
                     (Locale         *message)
{
  static const Locale init_value = LOCALE__INIT;
  *message = init_value;
}
size_t locale__get_packed_size
                     (const Locale *message)
{
  assert(message->base.descriptor == &locale__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t locale__pack
                     (const Locale *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &locale__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t locale__pack_to_buffer
                     (const Locale *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &locale__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Locale *
       locale__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Locale *)
     protobuf_c_message_unpack (&locale__descriptor,
                                allocator, len, data);
}
void   locale__free_unpacked
                     (Locale *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &locale__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   locales__init
                     (Locales         *message)
{
  static const Locales init_value = LOCALES__INIT;
  *message = init_value;
}
size_t locales__get_packed_size
                     (const Locales *message)
{
  assert(message->base.descriptor == &locales__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t locales__pack
                     (const Locales *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &locales__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t locales__pack_to_buffer
                     (const Locales *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &locales__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Locales *
       locales__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Locales *)
     protobuf_c_message_unpack (&locales__descriptor,
                                allocator, len, data);
}
void   locales__free_unpacked
                     (Locales *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &locales__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   get_locales__init
                     (GetLocales         *message)
{
  static const GetLocales init_value = GET_LOCALES__INIT;
  *message = init_value;
}
size_t get_locales__get_packed_size
                     (const GetLocales *message)
{
  assert(message->base.descriptor == &get_locales__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t get_locales__pack
                     (const GetLocales *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &get_locales__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t get_locales__pack_to_buffer
                     (const GetLocales *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &get_locales__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
GetLocales *
       get_locales__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (GetLocales *)
     protobuf_c_message_unpack (&get_locales__descriptor,
                                allocator, len, data);
}
void   get_locales__free_unpacked
                     (GetLocales *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &get_locales__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   set_locale__init
                     (SetLocale         *message)
{
  static const SetLocale init_value = SET_LOCALE__INIT;
  *message = init_value;
}
size_t set_locale__get_packed_size
                     (const SetLocale *message)
{
  assert(message->base.descriptor == &set_locale__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t set_locale__pack
                     (const SetLocale *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &set_locale__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t set_locale__pack_to_buffer
                     (const SetLocale *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &set_locale__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
SetLocale *
       set_locale__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (SetLocale *)
     protobuf_c_message_unpack (&set_locale__descriptor,
                                allocator, len, data);
}
void   set_locale__free_unpacked
                     (SetLocale *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &set_locale__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   launch_app__init
                     (LaunchApp         *message)
{
  static const LaunchApp init_value = LAUNCH_APP__INIT;
  *message = init_value;
}
size_t launch_app__get_packed_size
                     (const LaunchApp *message)
{
  assert(message->base.descriptor == &launch_app__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t launch_app__pack
                     (const LaunchApp *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &launch_app__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t launch_app__pack_to_buffer
                     (const LaunchApp *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &launch_app__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
LaunchApp *
       launch_app__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (LaunchApp *)
     protobuf_c_message_unpack (&launch_app__descriptor,
                                allocator, len, data);
}
void   launch_app__free_unpacked
                     (LaunchApp *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &launch_app__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor reset_connection__field_descriptors[2] =
{
  {
    "timeout",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(ResetConnection, timeout),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "force_disconnect",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_BOOL,
    0,   /* quantifier_offset */
    offsetof(ResetConnection, force_disconnect),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned reset_connection__field_indices_by_name[] = {
  1,   /* field[1] = force_disconnect */
  0,   /* field[0] = timeout */
};
static const ProtobufCIntRange reset_connection__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor reset_connection__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "ResetConnection",
  "ResetConnection",
  "ResetConnection",
  "",
  sizeof(ResetConnection),
  2,
  reset_connection__field_descriptors,
  reset_connection__field_indices_by_name,
  1,  reset_connection__number_ranges,
  (ProtobufCMessageInit) reset_connection__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor synchronize_settings__field_descriptors[2] =
{
  {
    "timestamp_hi",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(SynchronizeSettings, timestamp_hi),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "timestamp_lo",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_UINT32,
    0,   /* quantifier_offset */
    offsetof(SynchronizeSettings, timestamp_lo),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned synchronize_settings__field_indices_by_name[] = {
  0,   /* field[0] = timestamp_hi */
  1,   /* field[1] = timestamp_lo */
};
static const ProtobufCIntRange synchronize_settings__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor synchronize_settings__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "SynchronizeSettings",
  "SynchronizeSettings",
  "SynchronizeSettings",
  "",
  sizeof(SynchronizeSettings),
  2,
  synchronize_settings__field_descriptors,
  synchronize_settings__field_indices_by_name,
  1,  synchronize_settings__number_ranges,
  (ProtobufCMessageInit) synchronize_settings__init,
  NULL,NULL,NULL    /* reserved[123] */
};
#define keep_alive__field_descriptors NULL
#define keep_alive__field_indices_by_name NULL
#define keep_alive__number_ranges NULL
const ProtobufCMessageDescriptor keep_alive__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "KeepAlive",
  "KeepAlive",
  "KeepAlive",
  "",
  sizeof(KeepAlive),
  0,
  keep_alive__field_descriptors,
  keep_alive__field_indices_by_name,
  0,  keep_alive__number_ranges,
  (ProtobufCMessageInit) keep_alive__init,
  NULL,NULL,NULL    /* reserved[123] */
};
#define remove_device__field_descriptors NULL
#define remove_device__field_indices_by_name NULL
#define remove_device__number_ranges NULL
const ProtobufCMessageDescriptor remove_device__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "RemoveDevice",
  "RemoveDevice",
  "RemoveDevice",
  "",
  sizeof(RemoveDevice),
  0,
  remove_device__field_descriptors,
  remove_device__field_indices_by_name,
  0,  remove_device__number_ranges,
  (ProtobufCMessageInit) remove_device__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor locale__field_descriptors[1] =
{
  {
    "name",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Locale, name),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned locale__field_indices_by_name[] = {
  0,   /* field[0] = name */
};
static const ProtobufCIntRange locale__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor locale__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "Locale",
  "Locale",
  "Locale",
  "",
  sizeof(Locale),
  1,
  locale__field_descriptors,
  locale__field_indices_by_name,
  1,  locale__number_ranges,
  (ProtobufCMessageInit) locale__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor locales__field_descriptors[2] =
{
  {
    "supported_locales",
    1,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Locales, n_supported_locales),
    offsetof(Locales, supported_locales),
    &locale__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "current_locale",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Locales, current_locale),
    &locale__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned locales__field_indices_by_name[] = {
  1,   /* field[1] = current_locale */
  0,   /* field[0] = supported_locales */
};
static const ProtobufCIntRange locales__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor locales__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "Locales",
  "Locales",
  "Locales",
  "",
  sizeof(Locales),
  2,
  locales__field_descriptors,
  locales__field_indices_by_name,
  1,  locales__number_ranges,
  (ProtobufCMessageInit) locales__init,
  NULL,NULL,NULL    /* reserved[123] */
};
#define get_locales__field_descriptors NULL
#define get_locales__field_indices_by_name NULL
#define get_locales__number_ranges NULL
const ProtobufCMessageDescriptor get_locales__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "GetLocales",
  "GetLocales",
  "GetLocales",
  "",
  sizeof(GetLocales),
  0,
  get_locales__field_descriptors,
  get_locales__field_indices_by_name,
  0,  get_locales__number_ranges,
  (ProtobufCMessageInit) get_locales__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor set_locale__field_descriptors[1] =
{
  {
    "locale",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(SetLocale, locale),
    &locale__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned set_locale__field_indices_by_name[] = {
  0,   /* field[0] = locale */
};
static const ProtobufCIntRange set_locale__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor set_locale__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "SetLocale",
  "SetLocale",
  "SetLocale",
  "",
  sizeof(SetLocale),
  1,
  set_locale__field_descriptors,
  set_locale__field_indices_by_name,
  1,  set_locale__number_ranges,
  (ProtobufCMessageInit) set_locale__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor launch_app__field_descriptors[1] =
{
  {
    "app_id",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(LaunchApp, app_id),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned launch_app__field_indices_by_name[] = {
  0,   /* field[0] = app_id */
};
static const ProtobufCIntRange launch_app__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor launch_app__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "LaunchApp",
  "LaunchApp",
  "LaunchApp",
  "",
  sizeof(LaunchApp),
  1,
  launch_app__field_descriptors,
  launch_app__field_indices_by_name,
  1,  launch_app__number_ranges,
  (ProtobufCMessageInit) launch_app__init,
  NULL,NULL,NULL    /* reserved[123] */
};
