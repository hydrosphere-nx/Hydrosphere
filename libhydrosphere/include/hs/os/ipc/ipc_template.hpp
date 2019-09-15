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
    Invalid = 0,
    LegacyRequest,
    Close,
    LegacyControl,
    Request,
    Control,
    RequestWithContext,
    ControlWithContext
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

    /**
     * \short Push data as an out buffer (type-A buffer).
     */
    template <typename BufferT>
    void PushOutBuffer(BufferT *data, uint8_t flags = 0) {
        PushOutBufferArray(data, 1, flags);
    }

    /**
     * \short Push an array as an out buffer (type-A buffer).
     */
    template <typename BufferT>
    void PushOutBufferArray(BufferT *elements, size_t elements_count,
        uint8_t flags = 0) {
        PushBufferArray(elements, elements_count, BufferType::A, flags);
    }

    /**
     * \short Retreive the next out buffer (type-A buffer) in the message.
     */
    template <typename BufferT>
    BufferT *PopOutBuffer() {
        BufferT *res = nullptr;

        size_t n = PopOutBufferArray(&res);
        if (n != 1) {
            return nullptr;
        }

        return res;
    }

    /**
     * \short Retreive the next out buffer (type-A buffer) in the message as an array.
     */
    template <typename BufferT>
    constexpr size_t PopOutBufferArray(BufferT **buffer_ptr) {
        return PopBuffer(BufferType::A, buffer_ptr);
    }

    /**
     * \short Push data as an in buffer (type-B buffer).
     */
    template <typename BufferT>
    void PushInBuffer(BufferT *data, uint8_t flags = 0) {
        PushInBufferArray(data, 1, flags);
    }

    /**
     * \short Push an array as an in buffer (type-B buffer).
     */
    template <typename BufferT>
    void PushInBufferArray(BufferT *elements, size_t elements_count,
        uint8_t flags = 0) {
        PushBufferArray(elements, elements_count, BufferType::B, flags);
    }

    /**
     * \short Retreive the next in buffer (type-B buffer) in the message.
     */
    template <typename BufferT>
    BufferT *PopInBuffer() {
        BufferT *res = nullptr;

        size_t n = PopInBufferArray(&res);
        if (n != 1) {
            return nullptr;
        }

        return res;
    }

    /**
     * \short Retreive the next in buffer (type-B buffer) in the message as an array.
     */
    template <typename BufferT>
    constexpr size_t PopInBufferArray(BufferT **buffer_ptr) {
        return PopBuffer(BufferType::B, buffer_ptr);
    }

    /**
     * \short Push data as an out pointer (type-X buffer).
     */
    template <typename BufferT>
    void PushOutPointer(BufferT *data, uint8_t flags = 0) {
        PushOutPointerArray(data, 1, flags);
    }

    /**
     * \short Push an array as an out pointer (type-X buffer).
     */
    template <typename BufferT>
    void PushOutPointerArray(BufferT *elements, size_t elements_count,
        uint8_t flags = 0) {
        PushBufferArray(elements, elements_count, BufferType::X, flags);
    }

    /**
     * \short Retreive the next out pointer (type-X buffer) in the message.
     */
    template <typename BufferT>
    BufferT *PopOutPointer() {
        BufferT *res = nullptr;

        size_t n = PopOutPointerArray(&res);
        if (n != 1) {
            return nullptr;
        }

        return res;
    }

    /**
     * \short Retreive the next out pointer (type-X buffer) in the message as an array.
     */
    template <typename BufferT>
    constexpr size_t PopOutPointerArray(BufferT **buffer_ptr) {
        return PopBuffer(BufferType::X, buffer_ptr);
    }

    /**
     * \short Push data as an in pointer (type-C buffer).
     */
    template <typename BufferT>
    void PushInPointer(BufferT *data, uint8_t flags = 0) {
        PushInPointerArray(data, 1, flags);
    }

    /**
     * \short Push an array as an in pointer (type-C buffer).
     */
    template <typename BufferT>
    void PushInPointerArray(BufferT *elements, size_t elements_count,
        uint8_t flags = 0) {
        PushBufferArray(elements, elements_count, BufferType::C, flags);
    }

    /**
     * \short Retreive the next in pointer (type-C buffer) in the message.
     */
    template <typename BufferT>
    BufferT *PopInPointer() {
        BufferT *res = nullptr;

        size_t n = PopInPointerArray(&res);
        if (n != 1) {
            return nullptr;
        }

        return res;
    }

    /**
     * \short Retreive the next in pointer (type-C buffer) in the message as an array.
     */
    template <typename BufferT>
    constexpr size_t PopInPointerArray(BufferT **buffer_ptr) {
        return PopBuffer(BufferType::C, buffer_ptr);
    }


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

    template <typename BufferT>
    constexpr void PushBufferArray(BufferT *elements, size_t elements_count,
        BufferType buffer_type, uint8_t flags) {
        __HS_DEBUG_ASSERT(elements_count != 0);

        Buffer buffer;

        buffer.buffer_type = buffer_type;
        buffer.flags = flags;
        buffer.address = reinterpret_cast<uint64_t>(elements);
        buffer.size = sizeof(T) * elements_count;

        PushBuffer(buffer);
    }

    constexpr void PushBuffer(Buffer &buffer) {
        __HS_DEBUG_ASSERT(buffers_index < BufferCount);
        buffers[buffers_index++] = buffer;
    }

    template <typename BufferT>
    size_t PopBuffer(BufferType type, BufferT **buffer_ptr) {
        *buffer_ptr = nullptr;
        size_t buffer_count = 0;

        Buffer *buffer = PopBufferByType(type);
        if (buffer != nullptr) {
            __HS_DEBUG_ASSERT(buffer->size % sizeof(BufferT) != 0);

            // If the size is a multiple of sizeof(BufferT)
            if (buffer->size % sizeof(BufferT) == 0) {
                buffer_count = buffer->size / sizeof(BufferT);
                *buffer_ptr = reinterpret_cast<BufferT*>(buffer->address);
            }
        }

        return buffer_count;
    }

    Buffer *PopBufferByType(BufferType type) {
        for (size_t i = 0; i < buffers.size(); i++) {
            Buffer *buffer = &buffers[i];
            if (buffer->buffer_type == type) {
                // we change the buffer_type to Invalid to make sure we don't
                // get the buffer back again.
                buffer->buffer_type = BufferType::Invalid;
                return buffer;
            }
        }

        return nullptr;
    }
};

static_assert(sizeof(EmptyRaw) == 0, "EmptyRaw isn't a zero-sized type!");
}  // namespace hs::os::ipc
