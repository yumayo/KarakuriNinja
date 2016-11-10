# pragma once

# include <memory>
# include <vector>
# include <set>

namespace User
{
    class Actor;
    class Object;
    class Component;
    class GameObject;
    class EmptyObject;

    class CPMeshFilter;
    class CPMeshRenderer;

    using ComponentPtr = Component*;
    using ComponentRef = std::shared_ptr<Component>;
    using ComponentRefs = std::vector<ComponentRef>;
    using GameObjectPtr = GameObject*;
    using GameObjectRef = std::shared_ptr<GameObject>;
    using GameObjectSee = GameObject&;
}