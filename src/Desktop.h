#pragma once

#include "Timer.h"

#include <lrcpp/Frontend.h>

#include <string.h>
#include <map>

extern "C" {
    #include <lua.h>
}

namespace hc {
    class Logger;
    class Config;
    class Video;
    class Led;
    class Audio;
    class Input;
    class Perf;
    class Desktop;

    class View {
    public:
        View(Desktop* desktop) : _desktop(desktop) {}
        virtual ~View() {}

        virtual char const* getTitle() = 0;
        virtual void onStarted() {}
        virtual void onCoreLoaded() {}
        virtual void onGameLoaded() {}
        virtual void onGameStarted() {}
        virtual void onGamePaused() {}
        virtual void onGameResumed() {}
        virtual void onGameReset() {}
        virtual void onFrame() {}
        virtual void onStep() {}
        virtual void onDraw() {}
        virtual void onGameUnloaded() {}
        virtual void onCoreUnloaded() {}
        virtual void onQuit() {}

    protected:
        Desktop* _desktop;
    };

    class Desktop : public View {
    public:
        Desktop();
        virtual ~Desktop() {}

        void init();
        void addView(View* const view, bool const top, bool const free, char const* const id);

        double drawFps();
        void resetDrawFps();
        double frameFps();
        void resetFrameFps();

        template<typename T>
        T* getView() const {
            for (auto const& pair : _views) {
                View* const view = pair.second.view;

                if (dynamic_cast<T*>(view) != nullptr) {
                    return dynamic_cast<T*>(view);
                }
            }

            return nullptr;
        }

        void vprintf(retro_log_level level, char const* format, va_list args);
        void debug(char const* format, ...);
        void info(char const* format, ...);
        void warn(char const* format, ...);
        void error(char const* format, ...);

        // hc::View
        virtual void onStarted() override;
        virtual void onCoreLoaded() override;
        virtual void onGameLoaded() override;
        virtual void onGameStarted() override;
        virtual void onGamePaused() override;
        virtual void onGameResumed() override;
        virtual void onGameReset() override;
        virtual void onFrame() override;
        virtual void onStep() override;
        virtual void onDraw() override;
        virtual void onGameUnloaded() override;
        virtual void onCoreUnloaded() override;
        virtual void onQuit() override;

    protected:
        struct ViewProperties {
            View* view;
            bool top;
            bool free;
            char const* id;
            bool opened;
        };

        Logger* _logger;

        std::map<std::string, ViewProperties> _views;

        uint64_t _drawCount;
        Timer _drawTimer;

        uint64_t _frameCount;
        Timer _frameTimer;
    };
}
