#ifndef NOEDEN_REALM_HXX
#define NOEDEN_REALM_HXX

enum class RealmType;

/**
 * @brief Represents the abstract concept of a "scene", "level" or "world"
 */
class Realm {
public:
    /**
     * @brief Initializes the realm with the given service locator.
     *
     * This method is only called once after the realm has been created.
     *
     * @returns True if the realm was initialized successfully, false otherwise
     */
    [[nodiscard]] virtual bool initialize();

    /**
     * @brief Updates the realm. Called on each frame
     */
    virtual void update();

    /**
     * @brief Unloads the realm.
     *
     * Called by the realm manager, typically when this realm is being switched from.
     * It is expected that the realm will reset to its initial state when `initialize` was called.
     */
    virtual void unload();

    /**
     * @brief Loads the realm.
     *
     * Called by the realm manager, typically when this realm is being switched to.
     */
    virtual void load();

    [[nodiscard]] RealmType type() const;

    virtual ~Realm() = default;
protected:
    friend class RealmManager;
    explicit Realm(RealmType type);

    RealmType type_;
    ServiceLocator* services_ = nullptr;
    EventManager* events_ = nullptr;
};

#endif
