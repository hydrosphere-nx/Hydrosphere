#include <hs/diag.hpp>
#include <hs/os.hpp>
#include <hs/svc.hpp>
#include <hs/util.hpp>

struct Thread2Argument {
    hs::svc::Handle kernel_event_handle;
    hs::os::UserEvent user_event;
    uint32_t some_value;
};

// TODO(Kaenbyō): move this somewhere else
extern "C" __HS_ATTRIBUTE_VISIBILITY_HIDDEN void _init(void) {}

extern "C" __HS_ATTRIBUTE_VISIBILITY_HIDDEN void _fini(void) {}

__HS_ATTRIBUTE_ALIGNED(0x1000) char secondary_stack[0x1000];
void secondary_thread_entrypoint(void *arg) {
    __HS_DEBUG_LOG("secondary_thread_entrypoint");
    auto argument = reinterpret_cast<Thread2Argument*>(arg);
    hs::os::KernelEvent kevent;

    auto readable_event = hs::util::Optional<hs::svc::Handle>();
    readable_event.SetValue(argument->kernel_event_handle);

    auto writable_event = hs::util::Optional<hs::svc::Handle>();
    hs::os::LoadKernelEvent(&kevent, readable_event, writable_event);

    __HS_DEBUG_LOG("Waiting on kernel event");
    hs::os::WaitKernelEvent(&kevent);
    __HS_DEBUG_LOG("kernel event signaled");

    __HS_DEBUG_LOG("Waiting on user event");
    hs::os::WaitUserEvent(&argument->user_event);
    __HS_DEBUG_LOG("user event signaled");

    __HS_DEBUG_LOG("Now waiting 10 seconds");
    hs::os::SleepThread(10000000000);

    hs::os::DestroyKernelEvent(&kevent);
}

void test_sync_primitives() {
    hs::os::Thread secondary_thread;
    hs::os::KernelEvent kevent;
    Thread2Argument argument;

    argument.some_value = 0xDEAD;

    auto result = hs::os::CreateKernelEvent(&kevent, true);
    __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);

    hs::os::InitializeUserEvent(&argument.user_event, false, true);

    argument.kernel_event_handle = hs::os::PopReadableHandle(&kevent);

    result = hs::os::CreateThread(&secondary_thread, secondary_thread_entrypoint, &argument,
                                       secondary_stack, 0x1000, 0x2C);
    __HS_ABORT_CONDITIONAL_RESULT(result.Ok(), result);

    hs::os::StartThread(&secondary_thread);

    __HS_DEBUG_LOG("hsMain: Now waiting 2 seconds before signaling kernel event");
    hs::os::SleepThread(2000000000);
    hs::os::SignalKernelEvent(&kevent);

    __HS_DEBUG_LOG("hsMain: Now waiting 2 seconds before signaling user event");
    hs::os::SleepThread(2000000000);
    hs::os::SignalUserEvent(&argument.user_event);

    hs::os::WaitThread(&secondary_thread);
    hs::os::DestroyKernelEvent(&kevent);
    hs::os::DestroyThread(&secondary_thread);
}

void test_ipc() {
    struct test {
        uint32_t lol;
        uint32_t yay;
    };

    auto message = hs::os::ipc::Message<struct test, 0, 1, 1>::NewRequest(0);

    test raw_data;

    raw_data.lol = 0x42;
    raw_data.yay = 0xDEAD;

    message.PushCopyHandle(hs::os::GetCurrentThreadHandle());
    message.SetRawData(raw_data);
    __HS_ASSERT(message.GetRawData().yay == 0xDEAD);
}

extern"C" void hsMain(void) {
    __HS_DEBUG_LOG("Hello from hsMain");

    test_ipc();
    //test_sync_primitives();
}
