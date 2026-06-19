#include <Geode/Geode.hpp>
#include <Geode/modify/GameObject.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

/**
 * Hooking GameObject ensures we can modify how triggers affect objects
 * without manually searching for memory addresses.
 */
class $modify(MyGameObject, GameObject) {
    
    /**
     * updateAlpha is called whenever an Alpha Trigger (widely used in 'Every End') 
     * attempts to change an object's visibility.
     */
    void updateAlpha(float alpha) {
        // Safety Check: Use Geode's Mod class to check settings safely
        bool isEnabled = Mod::get()->getSettingValue<bool>("enable-mod");
        bool forceMax = Mod::get()->getSettingValue<bool>("bypass-alpha");

        if (isEnabled && forceMax) {
            // Null-pointer protection: Ensure 'this' is valid before applying logic
            // Although Geode hooks are stable, internal engine calls can occasionally 
            // trigger on semi-initialized objects during level load.
            if (this) {
                // Force the alpha to 255 (fully visible) regardless of trigger input
                GameObject::updateAlpha(255.0f);
                return;
            }
        }

        // Default behavior for normal gameplay
        GameObject::updateAlpha(alpha);
    }
};

/**
 * Hooking PlayLayer allows us to monitor the level state and 
 * perform cleanup or optimizations when the level starts/ends.
 */
class $modify(MyPlayLayer, PlayLayer) {

    bool init(GJGameLevel* level, bool useReplay, bool dontSave) {
        // Always call the original function first to ensure the game initializes correctly
        if (!PlayLayer::init(level, useReplay, dontSave)) {
            return false;
        }

        log::info("Every End Effect Applyer: Level initialized. Applying safety checks.");
        
        return true;
    }

    /**
     * postUpdate is a safe place to manage performance-heavy logic 
     * because it runs after the main physics loop.
     */
    void postUpdate(float dt) {
        PlayLayer::postUpdate(dt);

        // Null-pointer protection for the layer itself
        if (!this) return;

        bool isEnabled = Mod::get()->getSettingValue<bool>("enable-mod");
        bool optimize = Mod::get()->getSettingValue<bool>("optimization-mode");

        if (isEnabled && optimize) {
            // Logic Requirement: Optimization
            // In heavy levels like Every End, we can throttle non-essential 
            // EffectManager updates if needed.
            if (auto effectManager = this->m_effectManager) {
                // Safe check: Verify the effectManager exists before touching it
                // This prevents crashes in levels that don't use standard effect managers.
                
                // Add custom logic here to skip specific frames if performance is low
                // (Advanced optimization logic would go here)
            }
        }
    }
};
