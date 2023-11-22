#pragma once
#include "Registry.hpp"

inline Registry::Registry(const std::string &first_scene)
{
    _scenes.emplace(first_scene, Scene());
    _scene = _scenes.begin();
}

template <class T>
inline sparseArray<T> &Registry::register_component()//adds a new component array to our associative container
{
    _componentsArrays.emplace(std::type_index(typeid(T)), sparseArray<T>());

    _mapFunctionCreateComponent[std::type_index(typeid(T))] = [](std::any &sp_array, Registry::entity_t pos) {
        std::any_cast<sparseArray<T> &>(sp_array).template emplace_at(pos, std::nullopt);
    };

    _mapFunctionDeleteComponent[std::type_index(typeid(T))] = [](std::any &sp_array, Registry::entity_t pos) {
        std::any_cast<sparseArray<T> &>(sp_array).deactivateComponent(pos);
    };
    return get_components<T>();
}

template <class T>
inline sparseArray<T> &Registry::get_components()//are functions to retrieve the array that stores a specific type of components
{
    return std::any_cast<sparseArray<T> &>(_componentsArrays.at(std::type_index(typeid(T))));
}

template <class T>
inline sparseArray<T> const &Registry::get_components() const
{
    return std::any_cast<sparseArray<T> &>(_componentsArrays.at(std::type_index(typeid(T))));
}

inline Registry::entity_t Registry::spawn_entity()
{
    entity_t new_id = 0;

    if (!free_ids.empty()) {
        new_id = *free_ids.begin();
        free_ids.erase(new_id);
    } else
        new_id = entity_number++;

    auto it = _componentsArrays.begin();
    for (auto itC = _mapFunctionCreateComponent.begin(); it != _componentsArrays.end() ; it++, itC++) {
        itC->second(it->second, new_id);
    }

    return new_id;
}

inline Registry::entity_t Registry::entity_from_index(std::size_t idx)
{
    return idx;
}

inline void Registry::kill_entity(Registry::entity_t const &e)
{
    auto it = _componentsArrays.begin();

    if (e >= entity_number)
        return;
    for (auto itD = _mapFunctionDeleteComponent.begin(); it != _componentsArrays.end() ; it++, itD++) {
        itD->second(it->second, e);
    }
    free_ids.emplace(e);
    for (auto &[_, ids]: _tags)
        ids.erase(e);
    for (auto &[_, ids]: _scenes)
        ids.entities.erase(e);
}

template <typename T>
inline typename sparseArray<T>::reference_type Registry::add_component(Registry::entity_t const &to, T &&c)
{
    auto &value = get_components<T>()[to];
    value = c;
    return value;
}

template <typename T, typename... Params>
inline typename sparseArray<T>::reference_type Registry::emplace_component(entity_t const & to, Params &&... p )
{
    auto &comp = get_components<T>()[to];
    comp = T(p...);
    return comp;
}

template<typename T>
inline void Registry::remove_component(const Registry::entity_t &from)
{
    get_components<T>().deactivateComponent(from);
}

template<typename T>
inline bool Registry::has_component(const Registry::entity_t &entity)
{
    return get_components<T>()[entity].has_value();
}

template<class... Components, typename Function>
inline void Registry::add_system_to_current_scene(Function &&f)
{
    (*_scene).second.systems.push_back([f, this](){
        f(*this, get_components<Components>()...);
    });
}

inline void Registry::run_scene_systems()
{
    auto sceneData = (*_scene).second;

    for (auto &system: sceneData.systems)
        system();
}

inline void Registry::handle_scenes()
{
    if (_nextScene.has_value()) {
        _scene = _scenes.find(_nextScene.value());
        _nextScene.reset();
    }
}

inline void Registry::add_entity_to_scene(const entity_t &entity, const std::string &scene) 
{
    _scenes[scene].entities.emplace(entity);
}

inline void Registry::add_entity_to_current_scene(const entity_t &entity) 
{
    (*_scene).second.entities.emplace(entity);
}

inline void Registry::change_scene(const std::string &scene) 
{
    _nextScene = scene;
}

inline void Registry::add_scene(const std::string &scene, Scene &&sceneObj)
{
    _scenes.emplace(scene, sceneObj);
}

inline void Registry::tag_entity(const entity_t &entity, const std::string &tag)
{
    _tags[tag].emplace(entity);
}

inline void Registry::untag_entity(const entity_t &entity, const std::string &tag)
{
    _tags[tag].erase(entity);
}

inline const std::set<Registry::entity_t> &Registry::get_tagged_entities(const std::string &tag)
{
    return _tags[tag];
}

inline const Registry::Scene &Registry::getScene() const
{
    return (_scene->second);
}
