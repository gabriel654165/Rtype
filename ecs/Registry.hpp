#pragma once

#include <iostream>
#include <map>
#include <typeindex>
#include <any>
#include <optional>
#include <functional>
#include <memory>
#include <list>
#include <set>

#include "sparseArray.hpp"

class Registry {
    public :
        template<typename T>
        using A = typename sparseArray<T>::container_t;
        using entity_t = std::size_t;

        /**
         * @param firstScene will be the name of the scene created by default
         **/
        Registry(const std::string &firstScene = "default");

        /**
         * @brief register a component in the registry as a usable one
         **/
        template<class A>
        sparseArray<A> &register_component();

        /**
         * @brief register a component in the registry as a usable one
         **/
        template<class A>
        sparseArray<A> &get_components();

        /**
         *
         * @tparam  Component the component as a type
         * @return returns the component list
         */
        template<class A>
        sparseArray<A> const &get_components() const;

        /**
         * @brief create an entity in the registry with no component
         * @return  returns the entity id
         */
        entity_t spawn_entity();

        /**
         *
         * @param kill an entity with an id as it's key
         */
        void kill_entity(entity_t const &e);

        /**
         *
         * @tparam Component type
         * @param to the entity which to add a component
         * @param c a component rvalue reference used to construct it
         * @return returns a reference to the new component
         */
        template<typename Component>
        typename sparseArray<Component>::reference_type add_component(entity_t const &to, Component &&c);

        /**
         *
         * @tparam Component type
         * @tparam Params automatic templating
         * @param to the entity which to emplace a component
         * @param p params for the constructor
         * @return returns a reference to the new component
         */
        template<typename Component, typename... Params>
        typename sparseArray<Component>::reference_type emplace_component(entity_t const &to, Params &&... p);

        /**
         *
         * @tparam Component
         * @param from
         */
        template<typename Component>
        void remove_component(entity_t const &from);

        /**
         *
         * @tparam Component
         * @param entity
         * @return if an entity has the component
         */
        template<typename Component>
        bool has_component(entity_t const &entity);

        // template<class... Components, typename Function>
        // void add_system(Function &&f);

        /**
         * run the current scene systems
         */
        void run_scene_systems();


        struct Scene {
            Scene() = default;
            std::list<std::function<void()>> systems;
            std::set<entity_t> entities;
        };

        const Scene &getScene() const;

        /**
         *
         * @tparam Components
         * @tparam System
         * @param params
         */
        template<class... Components, typename Function>
        void add_system_to_current_scene(Function &&f);

        /**
         *  adds a scene (system and entities)
         * @param scene
         * @param sceneObj
         */
        void add_scene(const std::string &scene, Scene &&sceneObj);

        void add_entity_to_scene(const entity_t &entity, const std::string &scene);
        void add_entity_to_current_scene(const entity_t &entity);

        void change_scene(const std::string &scene);

        void tag_entity(const entity_t &, const std::string &tag);
        void untag_entity(const entity_t &, const std::string &tag);

        /**
         *
         * @param tag
         * @return returns a set of the entities tagged with `tag`
         */
        const std::set<entity_t> &get_tagged_entities(const std::string &tag);

        /*
        * Check if a system changed the actual scene 
        */
        void handle_scenes();

    private:

        // components containers
        std::unordered_map<std::type_index, std::any> _componentsArrays;
        std::unordered_map<std::type_index, std::function<void(std::any &, std::size_t pos)>> _mapFunctionCreateComponent;
        std::unordered_map<std::type_index, std::function<void(std::any &, std::size_t pos)>> _mapFunctionDeleteComponent;

        // entity ids
        std::size_t entity_number = 0;
        std::set<entity_t> free_ids;
        entity_t entity_from_index(std::size_t idx);
  
        // scenes
        std::unordered_map<std::string, Scene> _scenes;
        std::optional<std::string> _nextScene;
        std::unordered_map<std::string, Scene>::iterator _scene;

        // tags
        std::unordered_map<std::string, std::set<entity_t>> _tags;


};

#include "RegistryImpl.hpp"