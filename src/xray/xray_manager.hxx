#ifndef NOEDEN_XRAY_MANAGER_HXX
#define NOEDEN_XRAY_MANAGER_HXX

class Xray;

class XrayManager {
public:
    explicit XrayManager(ServiceLocator* svc, EventManager* events);

    ~XrayManager();

    void add_xray(std::unique_ptr<Xray>&& xray);
    void render();

private:
    /**
       * @brief Whether the Xray UI is currently visible
       */
    bool show_xray_ = true;

    Subscription<MouseDownEvent> mouse_down_sub_;
    Subscription<MouseUpEvent> mouse_up_sub_;
    Subscription<KeyDownEvent> key_down_sub_;
    Subscription<KeyUpEvent> key_up_sub_;

    EventResult on_mouse_down(const MouseDownEvent& event);
    EventResult on_mouse_up(const MouseUpEvent& event);
    EventResult on_key_down(const KeyDownEvent& event);
    EventResult on_key_up(const KeyUpEvent& event);

    std::vector<std::unique_ptr<Xray>> xrays_;

    ServiceLocator* svc_ = nullptr;
    EventManager* events_ = nullptr;
};

#endif
