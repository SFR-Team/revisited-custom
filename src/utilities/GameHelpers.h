#pragma once

static app::player::Player* GetPlayer() { return ((app::player::Player*)hh::game::GameManager::GetInstance()->gameObjectLayers[11]->objects[0]); }

static Eigen::Affine3f TransformToAffine3f(const csl::math::Transform& transform) {
	Eigen::Affine3f affine;
	affine.fromPositionOrientationScale(transform.position, transform.rotation, transform.scale);
	return affine;
}

#define ENUM_MEM(index, name) { index, #name, #name }

#define CAPTION_ATTRIBUTE(NAME) \
    static const char* NAME##Caption = #NAME; \
    static const RflCustomAttribute NAME##Attributes_[] = { \
        { "Caption", &NAME##Caption, nullptr } \
    }; \
    static const RflCustomAttributes NAME##Attributes = { \
        NAME##Attributes_, sizeof(NAME##Attributes_) / sizeof(RflCustomAttribute) \
    };

#define RFL_CLASS(NAME) \
    const hh::fnd::RflClass NAME::rflClass{ \
        #NAME, \
        nullptr, \
        sizeof(NAME), \
        nullptr, \
        0, \
        spawnerMembers, \
        sizeof(spawnerMembers) / sizeof(hh::fnd::RflClassMember), \
        nullptr, \
    };

#define RFL_CLASS_WENUM(NAME) \
    const hh::fnd::RflClass NAME::rflClass{ \
        #NAME, \
        nullptr, \
        sizeof(NAME), \
        spawnerEnums, \
        sizeof(spawnerEnums) / sizeof(hh::fnd::RflClassEnum), \
        spawnerMembers, \
        sizeof(spawnerMembers) / sizeof(hh::fnd::RflClassMember), \
        nullptr, \
    };

#define RFL_TYPE_INFO_FUNCS(NAME, CONSTRUCT_BODY) \
    void* construct##NAME(void* instance, csl::fnd::IAllocator* allocator) { \
        auto* self = static_cast<NAME*>(instance); \
        CONSTRUCT_BODY \
        return self; \
    } \
    void finish##NAME(void* instance) {} \
    void clean##NAME(void* instance) {}

#define RFL_TYPE_INFO_FUNCS_CLEAN(NAME, CONSTRUCT_BODY, CLEAN_BODY) \
    void* construct##NAME(void* instance, csl::fnd::IAllocator* allocator) { \
        auto* self = static_cast<NAME*>(instance); \
        CONSTRUCT_BODY \
        return self; \
    } \
    void finish##NAME(void* instance) {\
        auto* self = static_cast<NAME*>(instance); \
        CLEAN_BODY \
    } \
    void clean##NAME(void* instance) {}

#define RFL_TYPE_INFO(NAME) \
    const hh::fnd::RflTypeInfo NAME::rflTypeInfo{ \
        #NAME, \
        #NAME, \
        construct##NAME, \
        finish##NAME, \
        clean##NAME, \
        sizeof(NAME), \
    };

#define GAMEOBJECT_CLASS_RFL(NAME, CATEGORY) \
    const hh::fnd::RflClassMember::Value gameObjectClassAttributes[]{ \
        { "category", hh::fnd::RflClassMember::Type::CSTRING, #CATEGORY }, \
    }; \
    const GameObjectClass NAME##::gameObjectClass{ \
        #NAME, \
        #NAME, \
        sizeof(NAME), \
        &NAME##::Create, \
        sizeof(gameObjectClassAttributes) / sizeof(hh::fnd::RflClassMember::Value), \
        gameObjectClassAttributes, \
        &NAME##Spawner::rflClass, \
    }; \
    const GameObjectClass* NAME##::GetClass(){ \
        return &gameObjectClass; \
    }

#define GAMEOBJECT_CLASS_RFL_OBJINFO(NAME, CATEGORY) \
    const hh::fnd::RflClassMember::Value gameObjectClassAttributes[]{ \
        { "objinfo",  hh::fnd::RflClassMember::Type::CSTRING, NAME##Info::GetClass()->name }, \
        { "category", hh::fnd::RflClassMember::Type::CSTRING, #CATEGORY } \
    }; \
    const GameObjectClass NAME##::gameObjectClass{ \
        #NAME, \
        #NAME, \
        sizeof(NAME), \
        &NAME##::Create, \
        sizeof(gameObjectClassAttributes) / sizeof(hh::fnd::RflClassMember::Value), \
        gameObjectClassAttributes, \
        &NAME##Spawner::rflClass, \
    }; \
    const GameObjectClass* NAME##::GetClass(){ \
        return &gameObjectClass; \
    }

#define GAMEOBJECT_CLASS(NAME) \
    const GameObjectClass NAME##::gameObjectClass{ \
        #NAME, \
        #NAME, \
        sizeof(NAME), \
        &NAME##::Create, \
        0, \
        nullptr, \
        nullptr, \
    }; \
    const GameObjectClass* NAME##::GetClass(){ \
        return &gameObjectClass; \
    }

#define OBJINFO_CLASS(NAME) \
    const ObjInfoClass NAME##::objInfoClass{ \
        #NAME, \
        &NAME##::Create \
    }; \
    const char* NAME##::GetInfoName(){ \
        return objInfoClass.name; \
    } \
    const ObjInfoClass* NAME##::GetClass(){ \
        return &objInfoClass; \
    }
