#pragma once

#include "AbstractWindow.h"

class IInputEventAcceptor;
namespace sdl
{
// Совершает диспетчеризацию событий SDL по категориям.
void DispatchEvent(const SDL_Event & event, IInputEventAcceptor & acceptor);
}

// Принимает события SDL, разделённые по категориям.
// Деление условное и может быть изменено.
class IInputEventAcceptor
{
public:
    virtual ~IInputEventAcceptor() = default;

    virtual void OnMouseDown(const glm::vec2 &pos) { (void)pos; }
    virtual void OnMouseMotion(const glm::vec2 &pos) { (void)pos; }
    virtual void OnMouseUp(const glm::vec2 &pos) { (void)pos; }
    virtual void OnKeyDown(const SDL_KeyboardEvent &) {}
    virtual void OnKeyUp(const SDL_KeyboardEvent &) {}
};

// Окно, совершающее диспетчеризацию событий SDL
class CAbstractInputControlWindow
        : public CAbstractWindow
        , public IInputEventAcceptor
{
protected:
    void OnWindowEvent(const SDL_Event &event) final
    {
        sdl::DispatchEvent(event, *this);
    }
};
