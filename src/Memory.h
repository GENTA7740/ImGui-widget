#pragma once

#include "Desktop.h"
#include "PeekPoke.h"
#include "Scriptable.h"

#include <imgui.h>
#include <imgui_memory_editor.h>
#include <imguial_sparkline.h>

extern "C" {
    #include <hcdebug.h>
}

#include <string>
#include <vector>

extern "C" {
    #include <lauxlib.h>
}

namespace hc {
    class Memory : public MemoryPeek<Memory>, public MemoryPoke<Memory>, public Scriptable {
    public:
        virtual ~Memory() {}

        virtual char const* name() const = 0;
        virtual uint64_t base() const = 0;
        virtual uint64_t size() const = 0;
        virtual bool readonly() const = 0;
        virtual uint8_t peek(uint64_t address) const = 0;
        virtual void poke(uint64_t address, uint8_t value) = 0;

        unsigned requiredDigits();

        static Memory* check(lua_State* L, int index);
        static bool is(lua_State* L, int index);

        // hc::Scriptable
        virtual int push(lua_State* L) override;

    protected:
        static int l_name(lua_State* L);
        static int l_base(lua_State* L);
        static int l_size(lua_State* L);
        static int l_readonly(lua_State* L);
        static int l_peek(lua_State* L);
        static int l_poke(lua_State* L);
    };

    class MemorySelector : public View {
    public:
        MemorySelector(Desktop* desktop) : View(desktop), _selected(0), _viewCount(0) {}
        virtual ~MemorySelector() {}

        void init();
        void add(Memory* memory);

        Memory* select(char const* label, int* selected, bool button);

        // hc::View
        virtual char const* getTitle() override;
        virtual void onFrame() override;
        virtual void onDraw() override;
        virtual void onGameUnloaded() override;

    protected:
        std::vector<Memory*> _regions;
        int _selected;

        unsigned _viewCount;
    };

    class MemoryWatch : public View {
    public:
        MemoryWatch(Desktop* desktop, char const* title, Memory* memory);
        virtual ~MemoryWatch() {}

        // hc::View
        virtual char const* getTitle() override;
        virtual void onFrame() override;
        virtual void onDraw() override;
        virtual void onGameUnloaded() override;

    protected:
        enum {
            SparklineCount = 512
        };

        std::string _title;
        Memory* _memory;
        MemoryEditor _editor;

        ImGuiAl::BufferedSparkline<SparklineCount> _sparkline;
        size_t _lastPreviewAddress;
        int _lastEndianess;
        ImGuiDataType _lastType;
    };
}
