#pragma once

namespace Dog {

    namespace Event {
        struct WindowResize{ int width, height; };
        struct SceneResize { int width, height; };

        struct KeyTrigger { int key; };
        struct KeyRelease { int key; };
        struct KeyDown { int key; };

        struct MouseButtonTrigger { int button; };
        struct MouseButtonRelease { int button; };
        struct MouseDown { int button; };
        struct MouseMove { float x, y; };
        struct MouseScroll { float xOffset, yOffset; };
    }

    // Event manager class
    class Events {
    public:
        // This happens when the event handle (unique pointer) goes out of scope
        template <typename EventType>
        struct SubscriptionHandleDeleter {
            void operator()(std::function<void(const EventType&)>* listener) {
                Events::Unsubscribe<EventType>(listener);
            }
        };

        template <typename EventType>
        using SubscriptionHandle = std::unique_ptr<std::function<void(const EventType&)>, SubscriptionHandleDeleter<EventType>>;

        template <typename EventType>
        static SubscriptionHandle<EventType> Subscribe(std::function<void(const EventType&)> listener) {
            auto& listeners = GetListeners<EventType>();
            auto handle = std::unique_ptr<std::function<void(const EventType&)>, SubscriptionHandleDeleter<EventType>>(new std::function<void(const EventType&)>(listener));
            listeners.push_back(handle.get());

            DOG_INFO("Subscribed to event of type {0}.", typeid(EventType).name());

            return handle;
        }

        template <typename EventType>
        static void Unsubscribe(std::function<void(const EventType&)>* listener) {
            auto& listeners = GetListeners<EventType>();
            listeners.erase(std::remove(listeners.begin(), listeners.end(), listener), listeners.end());

            DOG_INFO("Unsubscribed from event of type {0}.", typeid(EventType).name());
        }

        template <typename EventType>
        static void Publish(const EventType& event) {
            auto& listeners = GetListeners<EventType>();
            for (auto& listener : listeners) {
                (*listener)(event);
            }
        }

    private:
        template <typename EventType>
        static std::vector<std::function<void(const EventType&)>*>& GetListeners() {
            static std::unordered_map<std::type_index, std::vector<std::function<void(const EventType&)>*>> listeners;
            return listeners[std::type_index(typeid(EventType))];
        }
    };

    // Macro for subscribing to events
#define SUBSCRIBE_EVENT(EVENT_TYPE, LISTENER) \
    Events::Subscribe<EVENT_TYPE>([this](const auto& event) { this->LISTENER(event); })

#define UNSUBSCRIBE_EVENT(EVENT_TYPE, HANDLE) \
    (HANDLE).reset();

#define PUBLISH_EVENT(EVENT_TYPE, ...) \
    Events::Publish(EVENT_TYPE{__VA_ARGS__});

} // namespace Dog
