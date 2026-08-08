#ifndef RENDERER_HEADER
#define RENDERER_HEADER
#include "Renderers/PrimitiveRenderer.h"
#include "Renderers/Renderer2D.h"
#include "Renderers/Renderer3D.h"
#include "Renderers/TextRenderer.h"


template<typename>
constexpr bool always_false = false;

class Renderer{
public:
    Renderer();
    ~Renderer();
    template<typename T>
    void Render(T&& object){
        using U = std::remove_cvref_t<T>;
        if constexpr (std::is_same_v<U, Object2D>){
            renderer2D.Render(std::forward<T>(object));
            toFlush |=1;
        }
        else if constexpr (std::is_same_v<U, Text>){
            textRenderer.Render(std::forward<T>(object));
            toFlush |=2;
        }
        else if constexpr (std::is_same_v<U, Shape>){
            primRenderer.Render(std::forward<T>(object));
            toFlush |=4;
        }
        else if constexpr (std::is_same_v<U, Entity>){
            renderer3D.Render(std::forward<T>(object));
            toFlush |=8;
        }
        else{
            static_assert(always_false<U>, "Unsupported render type");
        }
    }
    void ReloadShaders(uint8_t);
    void Flush();

private:
    uint8_t toFlush;
    Renderer2D renderer2D;
    Renderer3D renderer3D;
    TextRenderer textRenderer;
    PrimitiveRenderer primRenderer;
};


#endif