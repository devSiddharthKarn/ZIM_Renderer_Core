#include "Input.hpp"
#include "queue"

namespace zim
{
    KeyState::KeyState(KeyboardKey key, KeyboardKey modifier)
    {
        this->key = key;
        this->modifier = modifier;

        return;
    }

    KeyState::KeyState()
    {
        this->key = KeyboardKey::None;
        this->modifier = KeyboardKey::None;
    }

    void KeyState::Reset()
    {
        this->key = KeyboardKey::None;
        this->modifier = KeyboardKey::None;
    }

    struct KeyQueue::Impl_KeyQueue
    {
        std::queue<KeyState> keyStates;
        int remaining = 0;
    };

    KeyQueue::KeyQueue()
    {
        this->pImpl_KeyQueue = new KeyQueue::Impl_KeyQueue();
    }

    KeyState KeyQueue::Get()
    {
        if (this->pImpl_KeyQueue->remaining == 0)
        {
            return KeyState(KeyboardKey::None, KeyboardKey::None);
        }
        else
        {
            KeyState state = this->pImpl_KeyQueue->keyStates.front();
            this->pImpl_KeyQueue->keyStates.pop();
            this->pImpl_KeyQueue->remaining--;
            return state;
        }
    }

    void KeyQueue::Put(KeyState state)
    {
        this->pImpl_KeyQueue->keyStates.push(state);
        this->pImpl_KeyQueue->remaining++;
    }

    void KeyQueue::MakeEmpty()
    {
        std::queue<KeyState> emptyState;
        this->pImpl_KeyQueue->keyStates.swap(emptyState);
        this->pImpl_KeyQueue->remaining = 0;
    }

    Logic KeyQueue::IsEmpty() const
    {
        if (this->pImpl_KeyQueue->remaining == 0)
        {
            return Logic::True;
        }

        return Logic::False;
    }

    KeyQueue::~KeyQueue()
    {
        if (this->pImpl_KeyQueue)
        {
            delete this->pImpl_KeyQueue;
        }
    }

    KeyboardEvent::KeyboardEvent() {}
}