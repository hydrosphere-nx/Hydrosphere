/*
 * Copyright (c) 2019 Hydrosphère Developers
 *
 * Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
 * http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
 * <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
 * option. This file may not be copied, modified, or distributed
 * except according to those terms.
 */

#pragma once

#include <stddef.h>

#include <hs/diag/diag_macro.hpp>
#include <hs/os/ipc/ipc_buffer.hpp>
#include <hs/svc/svc_types.hpp>
#include <hs/util/util_api.hpp>
#include <hs/util/util_array.hpp>
#include <hs/util/util_optional.hpp>

namespace hs::os::ipc {

// Dummy zero-sized struct
struct EmptyRaw {
  int dummy[0];
};

enum class MessageType : uint8_t {
  Request = 0,
  Response = 1,
  Control = 2,
  Close = 3
};

template <typename T = EmptyRaw, uint8_t BufferCount = 8,
          uint8_t CopyHandleCount = 8, uint8_t MoveHandleCount = 8>
class Message {
 public:
    typedef Message<T, BufferCount, CopyHandleCount, MoveHandleCount> this_type;

    /**
     * \short Create a new request for the given command id.
     */
    static this_type NewRequest(uint32_t cmd_id) {
        return Message(MessageType::Request, cmd_id);
    }

    /**
     * \short Create a new response.
     */
    static this_type NewReply() {
        return Message(MessageType::Response, 0);
    }

    /**
     * \short Copy a handle over IPC.
     *
     * \remark The remote process will have a handle that points to the same
     * object.
     */
    void PushCopyHandle(hs::svc::Handle handle) {
        __HS_DEBUG_ASSERT(copy_handles_index < CopyHandleCount);
        copy_handles[copy_handles_index++] = handle;
    }

    /**
     * \short Retrieve a copied handle from this IPC message.
     *
     * \remark Those are popped in the order they were inserted.
     */
    hs::svc::Handle PopCopyHandle() {
        __HS_DEBUG_ASSERT(copy_handles_index > 0);
        return copy_handles[--copy_handles_index];
    }

    /**
     * \short Move a handle over IPC.
     *
     * \remark Once the message is sent, the handle will not exist in the current
     * process anymore.
     */
    void PushMoveHandle(hs::svc::Handle handle) {
      __HS_DEBUG_ASSERT(move_handles_index < MoveHandleCount);
      move_handles[move_handles_index++] = handle;
    }

    /**
     * \short Retrieve a moved handle from this IPC message.
     *
     * \remark Those are popped in the order they were inserted.
     */
    hs::svc::Handle PopMoveHandle() {
        __HS_DEBUG_ASSERT(move_handles_index > 0);
        return move_handles[--move_handles_index];
    }

    /**
     * \short Set the error code in the response message.
     */
    void SetErrorCode(uint32_t error_code) {
        __HS_DEBUG_ASSERT(message_type == MessageType::Response);
        this->cmd_id_or_error_code = error_code;
    }

    /**
     * \short Get the error code in the response message.
     */
    uint32_t GetError() {
        __HS_DEBUG_ASSERT(message_type == MessageType::Response);
        return this->cmd_id_or_error_code;
    }

    /**
     * \short Set the raw data of the message.
     */
    void SetRawData(T &raw_data) {
        __HS_DEBUG_ASSERT(!raw.HasValue());
        raw.SetValue(raw_data);
    }

    /**
     * \short Get the raw data of the message.
     */
    const T &GetRawData() {
        __HS_DEBUG_ASSERT(raw.HasValue());
        return raw.GetValue();
    }

    /**
     * \short Send a pid with this request message.
     */
    void SendPid() {
        __HS_DEBUG_ASSERT(message_type == MessageType::Request);
        pid.SetValue(0);
    }

    /**
     * \short Send a fake pid with this request message.
     *
     * \remark This **needs** a kernel patch to work. (See Atmosphère documentation for more details)
     */
    void SendFakePid(uint32_t fake_pid) {
        __HS_DEBUG_ASSERT(message_type == MessageType::Request);
        pid.SetValue(fake_pid);
    }

    // TODO(Kaenbyō): buffers helpers

    // TODO(Kaenbyō): pack & unpack

 private:
     Message(MessageType message_type, uint32_t cmd_id_or_error_code)
         : buffers(), copy_handles(), move_handles(), buffers_index(0),
           copy_handles_index(0), move_handles_index(0),
           message_type(message_type), raw(), pid(),
           cmd_id_or_error_code(cmd_id_or_error_code) {}

    hs::util::Array<Buffer, BufferCount> buffers;
    hs::util::Array<hs::svc::Handle, CopyHandleCount> copy_handles;
    hs::util::Array<hs::svc::Handle, MoveHandleCount> move_handles;
    // NOTE: only used when pushing buffer
    uint8_t buffers_index;
    uint8_t copy_handles_index;
    uint8_t move_handles_index;
    MessageType message_type;
    uint32_t cmd_id_or_error_code;
    hs::util::Optional<T> raw;
    hs::util::Optional<uint32_t> pid;

    void PushBuffer(Buffer buffer) {
      __HS_DEBUG_ASSERT(buffers_index < BufferCount);
      buffers[buffers_index++] = buffer;
    }

    Buffer *PopBufferByType(BufferType type) {
      for (auto buffer : this->buffers) {
        if (buffer.buffer_type == type) {
          return &buffer;
        }
      }

      return nullptr;
    }
};

static_assert(sizeof(EmptyRaw) == 0, "EmptyRaw isn't a zero-sized type!");
}  // namespace hs::os::ipc
